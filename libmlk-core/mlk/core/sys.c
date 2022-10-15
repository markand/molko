/*
 * sys.c -- system routines
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <sndfile.h>

#include <mlk/util/util.h>

#include "alloc.h"
#include "error.h"
#include "panic.h"
#include "sound.h"
#include "sys.h"
#include "sys_p.h"

ALCdevice *audio_dev = NULL;
ALCcontext *audio_ctx = NULL;

static struct {
	char organization[128];
	char name[128];
} info = {
	.organization = "fr.malikania",
	.name = "molko"
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
user_directory(enum sys_dir kind)
{
	/* Kept for future use. */
	(void)kind;

	static char path[PATH_MAX];
	char *pref;

	if ((pref = SDL_GetPrefPath(info.organization, info.name))) {
		util_strlcpy(path, pref, sizeof (path));
		SDL_free(pref);
	} else
		util_strlcpy(path, "./", sizeof (path));

	return path;
}

static inline int
mkpath(const char *path)
{
#ifdef _WIN32
	/* TODO: add error using the convenient FormatMessage function. */
	if (!CreateDirectoryA(path, NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
		return errorf("unable to create directory: %s", path);
#else
	if (mkdir(path, 0755) < 0 && errno != EEXIST)
		return errorf("%s", strerror(errno));
#endif

	return 0;
}

int
sys_init(const char *organization, const char *name)
{
#if defined(__MINGW64__)
	/* On MinGW buffering leads to painful debugging. */
	setbuf(stderr, NULL);
	setbuf(stdout, NULL);
#endif

	/* Kept for future use. */
	(void)organization;
	(void)name;

	/* SDL2. */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		return errorf("%s", SDL_GetError());
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
		return errorf("%s", SDL_GetError());
	if (TTF_Init() < 0)
		return errorf("%s", SDL_GetError());

	/* OpenAL. */
#if defined(_WIN32)
	SetEnvironmentVariable("ALSOFT_LOGLEVEL", "0");
#else
	putenv("ALSOFT_LOGLEVEL=0");
#endif

	if (!(audio_dev = alcOpenDevice(NULL)))
		return errorf("unable to create audio device");
	if (!(audio_ctx = alcCreateContext(audio_dev, NULL)))
		return errorf("unable to create audio context");

	alcMakeContextCurrent(audio_ctx);

	return 0;
}

const char *
sys_dir(enum sys_dir kind)
{
	return user_directory(kind);
}

int
sys_mkdir(const char *directory)
{
	char path[PATH_MAX], *p;

	/* Copy the directory to normalize and iterate over '/'. */
	util_strlcpy(path, directory, sizeof (path));
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
sys_finish(void)
{
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	alcMakeContextCurrent(NULL);

	if (audio_ctx) {
		alcDestroyContext(audio_ctx);
		audio_ctx = NULL;
	}
	if (audio_dev) {
		alcCloseDevice(audio_dev);
		audio_dev = NULL;
	}
}

struct audiostream *
audiostream_create(SNDFILE *file, const SF_INFO *info)
{
	struct audiostream *stream;

	stream = alloc_new(sizeof (*stream));
	stream->samplerate = info->samplerate;
	stream->samplesz = info->frames * info->channels;
	stream->samples = alloc_array(stream->samplesz, sizeof (*stream->samples));
	stream->format = info->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

	sf_command(file, SFC_SET_SCALE_FLOAT_INT_READ, NULL, SF_TRUE);

	if (sf_read_short(file, stream->samples, stream->samplesz) != stream->samplesz) {
		free(stream->samples);
		free(stream);
		stream = NULL;
	}

	alGenBuffers(1, &stream->buffer);
	alBufferData(stream->buffer, stream->format, stream->samples,
	    stream->samplesz * sizeof (*stream->samples), stream->samplerate);
	alGenSources(1, &stream->source);
	alSourcei(stream->source, AL_BUFFER, stream->buffer);

	sf_close(file);

	return stream;
}

struct audiostream *
audiostream_open(const char *path)
{
	assert(path);

	SF_INFO info;
	SNDFILE *file;

	if (!(file = sf_open(path, SFM_READ, &info)))
		return NULL;

	return audiostream_create(file, &info);
}

struct viodata {
	const unsigned char *data;
	const size_t datasz;
	sf_count_t offset;
};

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
		vio->offset = vio->datasz - offset;
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

struct audiostream *
audiostream_openmem(const void *data, size_t datasz)
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
		return NULL;

	return audiostream_create(file, &info);
}

void
audiostream_finish(struct audiostream *s)
{
	assert(s);

	alDeleteBuffers(1, &s->buffer);
	alSourcei(s->source, AL_BUFFER, 0);
	alDeleteSources(1, &s->source);
}
