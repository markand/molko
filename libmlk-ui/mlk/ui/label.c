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
query(struct mlk_label_if *self,
      const struct mlk_label *label,
      unsigned int *w,
      unsigned int *h)
{
	(void)self;

	return mlk_font_query(label->style->normal.font, label->text, w, h);
}

static void
draw(struct mlk_label_if *self, const struct mlk_label *label)
{
	(void)self;

	const struct mlk_style_attr *attr;

	if (label->selected)
		attr = &label->style->selected;
	else
		attr = &label->style->normal;

	mlk_ui_draw_text(
		MLK_ALIGN_NONE,
		attr->font,
		attr->color.text,
		label->text,
		label->x,
		label->y,
		0,
		0
	);
}

struct mlk_label_if mlk_label_if = {
	.query = query,
	.draw = draw
};

void
mlk_label_init(struct mlk_label *lbl,
               struct mlk_label_if *iface,
               struct mlk_style *st)
{
	assert(lbl);

	lbl->x = 0;
	lbl->y = 0;
	lbl->text = "";
	lbl->iface = iface ? iface : &mlk_label_if;
	lbl->style = st ? st : &mlk_style;
}

int
mlk_label_query(const struct mlk_label *lbl, unsigned int *w, unsigned int *h)
{
	assert(lbl);

	if (lbl->iface->query)
		return lbl->iface->query(lbl->iface, lbl, w, h);

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

	if (lbl->iface->update)
		lbl->iface->update(lbl->iface, lbl, ticks);
}

void
mlk_label_draw(const struct mlk_label *lbl)
{
	assert(lbl);

	if (lbl->iface->draw)
		lbl->iface->draw(lbl->iface, lbl);
}

void
mlk_label_finish(struct mlk_label *lbl)
{
	assert(lbl);

	if (lbl->iface->finish)
		lbl->iface->finish(lbl->iface, lbl);
}
