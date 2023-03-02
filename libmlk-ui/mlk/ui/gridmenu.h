/*
 * gridmenu.h -- GUI grid menu
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

#ifndef MLK_UI_GRIDMENU_H
#define MLK_UI_GRIDMENU_H

#include <stddef.h>

#include <mlk/core/core.h>

#include "label.h"

union mlk_event;

struct mlk_font;
struct mlk_gridmenu;

struct mlk_gridmenu_style {
	void *data;
	unsigned int padding;
	unsigned long text_color;
	unsigned long text_selected_color;
	struct mlk_font *text_font;
	void (*init)(struct mlk_gridmenu_style *, struct mlk_gridmenu *);
	void (*update)(struct mlk_gridmenu_style *, struct mlk_gridmenu *, unsigned int);
	void (*draw)(struct mlk_gridmenu_style *, const struct mlk_gridmenu *);
	void (*finish)(struct mlk_gridmenu_style *, struct mlk_gridmenu *);
};

struct mlk_gridmenu {
	/* public */
	int x, y;
	unsigned int w, h;
	const char * const *items;
	size_t itemsz;
	size_t selected;
	unsigned int nrows;
	unsigned int ncols;
	struct mlk_gridmenu_style *style;

	/* private */
	unsigned int eltw;      /* maximum entry label width */
	unsigned int elth;      /* maximum entry label height */
	unsigned int spacew;    /* space between element horizontally */
	unsigned int spaceh;    /* and vertically */
};

extern struct mlk_gridmenu_style mlk_gridmenu_style;

MLK_CORE_BEGIN_DECLS

void
mlk_gridmenu_init(struct mlk_gridmenu *);

int
mlk_gridmenu_ok(const struct mlk_gridmenu *);

void
mlk_gridmenu_resize(struct mlk_gridmenu *, int, int, unsigned int, unsigned int);

int
mlk_gridmenu_handle(struct mlk_gridmenu *, const union mlk_event *);

void
mlk_gridmenu_update(struct mlk_gridmenu *, unsigned int);

void
mlk_gridmenu_draw(const struct mlk_gridmenu *);

void
mlk_gridmenu_finish(struct mlk_gridmenu *);

MLK_CORE_END_DECLS

#endif /* !MLK_UI_GRIDMENU_H */
