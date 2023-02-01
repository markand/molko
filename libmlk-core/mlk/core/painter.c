/*
 * painter.c -- basic drawing routines
 *
 * Copyright (c) 2020-2023 David Demelier <markand@malikania.fr>
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
static struct texture *renderer;

struct texture *
painter_get_target(void)
{
	return renderer;
}

void
painter_set_target(struct texture *tex)
{
	renderer = tex;
	SDL_SetRenderTarget(RENDERER(), tex ? tex->handle : NULL);
}

unsigned long
painter_get_color(void)
{
	Uint8 r = 0, g = 0, b = 0, a = 0;

	SDL_GetRenderDrawColor(RENDERER(), &r, &g, &b, &a);

	return COLOR_HEX(r, g, b, a);
}

void
painter_set_color(unsigned long color)
{
	SDL_SetRenderDrawColor(
		RENDERER(),
		COLOR_R(color),
		COLOR_G(color),
		COLOR_B(color),
		COLOR_A(color)
	);
}

void
painter_draw_line(int x1, int y1, int x2, int y2)
{
	SDL_RenderDrawLine(RENDERER(), x1, y1, x2, y2);
}

void
painter_draw_point(int x1, int y1)
{
	SDL_RenderDrawPoint(RENDERER(), x1, y1);
}

void
painter_draw_rectangle(int x, int y, unsigned int width, unsigned int height)
{
	const SDL_Rect rect = {
		.w = width,
		.h = height,
		.x = x,
		.y = y
	};

	SDL_RenderFillRect(RENDERER(), &rect);
}

void
painter_draw_circle(int x, int y, int radius)
{
	// Note that there is more to altering the bitrate of this 
	// method than just changing this value.  See how pixels are
	// altered at the following web page for tips:
	//   http://www.libsdl.org/intro.en/usingvideo.html
	for (double dy = 1; dy <= radius; dy += 1.0) {
		double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));

		SDL_RenderDrawLine(RENDERER(), x - dx, y + dy - radius, x + dx, y + dy - radius);
		SDL_RenderDrawLine(RENDERER(), x - dx, y - dy + radius, x + dx, y - dy + radius);
	}
}

void
painter_clear(void)
{
	SDL_RenderClear(RENDERER());
}

void
painter_present(void)
{
	SDL_RenderPresent(RENDERER());
}
