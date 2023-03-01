/*
 * button-style-glow.c -- example of glowing button
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

#include "button-style-glow.h"

/*
 * This button style illuminates the background color by periodically changing
 * its red, green, blue components to the target color and then go back to the
 * original transition.
 *
 * Its sole purpose is to demonstrates how style works. Because the style needs
 * a state it is implemented through a parent button_style_glow structure which
 * holds an underlying style.
 *
 * This style does not override the glow function because we just change the
 * bg_color property that will be reused.
 */

static inline unsigned int
increment(unsigned long ccmp, unsigned long ctgt)
{
	if (ctgt > ccmp)
		return ccmp + 2 > ctgt ? ctgt : ccmp + 2;
	if (ctgt < ccmp)
		return ccmp - 2 < ctgt ? ctgt : ccmp - 2;

	return ccmp;
}

static void
init(struct mlk_button_style *style, struct mlk_button *button)
{
	(void)button;

	struct button_style_glow *glow = style->data;

	/*
	 * We start at colors[0] and reach colors[1], colors[2] will be the
	 * destination color that will swap over time.
	 */
	glow->colors[2] = glow->colors[1];
	glow->elapsed = 0;

	style->bg_color = glow->colors[0];
}

static void
update(struct mlk_button_style *style, struct mlk_button *button, unsigned int ticks)
{
	(void)button;

	struct button_style_glow *glow = style->data;
	unsigned int red, green, blue;

	glow->elapsed += ticks;

	if (glow->elapsed >= glow->delay) {
		glow->elapsed = 0;

		/* Color target reached, invert logic. */
		if (style->bg_color == glow->colors[2]) {
			if (glow->colors[2] == glow->colors[0])
				glow->colors[2] = glow->colors[1];
			else
				glow->colors[2] = glow->colors[0];
		} else {
			red = increment(
				MLK_COLOR_R(style->bg_color),
				MLK_COLOR_R(glow->colors[2])
			);
			green = increment(
				MLK_COLOR_G(style->bg_color),
				MLK_COLOR_G(glow->colors[2])
			);
			blue = increment(
				MLK_COLOR_B(style->bg_color),
				MLK_COLOR_B(glow->colors[2])
			);

			style->bg_color = MLK_COLOR_HEX(red, green, blue, 0xff);
		}
	}
}

void
button_style_glow_init(struct button_style_glow *glow)
{
	assert(glow);

	glow->style.data = glow;
	glow->style.init = init;
	glow->style.update = update;
}
