/*
 * battle-indicator.c -- drawable for rendering a hp/mp count usage
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
#include <math.h>
#include <stdio.h>
#include <string.h>

#include <mlk/core/color.h>
#include <mlk/core/font.h>
#include <mlk/core/panic.h>

#include <mlk/ui/theme.h>

#include "battle-indicator.h"

#define THEME(bti)      ((bti)->theme ? (bti)->theme : theme_default())
#define STEP            (2)
#define DELAY           (5)

static inline unsigned int
inc(int cmp, int tgt)
{
	return tgt > cmp ? fmin(cmp + STEP, tgt) : fmax(cmp - STEP, tgt);
}

static inline int
colored(const struct battle_indicator *bti)
{
	/* Only check r, g, b and ignore alpha. */
	return COLOR_R(bti->cur) == COLOR_R(bti->color) &&
	       COLOR_G(bti->cur) == COLOR_G(bti->color) &&
	       COLOR_B(bti->cur) == COLOR_B(bti->color);
}

void
battle_indicator_start(struct battle_indicator *bti)
{
	assert(bti);

	char buf[128];
	const struct theme *theme = THEME(bti);

	snprintf(buf, sizeof (buf), "%u", bti->amount);

	bti->cur = 0xffffffff;
	bti->elapsed = 0;
	bti->alpha = 250;

	if (mlk_font_render(theme->fonts[THEME_FONT_INTERFACE], &bti->tex[0], buf, bti->cur) < 0||
	    mlk_font_render(theme->fonts[THEME_FONT_INTERFACE], &bti->tex[1], buf, 0x000000ff) < 0)
		panic();
}

int
battle_indicator_completed(const struct battle_indicator *bti)
{
	assert(battle_indicator_ok(bti));

	return colored(bti) && bti->alpha == 0;
}

int
battle_indicator_ok(const struct battle_indicator *bti)
{
	return bti && texture_ok(&bti->tex[0]) && texture_ok(&bti->tex[1]);
}

int
battle_indicator_update(struct battle_indicator *bti, unsigned int ticks)
{
	assert(battle_indicator_ok(bti));

	bti->elapsed += ticks;

	if (bti->elapsed > DELAY) {
		bti->elapsed = 0;

		if (!colored(bti)) {
			/* Update colors first. */
			bti->cur = COLOR_HEX(
			    inc(COLOR_R(bti->cur), COLOR_R(bti->color)),
			    inc(COLOR_G(bti->cur), COLOR_G(bti->color)),
			    inc(COLOR_B(bti->cur), COLOR_B(bti->color)),
			    255
			);

			texture_set_color_mod(&bti->tex[0], bti->cur);
		} else {
			/* Update alpha next. */
			bti->alpha -= 10;

			texture_set_alpha_mod(&bti->tex[0], bti->alpha);
			texture_set_alpha_mod(&bti->tex[1], bti->alpha);
		}
	}

	return battle_indicator_completed(bti);
}

void
battle_indicator_draw(const struct battle_indicator *bti, int x, int y)
{
	assert(battle_indicator_ok(bti));

	texture_draw(&bti->tex[1], x + 1, y + 1);
	texture_draw(&bti->tex[0], x, y);
}

void
battle_indicator_finish(struct battle_indicator *bti)
{
	assert(bti);

	texture_finish(&bti->tex[0]);
	texture_finish(&bti->tex[1]);

	memset(bti, 0, sizeof (*bti));
}
