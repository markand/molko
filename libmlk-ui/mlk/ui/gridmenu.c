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

#include <mlk/core/err.h>
#include <mlk/core/event.h>
#include <mlk/core/font.h>
#include <mlk/core/maths.h>
#include <mlk/core/painter.h>
#include <mlk/core/texture.h>
#include <mlk/core/trace.h>
#include <mlk/core/window.h>

#include "gridmenu.h"
#include "style.h"
#include "ui_p.h"

struct index {
	unsigned int row;
	unsigned int col;
};

static struct index
get_index(const struct mlk_gridmenu *menu)
{
	return (struct index) {
		.row = (unsigned int)(menu->selected / menu->ncols),
		.col = (unsigned int)(menu->selected % menu->ncols)
	};
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

	const struct mlk_style_attr *attr = &menu->style->normal;
	size_t pagesz, pagenr, selected, c = 0, r = 0;
	int x, y;

	pagesz = menu->nrows * menu->ncols;
	pagenr = menu->selected / pagesz;

	for (size_t i = 0; i < pagesz; ++i) {
		x = (int)(menu->x + attr->geo.padding + (c * menu->eltw) + (c * menu->spacew));
		y = (int)(menu->y + attr->geo.padding + (r * menu->elth) + (r * menu->spaceh));

		if (mlk_maths_is_boxed(click->x, click->y, x, y, menu->eltw, menu->elth)) {
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

static void
resize(struct mlk_gridmenu_delegate *self, struct mlk_gridmenu *menu)
{
	(void)self;

	const struct mlk_style_attr *attr = &menu->style->normal;
	unsigned int reqw = 0, reqh = 0, lw, lh;

	/* Compute which item has the bigger width/height to create a spacing. */
	menu->eltw = menu->elth = 0;
	menu->spacew = menu->spaceh = 0;

	for (size_t i = 0; i < menu->itemsz; ++i) {
		if (!(menu->items[i]))
			continue;

		mlk_font_query(attr->font, menu->items[i], &lw, &lh);

		menu->eltw = fmax(menu->eltw, lw);
		menu->elth = fmax(menu->elth, lh);
	}

	/* Total texture size required to draw items. */
	reqw = (attr->geo.padding * 3) + (menu->eltw * menu->ncols);
	reqh = (attr->geo.padding * 3) + (menu->elth * menu->nrows);

	/*
	 * Compute spacing between elements. We remove the padding because it
	 * is outside of the elements.
	 */
	if (reqw > menu->w) {
		mlk_tracef(_("gridmenu width is too small: %u < %u"), menu->w, reqw);
		menu->spacew = 1;
	} else if (menu->ncols > 1) {
		reqw -= attr->geo.padding * 2;
		menu->spacew = (menu->w - reqw) / menu->ncols;
	}

	if (reqh > menu->h) {
		mlk_tracef(_("gridmenu height is too small: %u < %u"), menu->h, reqh);
		menu->spaceh = 1;
	} else if (menu->nrows > 1) {
		reqh -= attr->geo.padding * 2;
		menu->spaceh = (menu->h - reqh) / menu->nrows;
	}
}

static int
handle(struct mlk_gridmenu_delegate *self,
       struct mlk_gridmenu *menu,
       const union mlk_event *ev)
{
	(void)self;

	switch (ev->type) {
	case MLK_EVENT_KEYDOWN:
		return handle_keydown(menu, &ev->key);
	case MLK_EVENT_CLICKDOWN:
		return handle_clickdown(menu, &ev->click);
	default:
		break;
	}

	return 0;
}

static void
draw(struct mlk_gridmenu_delegate *self, const struct mlk_gridmenu *menu)
{
	(void)self;

	const struct mlk_style *st = menu->style;
	size_t pagesz, pagenr, item, c = 0, r = 0;
	struct mlk_texture tex;
	struct mlk_font *font;
	unsigned long color;
	int x, y;

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

		x = (int)(menu->x + st->normal.geo.padding + (c * menu->eltw) + (c * menu->spacew));
		y = (int)(menu->y + st->normal.geo.padding + (r * menu->elth) + (r * menu->spaceh));

		if (i == menu->selected % pagesz) {
			color = st->selected.color.text;
			font = st->selected.font;
		} else {
			color = st->normal.color.text;
			font = st->normal.font;
		}

		if (mlk_font_render(font, &tex, menu->items[item], color) < 0) {
			mlk_tracef(_("unable to render grid menu item: %s"), mlk_err());
			continue;
		}

		mlk_texture_draw(&tex, x, y);
		mlk_texture_finish(&tex);

		if (++c >= menu->ncols) {
			++r;
			c = 0;
		}
	}
}

struct mlk_gridmenu_delegate mlk_gridmenu_delegate = {
	.resize = resize,
	.handle = handle,
	.draw = draw
};

void
mlk_gridmenu_init(struct mlk_gridmenu *menu,
                  struct mlk_gridmenu_delegate *dt,
                  struct mlk_style *st)
{
	assert(menu);

	menu->x = 0;
	menu->y = 0;
	menu->w = 0;
	menu->h = 0;
	menu->items = NULL;
	menu->itemsz = 0;
	menu->nrows = 0;
	menu->ncols = 0;
	menu->delegate = dt ? dt : &mlk_gridmenu_delegate;
	menu->style = st ? st : &mlk_style;

	/* Can't be computed yet. */
	menu->eltw = 0;
	menu->elth = 0;
	menu->spacew = 0;
	menu->spaceh = 0;
}

void
mlk_gridmenu_resize(struct mlk_gridmenu *menu,
                    int x,
                    int y,
                    unsigned int w,
                    unsigned int h)
{
	assert(menu);

	menu->x = x;
	menu->y = y;
	menu->w = w;
	menu->h = h;

	if (menu->delegate->resize)
		menu->delegate->resize(menu->delegate, menu);
}

int
mlk_gridmenu_handle(struct mlk_gridmenu *menu, const union mlk_event *ev)
{
	assert(menu);
	assert(ev);

	if (menu->delegate->handle)
		return menu->delegate->handle(menu->delegate, menu, ev);

	return 0;
}

void
mlk_gridmenu_update(struct mlk_gridmenu *menu, unsigned int ticks)
{
	assert(menu);

	if (menu->delegate->update)
		menu->delegate->update(menu->delegate, menu, ticks);
}

void
mlk_gridmenu_draw(const struct mlk_gridmenu *menu)
{
	assert(menu);

	if (menu->delegate->draw)
		menu->delegate->draw(menu->delegate, menu);
}
