/*
 * gridmenu.c -- GUI grid menu
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
#include <math.h>
#include <string.h>

#include <mlk/core/event.h>
#include <mlk/core/maths.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/texture.h>
#include <mlk/core/trace.h>

#include "frame.h"
#include "label.h"
#include "gridmenu.h"
#include "theme.h"

#define THEME(m) ((m)->theme ? (m)->theme : theme_default())

struct index {
	unsigned int row;
	unsigned int col;
};

static struct index
get_index(const struct mlk_gridmenu *menu)
{
	return (struct index) {
		.row = menu->selected / menu->ncols,
		.col = menu->selected % menu->ncols
	};
}

static void
geometry(struct mlk_gridmenu *menu)
{
	const struct theme *theme = THEME(menu);
	struct label label = {
		.theme = theme,
		.flags = LABEL_FLAGS_SHADOW
	};
	unsigned int reqw = 0, reqh = 0, lw, lh;

	/* Compute which item has the bigger width/height to create a spacing. */
	menu->eltw = menu->elth = 0;
	menu->spacew = menu->spaceh = 0;

	for (size_t i = 0; i < menu->itemsz; ++i) {
		if (!(label.text = menu->items[i]))
			continue;


		label_query(&label, &lw, &lh);

		menu->eltw = fmax(menu->eltw, lw);
		menu->elth = fmax(menu->elth, lh);
	}

	/* Total texture size required to draw items. */
	reqw = (theme->padding * 2) + (menu->eltw * menu->ncols);
	reqh = (theme->padding * 2) + (menu->elth * menu->nrows);

	/*
	 * Compute spacing between elements. We remove the padding because it
	 * is outside of the elements.
	 */
	if (reqw > menu->w) {
		mlk_tracef("gridmenu width is too small: %u < %u", menu->w, reqw);
		menu->spacew = 1;
	} else if (menu->ncols > 1) {
		reqw -= theme->padding * 2;
		menu->spacew = (menu->w - reqw) / menu->ncols;
	}

	if (reqh > menu->h) {
		mlk_tracef("gridmenu height is too small: %u < %u", menu->h, reqh);
		menu->spaceh = 1;
	} else if (menu->nrows > 1) {
		reqh -= theme->padding * 2;
		menu->spaceh = (menu->h - reqh) / menu->nrows;
	}
}

static void
draw_frame(const struct mlk_gridmenu *menu)
{
	const struct mlk_frame f = {
		.x = menu->x,
		.y = menu->y,
		.w = menu->w,
		.h = menu->h,
		.theme = menu->theme,
	};

	mlk_frame_draw(&f);
}

static void
draw_labels(const struct mlk_gridmenu *menu)
{
	size_t pagesz, pagenr, item, c = 0, r = 0;
	struct label label = {0};
	const struct theme *theme = THEME(menu);

	label.theme = theme;
	label.flags = LABEL_FLAGS_SHADOW;

	/*
	 * Select the first top-left column based on the current selection and
	 * the number of rows/columns.
	 */
	pagesz = menu->nrows * menu->ncols;
	pagenr = menu->selected / pagesz;

	for (size_t i = 0; i < pagesz; ++i) {
		item = i + pagenr * pagesz;

		if (item >= menu->itemsz || !menu->items[item])
			continue;

		label.text = menu->items[item];
		label.x = menu->x + theme->padding + (c * menu->eltw) + (c * menu->spacew);
		label.y = menu->y + theme->padding + (r * menu->elth) + (r * menu->spaceh);

		if (i == menu->selected % pagesz)
			label.flags |= LABEL_FLAGS_SELECTED;
		else
			label.flags &= ~(LABEL_FLAGS_SELECTED);

		label_draw(&label);

		if (++c >= menu->ncols) {
			++r;
			c = 0;
		}
	}
}

static int
handle_keydown(struct mlk_gridmenu *menu, const struct mlk_event_key *key)
{
	assert(key->type == MLK_EVENT_KEYDOWN);

	const struct index idx = get_index(menu);
	int validate = 0;

	switch (key->key) {
	case MLK_KEY_UP:
		if (idx.row > 0)
			menu->selected -= menu->ncols;
		break;
	case MLK_KEY_RIGHT:
		if (menu->selected + 1U < menu->itemsz)
			menu->selected += 1;
		break;
	case MLK_KEY_DOWN:
		if (idx.row + 1U < menu->itemsz / menu->ncols)
			menu->selected += menu->ncols;
		else
			menu->selected = menu->itemsz - 1;
		break;
	case MLK_KEY_LEFT:
		if (idx.col > 0)
			menu->selected -= 1;
		break;
	case MLK_KEY_ENTER:
		validate = 1;
		break;
	default:
		break;
	}

	return validate;
}

static int
handle_clickdown(struct mlk_gridmenu *menu, const struct mlk_event_click *click)
{
	assert(click->type == MLK_EVENT_CLICKDOWN);

	const struct theme *theme = THEME(menu);
	size_t pagesz, pagenr, selected, c = 0, r = 0;
	int x, y;

	pagesz = menu->nrows * menu->ncols;
	pagenr = menu->selected / pagesz;

	for (size_t i = 0; i < pagesz; ++i) {
		x = menu->x + theme->padding + (c * menu->eltw) + (c * menu->spacew);
		y = menu->y + theme->padding + (r * menu->elth) + (r * menu->spaceh);

		if (mlk_maths_is_boxed(x, y, menu->eltw, menu->elth, click->x, click->y)) {
			selected  = c + r * menu->ncols;
			selected += pagesz * pagenr;

			if (selected < menu->itemsz) {
				menu->selected = selected;
				return 1;
			}
		}

		if (++c >= menu->ncols) {
			++r;
			c = 0;
		}
	}

	return 0;
}

void
mlk_gridmenu_init(struct mlk_gridmenu *menu,
              unsigned int nr,
              unsigned int nc,
              const char * const *items,
              size_t itemsz)
{
	assert(menu);
	assert(nr);
	assert(nc);

	memset(menu, 0, sizeof (*menu));

	menu->nrows = nr;
	menu->ncols = nc;
	menu->items = items;
	menu->itemsz = itemsz;
}

void
mlk_gridmenu_resize(struct mlk_gridmenu *menu, int x, int y, unsigned int w, unsigned int h)
{
	assert(menu);

	menu->x = x;
	menu->y = y;
	menu->w = w;
	menu->h = h;

	geometry(menu);
}

int
mlk_gridmenu_handle(struct mlk_gridmenu *menu, const union mlk_event *ev)
{
	assert(menu);
	assert(ev);

	switch (ev->type) {
	case MLK_EVENT_KEYDOWN:
		return handle_keydown(menu, &ev->key);
		break;
	case MLK_EVENT_CLICKDOWN:
		return handle_clickdown(menu, &ev->click);
		break;
	default:
		break;
	}

	return 0;
}

void
mlk_gridmenu_draw(const struct mlk_gridmenu *menu)
{
	assert(menu);

	draw_frame(menu);
	draw_labels(menu);
}
