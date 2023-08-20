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

#include <mlk/core/font.h>

#include "align.h"
#include "label.h"
#include "ui.h"
#include "ui_p.h"

static int
query(struct mlk_label_style *self,
      struct mlk_label *label,
      unsigned int *w,
      unsigned int *h)
{
	return mlk_font_query(MLK__STYLE_FONT(self->font, MLK_UI_FONT_INTERFACE), label->text, w, h);
}

static void
draw(struct mlk_label_style *self, struct mlk_label *label)
{
	mlk_ui_draw_text(
		MLK_ALIGN_NONE,
		MLK__STYLE_FONT(self->font, MLK_UI_FONT_INTERFACE),
		self->color,
		label->text,
		label->x,
		label->y,
		0,
		0
	);
}

struct mlk_label_style mlk_label_style_dark = {
	.color  = 0xf5f7faff,
	.font   = &mlk_ui_fonts[MLK_UI_FONT_INTERFACE],
	.query  = query,
	.draw   = draw
};

struct mlk_label_style mlk_label_style_light = {
	.color  = 0x222323ff,
	.font   = &mlk_ui_fonts[MLK_UI_FONT_INTERFACE],
	.query  = query,
	.draw   = draw
};

struct mlk_label_style *mlk_label_style = &mlk_label_style_light;

int
mlk_label_query(struct mlk_label *label, unsigned int *w, unsigned int *h)
{
	assert(label);

	return MLK__STYLE_CALL(label->style, mlk_label_style, query, label, w, h);
}

void
mlk_label_update(struct mlk_label *label, unsigned int ticks)
{
	assert(label);

	MLK__STYLE_CALL(label->style, mlk_label_style, update, label, ticks);
}

void
mlk_label_draw(struct mlk_label *label)
{
	assert(label);

	MLK__STYLE_CALL(label->style, mlk_label_style, draw, label);
}
