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

uint32_t
window_get_color(const struct window *w)
{
	assert(w);

	Uint8 r = 0, g = 0, b = 0, a = 0;

	SDL_GetRenderDrawColor(w->renderer, &r, &g, &b, &a);

	return r << 24 | g << 16 | b << 8 | a;
}

void
window_set_color(struct window *w, uint32_t color)
{
	assert(w);

	const Uint8 r = color >> 24 & 0xff;
	const Uint8 g = color >> 16 & 0xff;
	const Uint8 b = color >> 8  & 0xff;
	const Uint8 a = color       & 0xff;

	SDL_SetRenderDrawColor(w->renderer, r, g, b, a);
}

void
window_draw_line(struct window *w, int x1, int y1, int x2, int y2)
{
	assert(w);

	SDL_RenderDrawLine(w->renderer, x1, y1, x2, y2);
}

void
window_draw_point(struct window *w, int x1, int y1)
{
	assert(w);

	SDL_RenderDrawPoint(w->renderer, x1, y1);
}

void
window_draw_rectangle(struct window *w, bool fill, int x, int y, unsigned width, unsigned height)
{
	const SDL_Rect rect = {
		.w = width,
		.h = height,
		.x = x,
		.y = y
	};
       
	assert(w);

	if (fill)
		SDL_RenderFillRect(w->renderer, &rect);
	else
		SDL_RenderDrawRect(w->renderer, &rect);
}

void
window_clear(struct window *window)
{
	assert(window);

	SDL_RenderClear(window->renderer);
}

void
window_present(struct window *window)
{
	assert(window);

	SDL_RenderPresent(window->renderer);
}

void
window_close(struct window *w)
{
	assert(window);

	SDL_DestroyRenderer(w->renderer);
	SDL_DestroyWindow(w->win);
	free(w);
}
