/*
 * glower.c -- simple color animation
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

#include <assert.h>

#include <mlk/core/color.h>

#include "glower.h"

static inline unsigned int
increment(unsigned long ccmp, unsigned long ctgt)
{
	if (ctgt > ccmp)
		return (unsigned int)(ccmp + 2 > ctgt ? ctgt : ccmp + 2);
	if (ctgt < ccmp)
		return (unsigned int)(ccmp - 2 < ctgt ? ctgt : ccmp - 2);

	return (unsigned int)ccmp;
}

void
mlk_glower_init(struct mlk_glower *glow)
{
	assert(glow);

	glow->color = glow->start;
	glow->target = glow->end;
}

void
mlk_glower_update(struct mlk_glower *glow, unsigned int ticks)
{
	assert(glow);

	unsigned int r, g, b;

	glow->elapsed += ticks;

	if (glow->elapsed >= glow->delay) {
		glow->elapsed = 0;

		/* Color target reached, invert target color. */
		if (glow->color == glow->target)
			glow->target = glow->target == glow->start ? glow->end : glow->start;
		else {
			r = increment(MLK_COLOR_R(glow->color), MLK_COLOR_R(glow->target));
			g = increment(MLK_COLOR_G(glow->color), MLK_COLOR_G(glow->target));
			b = increment(MLK_COLOR_B(glow->color), MLK_COLOR_B(glow->target));

			glow->color = MLK_COLOR_HEX(r, g, b, 0xff);
		}
	}
}
