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

#include "window.h"

struct window {
	SDL_Window *win;
	SDL_Renderer *renderer;
};

struct window *
window_init(const char *title, unsigned width, unsigned height)
{
	struct window *w;

	assert(title);

	if (!(w = calloc(1, sizeof (struct window))))
		return NULL;
	if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_OPENGL,
	    &w->win, &w->renderer) < 0)
		goto fail;

	SDL_SetWindowTitle(w->win, title);

	return w;

fail:
	free(w);
	return NULL;
}

void
window_close(struct window *w)
{
	assert(window);

	SDL_DestroyRenderer(w->renderer);
	SDL_DestroyWindow(w->win);
	free(w);
}
