/*
 * sys.c -- system routines
 *
 * Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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

#include "config.h"

#include <sys/stat.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#if defined(_WIN32)
#       include <shlwapi.h>
#       include <windows.h>
#else
#       include <sys/stat.h>
#       include <errno.h>
#       include <string.h>
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <port/port.h>

#include "error.h"
#include "sound.h"
#include "sys.h"

static struct {
	char organization[128];
	char name[128];
} info = {
	.organization = "fr.malikania",
	.name = "molko"
};

static struct {
	char bindir[PATH_MAX];
	char datadir[PATH_MAX];
	char localedir[PATH_MAX];
} paths;

static inline char *
normalize(char *str)
{
	for (char *p = str; *p; ++p)
		if (*p == '\\')
			*p = '/';

	return str;
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
system_directory(const char *whichdir)
{
	static char path[PATH_MAX];
	static char ret[PATH_MAX];
	char *base, *binsect;

	/*
	 * Some system does not provide support (shame on you OpenBSD)
	 * to the executable path. In that case we use PREFIX+<dir>
	 * instead unless <dir> is already absolute.
	 */

	/* Component (e.g. MLK_BINDIR is set to /bin), return immediately. */
	if (absolute(whichdir))
		return whichdir;

	/* Determine base executable path. */
	if (!(base = SDL_GetBasePath())) {
		if (absolute(whichdir))
			strlcpy(ret, whichdir, sizeof (ret));
		else
			snprintf(ret, sizeof (ret), "%s/%s", MLK_PREFIX, whichdir);
	} else {
		/*
		 * Decompose the path to the given special directory by
		 * computing relative directory to it from where the
		 * binary is located.
		 *
		 * Example:
		 *
		 *   PREFIX/bin/mlk
		 *   PREFIX/share/mlk-adventure
		 *
		 * The path to the data is ../share/molko starting from
		 * the binary.
		 *
		 * Put the base path into the path and remove the value
		 * of MLK_BINDIR.
		 *
		 * Example:
		 *   from: /usr/local/bin
		 *   to:   /usr/local
		 */
		strlcpy(path, base, sizeof (path));
		SDL_free(base);

		if ((binsect = strstr(path, MLK_BINDIR)))
			*binsect = '\0';

		snprintf(ret, sizeof (ret), "%s%s", path, whichdir);
	}

	return normalize(ret);
}

static const char *
user_directory(enum sys_dir kind)
{
	/* Kept for future use. */
	(void)kind;

	static char path[PATH_MAX];
	char *pref;

	if ((pref = SDL_GetPrefPath(info.organization, info.name))) {
		strlcpy(path, pref, sizeof (path));
		SDL_free(pref);
	} else
		strlcpy(path, "./", sizeof (path));

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

static inline void
set_bindir(void)
{
	strlcpy(paths.bindir, system_directory(MLK_BINDIR), sizeof (paths.bindir));
}

static void
set_datadir(void)
{
	char *base, test[PATH_MAX];
	struct stat st;

	/*
	 * For convenient purposes, if we're running executables directly from
	 * source tree, check for libmlk-data presence and use it. Otherwise
	 * use standard system directory with the project named concatenated.
	 */
	if ((base = SDL_GetBasePath())) {
		snprintf(test, sizeof (test), "%ssrc/libmlk-data", base);

		if (stat(test, &st) == 0 && S_ISDIR(st.st_mode)) {
			strlcpy(paths.datadir, test, sizeof (paths.datadir));
			normalize(paths.datadir);
		}

		free(base);
	}

	/* Not found, use standard. */
	if (!paths.datadir[0])
		snprintf(paths.datadir, sizeof (paths.datadir), "%s/%s",
		    system_directory(MLK_DATADIR), info.name);
}

static inline void
set_localedir(void)
{
	strlcpy(paths.localedir, system_directory(MLK_BINDIR), sizeof (paths.localedir));
}

int
sys_init(const char *organization, const char *name)
{
#if defined(__MINGW64__)
	/* On MinGW buffering leads to painful debugging. */
	setbuf(stderr, NULL);
	setbuf(stdout, NULL);
#endif
	set_bindir();
	set_datadir();
	set_localedir();

	strlcpy(info.organization, organization, sizeof (info.organization));
	strlcpy(info.name, name, sizeof (info.name));

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		return errorf("%s", SDL_GetError());
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
		return errorf("%s", SDL_GetError());
	if (TTF_Init() < 0)
		return errorf("%s", SDL_GetError());
	if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG)
		return errorf("%s", SDL_GetError());
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
		return errorf("%s", SDL_GetError());

	Mix_AllocateChannels(SOUND_CHANNELS_MAX);

	return 0;
}

const char *
sys_dir(enum sys_dir kind)
{
	switch (kind) {
	case SYS_DIR_BIN:
		return paths.bindir;
	case SYS_DIR_DATA:
		return paths.datadir;
	case SYS_DIR_LOCALE:
		return paths.localedir;
	default:
		return user_directory(kind);
	}
}

int
sys_mkdir(const char *directory)
{
	char path[PATH_MAX], *p;

	/* Copy the directory to normalize and iterate over '/'. */
	strlcpy(path, directory, sizeof (path));
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
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
