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

struct mlk_theme;

union mlk_event;

struct mlk_gridmenu {
	/* public */
	int x;
	int y;
	unsigned int w;
	unsigned int h;
	size_t selected;
	const struct mlk_theme *theme;
	const char * const *items;
	size_t itemsz;
	unsigned int nrows;
	unsigned int ncols;

	/* private */
	unsigned int eltw;      /* maximum entry label width */
	unsigned int elth;      /* maximum entry label height */
	unsigned int spacew;    /* space between element horizontally */
	unsigned int spaceh;    /* and vertically */
};

MLK_CORE_BEGIN_DECLS

void
mlk_gridmenu_init(struct mlk_gridmenu *, unsigned int, unsigned int, const char * const *, size_t);

void
mlk_gridmenu_resize(struct mlk_gridmenu *, int, int, unsigned int, unsigned int);

int
mlk_gridmenu_handle(struct mlk_gridmenu *, const union mlk_event *);

void
mlk_gridmenu_draw(const struct mlk_gridmenu *);

MLK_CORE_END_DECLS

#endif /* !MLK_UI_GRIDMENU_H */
