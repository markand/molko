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
		return false;

	SDL_SetWindowTitle(win.win, title);

	return true;
}

uint32_t
window_get_color(void)
{
	Uint8 r = 0, g = 0, b = 0, a = 0;

	SDL_GetRenderDrawColor(win.renderer, &r, &g, &b, &a);

	return r << 24 | g << 16 | b << 8 | a;
}

void
window_set_color(uint32_t color)
{
	const Uint8 r = color >> 24 & 0xff;
	const Uint8 g = color >> 16 & 0xff;
	const Uint8 b = color >> 8  & 0xff;
	const Uint8 a = color       & 0xff;

	SDL_SetRenderDrawColor(win.renderer, r, g, b, a);
}

void
window_draw_line(int x1, int y1, int x2, int y2)
{
	SDL_RenderDrawLine(win.renderer, x1, y1, x2, y2);
}

void
window_draw_point(int x1, int y1)
{
	SDL_RenderDrawPoint(win.renderer, x1, y1);
}

void
window_draw_rectangle(bool fill, int x, int y, unsigned width, unsigned height)
{
	const SDL_Rect rect = {
		.w = width,
		.h = height,
		.x = x,
		.y = y
	};

	if (fill)
		SDL_RenderFillRect(win.renderer, &rect);
	else
		SDL_RenderDrawRect(win.renderer, &rect);
}

void
window_clear(void)
{
	SDL_RenderClear(win.renderer);
}

void
window_present(void)
{
	SDL_RenderPresent(win.renderer);
}

void
window_close(void)
{
	SDL_DestroyRenderer(win.renderer);
	SDL_DestroyWindow(win.win);
}
