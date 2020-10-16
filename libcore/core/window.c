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
#include <string.h>

#include <SDL.h>

#include "error_p.h"
#include "window.h"
#include "window_p.h"

static struct window_handle handle = {
	.win = NULL,
	.renderer = NULL
};

struct window window = {
	.handle = &handle
};

bool
window_open(const char *title, unsigned int w, unsigned int h)
{
	assert(title);

	if (SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_OPENGL,
	    &handle.win, &handle.renderer) < 0)
		return error_sdl();

	SDL_SetWindowTitle(handle.win, title);

	window.w = w;
	window.h = h;

	return true;
}

void
window_finish(void)
{
	if (handle.renderer)
		SDL_DestroyRenderer(handle.renderer);
	if (handle.win)
		SDL_DestroyWindow(handle.win);

	memset(&handle, 0, sizeof (handle));
}
