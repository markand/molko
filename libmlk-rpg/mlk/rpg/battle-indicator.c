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

#define THEME(bti)      ((bti)->theme ? (bti)->theme : &mlk_theme)
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
	return MLK_COLOR_R(bti->cur) == MLK_COLOR_R(bti->color) &&
	       MLK_COLOR_G(bti->cur) == MLK_COLOR_G(bti->color) &&
	       MLK_COLOR_B(bti->cur) == MLK_COLOR_B(bti->color);
}

void
battle_indicator_start(struct battle_indicator *bti)
{
	assert(bti);

	char buf[128];
	const struct mlk_theme *theme = THEME(bti);
	int err;

	snprintf(buf, sizeof (buf), "%u", bti->amount);

	bti->cur = 0xffffffff;
	bti->elapsed = 0;
	bti->alpha = 250;

	if ((err = mlk_font_render(theme->fonts[MLK_THEME_FONT_INTERFACE], &bti->tex[0], buf, bti->cur)) < 0 ||
	    (err = mlk_font_render(theme->fonts[MLK_THEME_FONT_INTERFACE], &bti->tex[1], buf, 0x000000ff)) < 0)
		mlk_panic(err);
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
	return bti && mlk_texture_ok(&bti->tex[0]) && mlk_texture_ok(&bti->tex[1]);
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
			bti->cur = MLK_COLOR_HEX(
			    inc(MLK_COLOR_R(bti->cur), MLK_COLOR_R(bti->color)),
			    inc(MLK_COLOR_G(bti->cur), MLK_COLOR_G(bti->color)),
			    inc(MLK_COLOR_B(bti->cur), MLK_COLOR_B(bti->color)),
			    255
			);

			mlk_texture_set_color_mod(&bti->tex[0], bti->cur);
		} else {
			/* Update alpha next. */
			bti->alpha -= 10;

			mlk_texture_set_alpha_mod(&bti->tex[0], bti->alpha);
			mlk_texture_set_alpha_mod(&bti->tex[1], bti->alpha);
		}
	}

	return battle_indicator_completed(bti);
}

void
battle_indicator_draw(const struct battle_indicator *bti, int x, int y)
{
	assert(battle_indicator_ok(bti));

	mlk_texture_draw(&bti->tex[1], x + 1, y + 1);
	mlk_texture_draw(&bti->tex[0], x, y);
}

void
battle_indicator_finish(struct battle_indicator *bti)
{
	assert(bti);

	mlk_texture_finish(&bti->tex[0]);
	mlk_texture_finish(&bti->tex[1]);

	memset(bti, 0, sizeof (*bti));
}
