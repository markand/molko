/*
 * label.c -- GUI label
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
#include <string.h>

#include <mlk/core/font.h>
#include <mlk/core/panic.h>
#include <mlk/core/texture.h>

#include "label.h"
#include "theme.h"

void
mlk_label_draw_default(const struct mlk_theme *t, const struct mlk_label *label)
{
	assert(t);
	assert(label);

	struct mlk_font *font;
	struct mlk_texture tex;
	unsigned long color;
	int err;

	font = label->flags & MLK_LABEL_FLAGS_IMPORTANT
		? t->fonts[MLK_THEME_FONT_IMPORTANT]
		: t->fonts[MLK_THEME_FONT_INTERFACE];
	color = label->flags & MLK_LABEL_FLAGS_SELECTED
		? t->colors[MLK_THEME_COLOR_SELECTED]
	        : t->colors[MLK_THEME_COLOR_NORMAL];

	/* Shadow text, only if enabled. */
	if (label->flags & MLK_LABEL_FLAGS_SHADOW) {
		if ((err = mlk_font_render(font, &tex, label->text, t->colors[MLK_THEME_COLOR_SHADOW])) < 0)
			mlk_panic(err);

		mlk_texture_draw(&tex, label->x + 1, label->y + 1);
		mlk_texture_finish(&tex);
	}

	/* Normal text. */
	if ((err = mlk_font_render(font, &tex, label->text, color)) < 0)
		mlk_panic(err);

	mlk_texture_draw(&tex, label->x, label->y);
	mlk_texture_finish(&tex);
}

int
mlk_label_ok(const struct mlk_label *label)
{
	return label && label->text && strlen(label->text) > 0;
}

void
mlk_label_query(const struct mlk_label *label, unsigned int *w, unsigned int *h)
{
	assert(label);
	assert(label->text);

	const struct mlk_theme *t = label->theme ? label->theme : mlk_theme_default();
	const struct mlk_font *f = label->flags & MLK_LABEL_FLAGS_IMPORTANT
		? t->fonts[MLK_THEME_FONT_IMPORTANT]
		: t->fonts[MLK_THEME_FONT_INTERFACE];
	int err;

	if ((err = mlk_font_query(f, label->text, w, h)) < 0)
		mlk_panic(err);
}

void
mlk_label_draw(const struct mlk_label *label)
{
	assert(label);
	assert(label->text);

	mlk_theme_draw_label(label->theme, label);
}
