/*
 * sys.c -- system routines
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>

#include <mlk/util/util.h>

#include <sys/stat.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#if defined(_WIN32)
#       include <shlwapi.h>
#       include <windows.h>
#else
#       include <errno.h>
#       include <string.h>
#endif

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <sndfile.h>

#include <assets/misc/gamecontrollerdb.h>

#include "alloc.h"
#include "core_p.h"
#include "err.h"
#include "panic.h"
#include "sound.h"
#include "sys.h"
#include "sys_p.h"

ALCdevice *mlk__audio_dev = NULL;
ALCcontext *mlk__audio_ctx = NULL;

static struct {
	char organization[128];
	char name[128];
} info = {
	.organization = "fr.malikania",
	.name = "molko"
};

struct viodata {
	const unsigned char *data;
	const size_t datasz;
	sf_count_t offset;
};

static inline char *
normalize(char *str)
{
	for (char *p = str; *p; ++p)
		if (*p == '\\')
			*p = '/';

	return str;
}

static const char *
user_directory(enum mlk_sys_dir kind)
{
	/* Kept for future use. */
	(void)kind;

	static MLK_THREAD_LOCAL char path[MLK_PATH_MAX];
	char *pref;

	if ((pref = SDL_GetPrefPath(info.organization, info.name))) {
		mlk_util_strlcpy(path, pref, sizeof (path));
		SDL_free(pref);
	} else
		mlk_util_strlcpy(path, "./", sizeof (path));

	return path;
}

static inline int
absolute(const char *path)
{
#if defined(_WIN32)
	return !PathIsRelativeA(path);
#else
	/* Assuming UNIX like. */
	if (path[0] == '/')
		return 1;

	return 0;
#endif
}

static const char *
system_directory(const char *)
{
#if 0
	static MLK_THREAD_LOCAL char ret[PATH_MAX];
	const char *base, *binsect, path[PATH_MAX];

	/*
	 * Some system does not provide support (shame on you OpenBSD)
	 * to the executable path. In that case we use PREFIX+<dir>
	 * instead unless <dir> is already absolute.
	 */

	/*
	 * If requested directory is absolute return immediately.
	 *
	 * e.g. whichdir == /usr/share  -> return immediately
	 *      whichdir == bin         -> will be computed
	 */
	if (absolute(whichdir))
		return whichdir;

	/*
	 * If MLK_BINDIR is absolute then we're unable to compute whichdir which
	 * now is mandatory relative. In that case return its whole path to the
	 * prefix.
	 */
	if (absolute(MLK_BINDIR) || !(base = SDL_GetBasePath()))
		snprintf(ret, sizeof (ret), "%s/%s", MLK_PREFIX, whichdir);
	else {
		/*
		 * Decompose the path to the given special directory by
		 * computing relative directory to it from where the
		 * binary is located.
		 *
		 * Example:
		 *
		 *   PREFIX/bin/<executable>
		 *   PREFIX/share/mysupergame
		 *
		 * The path to the data is ../share/mysupergame starting from
		 * the binary.
		 *
		 * Put the base path into the path and remove the value
		 * of MLK_BINDIR.
		 *
		 * Example:
		 *   from: /usr/local/bin
		 *   to:   /usr/local
		 */
		mlk_util_strlcpy(path, base, sizeof (path));
		SDL_free(base);

		/* TODO: remove using negative offset. */
		if ((binsect = strstr(path, MLK_BINDIR)))
			*binsect = '\0';

		snprintf(ret, sizeof (ret), "%s%s", path, whichdir);
	}

	return normalize(ret);
#endif
	return "";
}

