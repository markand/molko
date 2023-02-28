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
label_draw_default(const struct theme *t, const struct label *label)
{
	assert(t);
	assert(label);

	struct mlk_font *font;
	struct mlk_texture tex;
	unsigned long color;
	int err;

	font = label->flags & LABEL_FLAGS_IMPORTANT
		? t->fonts[THEME_FONT_IMPORTANT]
		: t->fonts[THEME_FONT_INTERFACE];
	color = label->flags & LABEL_FLAGS_SELECTED
		? t->colors[THEME_COLOR_SELECTED]
	        : t->colors[THEME_COLOR_NORMAL];

	/* Shadow text, only if enabled. */
	if (label->flags & LABEL_FLAGS_SHADOW) {
		if ((err = mlk_font_render(font, &tex, label->text, t->colors[THEME_COLOR_SHADOW])) < 0)
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
label_ok(const struct label *label)
{
	return label && label->text && strlen(label->text) > 0;
}

void
label_query(const struct label *label, unsigned int *w, unsigned int *h)
{
	assert(label);
	assert(label->text);

	const struct theme *t = label->theme ? label->theme : theme_default();
	const struct mlk_font *f = label->flags & LABEL_FLAGS_IMPORTANT
		? t->fonts[THEME_FONT_IMPORTANT]
		: t->fonts[THEME_FONT_INTERFACE];
	int err;

	if ((err = mlk_font_query(f, label->text, w, h)) < 0)
		mlk_panic(err);
}

void
label_draw(const struct label *label)
{
	assert(label);
	assert(label->text);

	theme_draw_label(label->theme, label);
}
