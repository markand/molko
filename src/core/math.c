/*
 * math.c -- basic maths
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

#include "math.h"

bool
math_is_boxed(int x, int y, unsigned int w, unsigned int h, int px, int py)
{
	return px >= x &&
	       py >= y &&
	       px <= x + w &&
	       py <= y + h;
}

void
math_centerize(int *x,
               int *y,
               unsigned int w,
               unsigned int h,
               int px,
               int py,
               unsigned int pw,
               unsigned int ph)
{
	if (x)
		*x = px + (pw / 2) - (w / 2);
	if (y)
		*y = py + (ph / 2) - (h / 2);
}
