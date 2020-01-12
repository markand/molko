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

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "sys.h"

bool
sys_init(void)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		return false;
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
		return false;
	if (TTF_Init() < 0)
		return false;
	if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG)
		return false;

	return true;
}

const char *
sys_datadir(void)
{
#if 0
	static char path[1024] = { 0 };

	if (path[0] == '\0') {
		char *prefix = SDL_GetBasePath();
		printf("%s\n", prefix);
		SDL_free(prefix);
	}

	return path;
#endif
	return NULL;
}

void
sys_close(void)
{
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
