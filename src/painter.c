/*
 * painter.c -- basic drawing routines
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

#include "color.h"
#include "painter.h"
#include "texture_p.h"
#include "window_p.h"

void
painter_use(struct texture *tex)
{
	SDL_SetRenderTarget(win.renderer, tex ? tex->handle : NULL);
}

uint32_t
painter_get_color(void)
{
	Uint8 r = 0, g = 0, b = 0, a = 0;

	SDL_GetRenderDrawColor(win.renderer, &r, &g, &b, &a);

	return COLOR_HEX(r, g, b, a);
}

void
painter_set_color(uint32_t color)
{
	SDL_SetRenderDrawColor(
		win.renderer,
		COLOR_R(color),
		COLOR_G(color),
		COLOR_B(color),
		COLOR_A(color)
	);
}

void
painter_draw_line(int x1, int y1, int x2, int y2)
{
	SDL_RenderDrawLine(win.renderer, x1, y1, x2, y2);
}

void
painter_draw_point(int x1, int y1)
{
	SDL_RenderDrawPoint(win.renderer, x1, y1);
}

void
painter_draw_rectangle(bool fill, int x, int y, unsigned width, unsigned height)
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
painter_clear(void)
{
	SDL_RenderClear(win.renderer);
}

void
painter_present(void)
{
	SDL_RenderPresent(win.renderer);
}

