/*
 * gridmenu.h -- GUI grid menu
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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

#include <core/core.h>

#include "label.h"

struct theme;

union event;

struct gridmenu {
	int x;
	int y;
	unsigned int w;
	unsigned int h;
	size_t selected;
	const struct theme *theme;
	const char * const *items;
	size_t itemsz;
	unsigned int nrows;
	unsigned int ncols;

	/* Private fields. */
	unsigned int eltw;      /* maximum entry label width */
	unsigned int elth;      /* maximum entry label height */
	unsigned int spacew;    /* space between element horizontally */
	unsigned int spaceh;    /* and vertically */
};

CORE_BEGIN_DECLS

void
gridmenu_init(struct gridmenu *, unsigned int, unsigned int, const char * const *, size_t);

void
gridmenu_resize(struct gridmenu *, int, int, unsigned int, unsigned int);

int
gridmenu_handle(struct gridmenu *, const union event *);

void
gridmenu_draw(const struct gridmenu *);

CORE_END_DECLS

#endif /* !MLK_UI_GRIDMENU_H */
