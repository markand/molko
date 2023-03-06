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

#include <mlk/core/err.h>
#include <mlk/core/font.h>
#include <mlk/core/texture.h>
#include <mlk/core/trace.h>

#include "align.h"
#include "label.h"
#include "ui.h"
#include "ui_p.h"

static inline struct mlk_font *
style_font(const struct mlk_label *label)
{
	const struct mlk_label_style *style = MLK__STYLE(label, mlk_label_style);

	if (style->font)
		return style->font;

	return mlk_ui_fonts[MLK_UI_FONT_INTERFACE];
}

static int
delegate_query(struct mlk_label_delegate *delegate, const struct mlk_label *label, unsigned int *w, unsigned *h)
{
	(void)delegate;

	struct mlk_font *font;
	int err;

	font = style_font(label);

	if ((err = mlk_font_query(font, label->text, w, h)) < 0)
		return err;

	return 0;
}

static void
delegate_draw(struct mlk_label_delegate *delegate, const struct mlk_label *label)
{
	(void)delegate;

	const struct mlk_label_style *style = MLK__STYLE(label, mlk_label_style);

	mlk_ui_draw_text(
		MLK_ALIGN_NONE,
		style_font(label),
		style->color,
		label->text,
		label->x,
		label->y,
		0,
		0
	);
}

struct mlk_label_style mlk_label_style = {
	.color  = MLK_UI_COLOR_TEXT
};

struct mlk_label_delegate mlk_label_delegate = {
	.query  = delegate_query,
	.draw   = delegate_draw
};

int
mlk_label_ok(const struct mlk_label *label)
{
	return label && label->text && strlen(label->text) > 0;
}

int
mlk_label_query(const struct mlk_label *label, unsigned int *w, unsigned int *h)
{
	assert(mlk_label_ok(label));

	MLK__DELEGATE_INVOKE_RET(label->delegate, mlk_label_delegate, query, label, w, h);

	if (w)
		*w = 0;
	if (h)
		*h = 0;

	return 0;
}

void
mlk_label_update(struct mlk_label *label, unsigned int ticks)
{
	assert(mlk_label_ok(label));

	MLK__DELEGATE_INVOKE(label->delegate, mlk_label_delegate, update, label, ticks);
}

void
mlk_label_draw(const struct mlk_label *label)
{
	assert(mlk_label_ok(label));

	MLK__DELEGATE_INVOKE(label->delegate, mlk_label_delegate, draw, label);
}
