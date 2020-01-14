/*
 * window.c -- basic window management
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

#include <SDL.h>

#include "error_p.h"
#include "window.h"
#include "window_p.h"

/* global object, used by textures */
struct window win = {
	.win = NULL,
	.renderer = NULL
};

bool
window_init(const char *title, unsigned width, unsigned height)
{
	assert(title);

	if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_OPENGL,
	    &win.win, &win.renderer) < 0)
		return error_sdl();

	SDL_SetWindowTitle(win.win, title);

	return true;
}

void
window_close(void)
{
	SDL_DestroyRenderer(win.renderer);
	SDL_DestroyWindow(win.win);
}
