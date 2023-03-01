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
#include "ui.h"

#define STYLE_INVOKE(s, f, ...)                                                 \
do {                                                                            \
        if (s && s->f)                                                          \
                s->f(s, __VA_ARGS__);                                           \
        else if (mlk_label_style.f)                                             \
                mlk_label_style.f(s ? s : &mlk_label_style, __VA_ARGS__);       \
} while (0)

static inline struct mlk_font *
style_font(struct mlk_label_style *style)
{
	if (style && style->text_font)
		return style->text_font;

	return mlk_ui_fonts[MLK_UI_FONT_INTERFACE];
}

static void
draw(struct mlk_label_style *style, const struct mlk_label *label)
{
	struct mlk_font *font;
	struct mlk_texture tex;
	int err;

	font = style_font(style);

	if ((err = mlk_font_render(font, &tex, label->text, style->text_color)) < 0)
		mlk_panic(err);

	mlk_texture_draw(&tex, label->x, label->y);
	mlk_texture_finish(&tex);
}

struct mlk_label_style mlk_label_style = {
	.text_color     = 0x000000ff,
	.draw           = draw
};

struct mlk_label_style mlk_label_style_selected = {
	.text_color     = 0x7da42dff,
	.draw           = draw
};

void
mlk_label_init(struct mlk_label *label)
{
	assert(mlk_label_ok(label));

	STYLE_INVOKE(label->style, init, label);
}

int
mlk_label_ok(const struct mlk_label *label)
{
	return label && label->text && strlen(label->text) > 0;
}

int
mlk_label_query(const struct mlk_label *label, unsigned int *w, unsigned int *h)
{
	assert(mlk_label_ok(label));

	struct mlk_font *font;
	int err;

	font = style_font(label->style);

	if ((err = mlk_font_query(font, label->text, w, h)) < 0)
		return err;

	return err;
}

void
mlk_label_draw(const struct mlk_label *label)
{
	assert(mlk_label_ok(label));

	STYLE_INVOKE(label->style, draw, label);
}

void
mlk_label_finish(struct mlk_label *label)
{
	assert(mlk_label_ok(label));

	STYLE_INVOKE(label->style, finish, label);
}
