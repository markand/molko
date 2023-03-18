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

#include <mlk/core/err.h>
#include <mlk/core/event.h>
#include <mlk/core/font.h>
#include <mlk/core/maths.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/texture.h>
#include <mlk/core/trace.h>

#include "gridmenu.h"
#include "ui.h"
#include "ui_p.h"

struct index {
	unsigned int row;
	unsigned int col;
};

static inline struct mlk_font *
style_font(struct mlk_gridmenu_style *style)
{
	if (style && style->text_font)
		return style->text_font;

	return mlk_ui_fonts[MLK_UI_FONT_INTERFACE];
}

static struct index
get_index(const struct mlk_gridmenu *menu)
{
	return (struct index) {
		.row = (unsigned int)(menu->selected / menu->ncols),
		.col = (unsigned int)(menu->selected % menu->ncols)
	};
}

static void
geometry(struct mlk_gridmenu *menu)
{
	const struct mlk_gridmenu_style *style;
	unsigned int reqw = 0, reqh = 0, lw, lh;

	/* Compute which item has the bigger width/height to create a spacing. */
	menu->eltw = menu->elth = 0;
	menu->spacew = menu->spaceh = 0;

	style = MLK__STYLE(menu, mlk_gridmenu_style);

	for (size_t i = 0; i < menu->itemsz; ++i) {
		if (!(menu->items[i]))
			continue;

		mlk_font_query(style_font(menu->style), menu->items[i], &lw, &lh);

		menu->eltw = fmax(menu->eltw, lw);
		menu->elth = fmax(menu->elth, lh);
	}

	/* Total texture size required to draw items. */
	reqw = (style->padding * 3) + (menu->eltw * menu->ncols);
	reqh = (style->padding * 3) + (menu->elth * menu->nrows);

	/*
	 * Compute spacing between elements. We remove the padding because it
	 * is outside of the elements.
	 */
	if (reqw > menu->w) {
		mlk_tracef("gridmenu width is too small: %u < %u", menu->w, reqw);
		menu->spacew = 1;
	} else if (menu->ncols > 1) {
		reqw -= style->padding * 2;
		menu->spacew = (menu->w - reqw) / menu->ncols;
	}

	if (reqh > menu->h) {
		mlk_tracef("gridmenu height is too small: %u < %u", menu->h, reqh);
		menu->spaceh = 1;
	} else if (menu->nrows > 1) {
		reqh -= style->padding * 2;
		menu->spaceh = (menu->h - reqh) / menu->nrows;
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

	const struct mlk_gridmenu_style *style;
	size_t pagesz, pagenr, selected, c = 0, r = 0;
	int x, y;

	style  = MLK__STYLE(menu, mlk_gridmenu_style);
	pagesz = menu->nrows * menu->ncols;
	pagenr = menu->selected / pagesz;

	for (size_t i = 0; i < pagesz; ++i) {
		x = (int)(menu->x + style->padding + (c * menu->eltw) + (c * menu->spacew));
		y = (int)(menu->y + style->padding + (r * menu->elth) + (r * menu->spaceh));

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
draw(struct mlk_gridmenu_delegate *delegate, const struct mlk_gridmenu *menu)
{
	(void)delegate;

	size_t pagesz, pagenr, item, c = 0, r = 0;
	const struct mlk_gridmenu_style *style;
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

	style = MLK__STYLE(menu, mlk_gridmenu_style);
	font = style_font(menu->style);

	for (size_t i = 0; i < pagesz; ++i) {
		item = i + pagenr * pagesz;

		if (item >= menu->itemsz || !menu->items[item])
			continue;

		x = (int)(menu->x + style->padding + (c * menu->eltw) + (c * menu->spacew));
		y = (int)(menu->y + style->padding + (r * menu->elth) + (r * menu->spaceh));

		if (i == menu->selected % pagesz)
			color = style->selected_color;
		else
			color = style->text_color;

		if (mlk_font_render(font, &tex, menu->items[item], color) < 0) {
			mlk_tracef("unable to render grid menu item: %s", mlk_err());
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

struct mlk_gridmenu_style mlk_gridmenu_style = {
	.padding                = 10,
	.text_color             = MLK_UI_COLOR_TEXT,
	.selected_color         = MLK_UI_COLOR_SELECTED
};

struct mlk_gridmenu_delegate mlk_gridmenu_delegate = {
	.draw                   = draw
};

int
mlk_gridmenu_ok(const struct mlk_gridmenu *menu)
{
	return menu && menu->items && menu->itemsz;
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
mlk_gridmenu_update(struct mlk_gridmenu *menu, unsigned int ticks)
{
	assert(mlk_gridmenu_ok(menu));

	MLK__DELEGATE_INVOKE(menu->delegate, mlk_gridmenu_delegate, update, menu, ticks);
}

void
mlk_gridmenu_draw(const struct mlk_gridmenu *menu)
{
	assert(menu);

	MLK__DELEGATE_INVOKE(menu->delegate, mlk_gridmenu_delegate, draw, menu);
}
