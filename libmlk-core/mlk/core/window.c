/*
 * window.c -- basic window management
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

#include <assert.h>
#include <string.h>

#include <SDL.h>

#include "err.h"
#include "util.h"
#include "window.h"
#include "window_p.h"

static struct window_handle handle = {
	.win = NULL,
	.renderer = NULL
};

static SDL_Cursor *cursors[WINDOW_CURSOR_LAST];

struct window window = {
	.handle = &handle
};

static void
load_cursors(void)
{
	cursors[WINDOW_CURSOR_ARROW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	cursors[WINDOW_CURSOR_EDIT] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
	cursors[WINDOW_CURSOR_WAIT] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);
	cursors[WINDOW_CURSOR_CROSSHAIR] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
	cursors[WINDOW_CURSOR_SIZE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
	cursors[WINDOW_CURSOR_NO] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);
	cursors[WINDOW_CURSOR_HAND] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
}

static void
load_framerate(void)
{
	SDL_DisplayMode mode;

	if (SDL_GetWindowDisplayMode(handle.win, &mode) == 0)
		window.framerate = mode.refresh_rate;
}

static void
finish_cursors(void)
{
	for (size_t i = 0; i < UTIL_SIZE(cursors); ++i)
		if (cursors[i])
			SDL_FreeCursor(cursors[i]);
}

static int
load_window(const char *title, unsigned int w, unsigned int h)
{
	return (handle.win = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, 0)) != NULL;
}

static int
load_renderer(void)
{
	return (handle.renderer = SDL_CreateRenderer(handle.win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) != NULL;
}

int
window_open(const char *title, unsigned int w, unsigned int h)
{
	assert(title);

	if (!load_window(title, w, h) || !load_renderer())
		return MLK_ERR_SDL;

	window.w = w;
	window.h = h;

	load_framerate();
	load_cursors();

	return 0;
}

void
window_set_cursor(enum window_cursor cursor)
{
	assert(cursor < WINDOW_CURSOR_LAST);

	SDL_SetCursor(cursors[cursor]);
}

void
window_finish(void)
{
	if (handle.renderer)
		SDL_DestroyRenderer(handle.renderer);
	if (handle.win)
		SDL_DestroyWindow(handle.win);

	finish_cursors();

	memset(&handle, 0, sizeof (handle));
}
