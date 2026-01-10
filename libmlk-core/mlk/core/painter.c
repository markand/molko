/*
 * painter.c -- basic drawing routines
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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

#include <math.h>

#include "color.h"
#include "painter.h"
#include "texture.h"
#include "window.h"
#include "window_p.h"

/* Current texture renderer. */
static struct mlk_texture *renderer;

struct mlk_texture *
mlk_painter_get_target(void)
{
	return renderer;
}

void
mlk_painter_set_target(struct mlk_texture *tex)
{
	renderer = tex;
	SDL_SetRenderTarget(MLK__RENDERER(), tex ? tex->handle : NULL);
}

unsigned long
mlk_painter_get_color(void)
{
	Uint8 r = 0, g = 0, b = 0, a = 0;

	SDL_GetRenderDrawColor(MLK__RENDERER(), &r, &g, &b, &a);

	return MLK_COLOR_HEX(r, g, b, a);
}

void
mlk_painter_set_color(unsigned long color)
{
	SDL_SetRenderDrawColor(
		MLK__RENDERER(),
		MLK_COLOR_R(color),
		MLK_COLOR_G(color),
		MLK_COLOR_B(color),
		MLK_COLOR_A(color)
	);
}

void
mlk_painter_draw_line(int x1, int y1, int x2, int y2)
{
	SDL_RenderLine(MLK__RENDERER(), x1, y1, x2, y2);
}

void
mlk_painter_draw_point(int x1, int y1)
{
	SDL_RenderPoint(MLK__RENDERER(), x1, y1);
}

void
mlk_painter_draw_rectangle(int x, int y, unsigned int width, unsigned int height)
{
	const SDL_FRect rect = {
		.w = width,
		.h = height,
		.x = x,
		.y = y
	};

	SDL_RenderFillRect(MLK__RENDERER(), &rect);
}

void
mlk_painter_draw_circle(int x, int y, int radius)
{
	double dx;

	for (double dy = 1; dy <= radius; dy += 1.0) {
		dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));

		SDL_RenderLine(MLK__RENDERER(), x - dx, y + dy - radius, x + dx, y + dy - radius);
		SDL_RenderLine(MLK__RENDERER(), x - dx, y - dy + radius, x + dx, y - dy + radius);
	}
}

void
mlk_painter_clear(void)
{
	SDL_RenderClear(MLK__RENDERER());
}

void
mlk_painter_present(void)
{
	SDL_RenderPresent(MLK__RENDERER());
}
