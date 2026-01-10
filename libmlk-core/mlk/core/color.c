/*
 * color.c -- basic color routines
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

#include "color.h"
#include "util.h"

unsigned long
mlk_color_darken(unsigned long color, float perc)
{
	unsigned int r = MLK_COLOR_R(color) * perc;
	unsigned int g = MLK_COLOR_G(color) * perc;
	unsigned int b = MLK_COLOR_B(color) * perc;
	unsigned int a = MLK_COLOR_A(color) * perc;

	return MLK_COLOR_HEX(
		mlk_clampu(r, 0, 255),
		mlk_clampu(g, 0, 255),
		mlk_clampu(b, 0, 255),
		mlk_clampu(a, 0, 255)
	);
	
}