static inline int
mkpath(const char *path)
{
#ifdef _WIN32
	/* TODO: add error using the convenient FormatMessage function. */
	if (!CreateDirectoryA(path, NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
		return -1;
#else
	if (mkdir(path, 0755) < 0 && errno != EEXIST)
		return mlk_errf("%s", strerror(errno));
#endif

	return 0;
}

static sf_count_t
vio_get_filelen(void *data)
{
	const struct viodata *vio = data;

	return (sf_count_t)vio->datasz;
}

static sf_count_t
vio_seek(sf_count_t offset, int whence, void *data)
{
	struct viodata *vio = data;

	switch (whence) {
	case SEEK_SET:
		vio->offset = offset;
		break;
	case SEEK_CUR:
		vio->offset += offset;
		break;
	case SEEK_END:
		vio->offset = vio->datasz + offset;
		break;
	default:
		break;
	}

	return vio->offset;
}

static sf_count_t
vio_read(void *ptr, sf_count_t count, void *data)
{
	struct viodata *vio = data;

	if (vio->offset + (size_t)count > vio->datasz)
		count = vio->datasz - vio->offset;

	memcpy(ptr, vio->data + vio->offset, count);
	vio->offset += count;

	return count;
}

static sf_count_t
vio_tell(void *data)
{
	const struct viodata *vio = data;

	return vio->offset;
}

static void
audio_finish(void)
{
	if (mlk__audio_ctx) {
		alcMakeContextCurrent(NULL);
		alcDestroyContext(mlk__audio_ctx);
		mlk__audio_ctx = NULL;
	}
	if (mlk__audio_dev) {
		alcCloseDevice(mlk__audio_dev);
		mlk__audio_dev = NULL;
	}
}

int
mlk_sys_init(const char *organization, const char *name)
{
#if defined(__MINGW64__)
	/* On MinGW buffering leads to painful debugging. */
	setbuf(stderr, NULL);
	setbuf(stdout, NULL);
#endif

	/* Kept for future use. */
	(void)organization;
	(void)name;

	/* SDL. */
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD))
		return mlk_errf("%s", SDL_GetError());
	if (!TTF_Init())
		return mlk_errf("%s", SDL_GetError());

#if 0
	/*
	 * TODO: our bcc invocation creates a not NUL terminated array by
	 * default so we have to create a dummy string just for this function.
	 *
	 * As long as our CMake macros allows better control of that, change
	 * this ugly hack.
	 */
	if ((ops = SDL_RWFromConstMem(assets_misc_gamecontrollerdb, sizeof (assets_misc_gamecontrollerdb))))
		SDL_AddGamepadMappingsFromRW(ops, 1);
#endif

	/* OpenAL. */
#if defined(MLK_OS_WINDOW)
	SetEnvironmentVariable("ALSOFT_LOGLEVEL", "0");
#else
	putenv("ALSOFT_LOGLEVEL=0");
#endif

	if (!(mlk__audio_dev = alcOpenDevice(NULL))) {
		mlk_errf(_("unable to open audio device"));
		goto err;
	}
	if (!(mlk__audio_ctx = alcCreateContext(mlk__audio_dev, NULL))) {
		mlk_errf("%s", alcGetError(mlk__audio_dev));
		goto err;
	}
	if (alcMakeContextCurrent(mlk__audio_ctx) != ALC_TRUE) {
		mlk_errf("%s", alcGetError(mlk__audio_dev));
		goto err;
	}

	return 0;

err:
	audio_finish();

	return -1;
}

const char *
mlk_sys_dir(enum mlk_sys_dir kind)
{
	assert(kind >= 0 && kind < MLK_SYS_DIR_LAST);

	switch (kind) {
	case MLK_SYS_DIR_SAVE:
		return user_directory(kind);
	case MLK_SYS_DIR_LOCALES:
		return system_directory(MLK_LOCALEDIR);
	default:
		break;
	}

	return NULL;
}

int
mlk_sys_mkdir(const char *directory)
{
	char path[MLK_PATH_MAX], *p;

	/* Copy the directory to normalize and iterate over '/'. */
	mlk_util_strlcpy(path, directory, sizeof (path));
	normalize(path);

#if defined(_WIN32)
	/* Remove drive letter that we don't need. */
	if ((p = strchr(path, ':')))
		++p;
	else
		p = path;
#else
	p = path;
#endif

	for (p = p + 1; *p; ++p) {
		if (*p == '/') {
			*p = 0;

			if (mkpath(path) < 0)
				return -1;

			*p = '/';
		}
	}

	return mkpath(path);
}

void
mlk_sys_finish(void)
{
	audio_finish();
}

static int
create_audiostream(struct mlk__audiostream **ptr, SNDFILE *file, const SF_INFO *info)
{
	struct mlk__audiostream *stream;
	int ret = 0;

	stream = mlk_alloc_new(1, sizeof (*stream));
	stream->samplerate = info->samplerate;
	stream->samplesz = info->frames * info->channels;
	stream->samples = mlk_alloc_new(stream->samplesz, sizeof (*stream->samples));
	stream->format = info->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

	sf_command(file, SFC_SET_SCALE_FLOAT_INT_READ, NULL, SF_TRUE);

	if (sf_read_short(file, stream->samples, stream->samplesz) != stream->samplesz) {
		free(stream->samples);
		free(stream);
		stream = NULL;
		ret = mlk_errf("%s", sf_error(file));
	}

	alGenBuffers(1, &stream->buffer);
	alBufferData(stream->buffer, stream->format, stream->samples,
	    stream->samplesz * sizeof (*stream->samples), stream->samplerate);
	alGenSources(1, &stream->source);
	alSourcei(stream->source, AL_BUFFER, stream->buffer);

	sf_close(file);

	*ptr = stream;

	return ret;
}

int
mlk__audiostream_open(struct mlk__audiostream **stream, const char *path)
{
	assert(path);

	SF_INFO info;
	SNDFILE *file;

	if (!(file = sf_open(path, SFM_READ, &info)))
		return sf_error(NULL);

	return create_audiostream(stream, file, &info);
}

int
mlk__audiostream_openmem(struct mlk__audiostream **stream, const void *data, size_t datasz)
{
	assert(data);

	SF_VIRTUAL_IO io = {
		.get_filelen = vio_get_filelen,
		.seek = vio_seek,
		.read = vio_read,
		.tell = vio_tell
	};
	SF_INFO info;
	SNDFILE *file;
	struct viodata viodata = {
		.data = data,
		.datasz = datasz,
	};

	if (!(file = sf_open_virtual(&io, SFM_READ, &info, &viodata)))
		return mlk_errf("%s", sf_strerror(NULL));

	return create_audiostream(stream, file, &info);
}

void
mlk__audiostream_finish(struct mlk__audiostream *s)
{
	assert(s);

	alDeleteBuffers(1, &s->buffer);
	alSourcei(s->source, AL_BUFFER, 0);
	alDeleteSources(1, &s->source);
}
