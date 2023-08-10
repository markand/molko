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
#include "style.h"
#include "ui.h"

static int
query(struct mlk_label_delegate *self,
      const struct mlk_label *label,
      unsigned int *w,
      unsigned int *h)
{
	(void)self;

	return mlk_font_query(label->style->text_font, label->text, w, h);
}

static void
draw(struct mlk_label_delegate *self, const struct mlk_label *label)
{
	(void)self;

	mlk_ui_draw_text(
		MLK_ALIGN_NONE,
		label->style->text_font,
		label->style->text_color,
		label->text,
		label->x,
		label->y,
		0,
		0
	);
}

struct mlk_label_delegate mlk_label_delegate = {
	.query = query,
	.draw = draw
};

void
mlk_label_init(struct mlk_label *lbl,
               struct mlk_style *st,
               struct mlk_label_delegate *dt)
{
	assert(lbl);

	lbl->x = 0;
	lbl->y = 0;
	lbl->text = "";
	lbl->style = st ? st : &mlk_style;
	lbl->delegate = dt ? dt : &mlk_label_delegate;
}

int
mlk_label_query(const struct mlk_label *lbl, unsigned int *w, unsigned int *h)
{
	assert(lbl);

	if (lbl->delegate->query)
		return lbl->delegate->query(lbl->delegate, lbl, w, h);

	if (w)
		*w = 0;
	if (h)
		*h = 0;

	return 0;
}

void
mlk_label_update(struct mlk_label *lbl, unsigned int ticks)
{
	assert(lbl);

	if (lbl->delegate->update)
		lbl->delegate->update(lbl->delegate, lbl, ticks);
}

void
mlk_label_draw(const struct mlk_label *lbl)
{
	assert(lbl);

	if (lbl->delegate->draw)
		lbl->delegate->draw(lbl->delegate, lbl);
}

void
mlk_label_finish(struct mlk_label *lbl)
{
	assert(lbl);

	if (lbl->delegate->finish)
		lbl->delegate->finish(lbl->delegate, lbl);
}
