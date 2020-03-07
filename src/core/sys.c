/*
 * sys.c -- system routines
 *
 * Copyright (c) 2020 David Demelier <markand@malikania.fr>
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#if !defined(_WIN32)            /* Assuming POSIX */
#	include <sys/types.h>
#	include <dirent.h>
#endif

#include "error.h"
#include "error_p.h"
#include "sys.h"

#if defined(_WIN32)

static void
determine(char path[], size_t pathlen)
{
	char *base = SDL_GetBasePath();

	/* On Windows, the data hierarchy is the same as the project. */
	snprintf(path, pathlen, "%sassets", base);
	SDL_free(base);
}

#else                           /* Assuming POSIX */

static bool
is_absolute(const char *path)
{
	assert(path);

	return path[0] == '/';
}

static void
determine(char path[], size_t pathlen)
{
	char localassets[PATH_MAX];
	char *base = SDL_GetBasePath();
	DIR *dp;

	/* Try assets directory where executable lives. */
	snprintf(localassets, sizeof (localassets), "%sassets", base);

	if ((dp = opendir(localassets))) {
		snprintf(path, pathlen, "%sassets", base);
		closedir(dp);
	} else {
		/* We are not in the project source directory. */
		if (is_absolute(SHAREDIR)) {
			/* SHAREDIR is absolute */
			snprintf(path, pathlen, "%s/molko", SHAREDIR);
		} else if (is_absolute(BINDIR)) {
			/* SHAREDIR is relative but BINDIR is absolute */
			snprintf(path, pathlen, "%s/%s/molko", PREFIX, SHAREDIR);
		} else {
			/* SHAREDIR, BINDIR are both relative */
			char *ptr = strstr(base, BINDIR);

			if (ptr) {
				*ptr = '\0';
				snprintf(path, pathlen, "%s%s/molko", base, SHAREDIR);
			} else {
				/* Unable to determine. */
				snprintf(path, pathlen, ".");
			}
		}
	}

	SDL_free(base);
}

#endif

bool
sys_init(void)
{
#if defined(__MINGW64__)
	/* On MinGW buffering leads to painful debugging. */
	setbuf(stderr, NULL);
	setbuf(stdout, NULL);
#endif

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		return error_sdl();
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
		return error_sdl();
	if (TTF_Init() < 0)
		return error_sdl();
	if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG)
		return error_sdl();

	return true;
}

const char *
sys_datadir(void)
{
	static char path[PATH_MAX];

	if (path[0] == '\0')
		determine(path, sizeof (path));

	return path;
}

const char *
sys_datapath(const char *fmt, ...)
{
	const char *ret;
	va_list ap;

	va_start(ap, fmt);
	ret = sys_datapathv(fmt, ap);
	va_end(ap);

	return ret;
}

const char *
sys_datapathv(const char *fmt, va_list ap)
{
	static char path[PATH_MAX];
	char filename[FILENAME_MAX];

	vsnprintf(filename, sizeof (filename), fmt, ap);
	snprintf(path, sizeof (path), "%s/%s", sys_datadir(), filename);

	return path;
}

const char *
sys_savepath(unsigned int idx)
{
	static char path[PATH_MAX];
	char *pref;

	if ((pref = SDL_GetPrefPath("malikania", "molko"))) {
		snprintf(path, sizeof (path), "%ssave-%u", idx);
		SDL_free(pref);
	} else
		snprintf(path, sizeof (path), "save-%u", idx);

	return path;
}

void
sys_close(void)
{
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
