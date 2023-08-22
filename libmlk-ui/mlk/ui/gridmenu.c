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
#include <mlk/core/trace.h>
#include <mlk/core/window.h>

#include "align.h"
#include "frame.h"
#include "gridmenu.h"
#include "label.h"
#include "ui.h"
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

	size_t pagesz, pagenr, selected, c = 0, r = 0;
	int x, y;

	pagesz = menu->nrows * menu->ncols;
	pagenr = menu->selected / pagesz;

	for (size_t i = 0; i < pagesz; ++i) {
		x = (int)(menu->x + (c * menu->eltw) + (c * menu->spacew));
		y = (int)(menu->y + (r * menu->elth) + (r * menu->spaceh));

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
draw_frame(struct mlk_gridmenu_style *self, struct mlk_gridmenu *menu)
{
	(void)self;

	struct mlk_frame frame = {
		.x = menu->x,
		.y = menu->y,
		.w = menu->w,
		.h = menu->h
	};

	mlk_frame_draw(&frame);
}

static void
draw_text(struct mlk_gridmenu_style *self,
          struct mlk_gridmenu *menu,
          const char *item,
          unsigned int row,
          unsigned int col,
          int selected)
{
	unsigned long color = selected ? self->color_selected : self->color;

	mlk_ui_draw_text(
		MLK_ALIGN_CENTER,
		MLK__STYLE_FONT(self->font, MLK_UI_FONT_INTERFACE),
		color,
		item,
		menu->x + (col * menu->eltw) + (col * menu->spacew),
		menu->y + (row * menu->elth) + (row * menu->spaceh),
		menu->eltw,
		menu->elth
	);
}

struct mlk_gridmenu_style mlk_gridmenu_style_dark = {
	.background = 0x222323ff,
	.border = 0x141414ff,
	.border_size = 2,
	.color = 0xffffffff,
	.color_selected = 0x518822ff,
	.draw_frame = draw_frame,
	.draw_text = draw_text
};

struct mlk_gridmenu_style mlk_gridmenu_style_light = {
	.background = 0x222323ff,
	.border = 0x141414ff,
	.border_size = 2,
	.color = 0x000000ff,
	.color_selected = 0x518822ff,
	.draw_frame = draw_frame,
	.draw_text = draw_text
};

struct mlk_gridmenu_style *mlk_gridmenu_style = &mlk_gridmenu_style_light;

void
mlk_gridmenu_resize(struct mlk_gridmenu *menu)
{
	unsigned int reqw = 0, reqh = 0, lw, lh;
	struct mlk_font *font;

	/* Compute which item has the bigger width/height to create a spacing. */
	menu->eltw = menu->elth = 0;
	menu->spacew = menu->spaceh = 0;

	/* Get appropriate font. */
	if (menu->style)
		font = MLK__STYLE_FONT(menu->style->font, MLK_UI_FONT_INTERFACE);
	else
		font = &mlk_ui_fonts[MLK_UI_FONT_INTERFACE];

	for (size_t i = 0; i < menu->itemsz; ++i) {
		if (!(menu->items[i]))
			continue;

		mlk_font_query(font, menu->items[i], &lw, &lh);

		menu->eltw = fmax(menu->eltw, lw);
		menu->elth = fmax(menu->elth, lh);
	}

	/* Total texture size required to draw items. */
	reqw = menu->eltw * menu->ncols;
	reqh = menu->elth * menu->nrows;

	/*
	 * Compute spacing between elements. We remove the padding because it
	 * is outside of the elements.
	 */
	if (reqw > menu->w) {
		mlk_tracef(_("gridmenu width is too small: %u < %u"), menu->w, reqw);
		menu->spacew = 1;
	} else if (menu->ncols > 1)
		menu->spacew = (menu->w - reqw) / menu->ncols;

	if (reqh > menu->h) {
		mlk_tracef(_("gridmenu height is too small: %u < %u"), menu->h, reqh);
		menu->spaceh = 1;
	} else if (menu->nrows > 1)
		menu->spaceh = (menu->h - reqh) / menu->nrows;
}

int
mlk_gridmenu_handle(struct mlk_gridmenu *menu, const union mlk_event *ev)
{
	assert(menu);
	assert(ev);

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

void
mlk_gridmenu_update(struct mlk_gridmenu *menu, unsigned int ticks)
{
	assert(menu);

	MLK__STYLE_CALL(menu->style, mlk_gridmenu_style, update, menu, ticks);
}

void
mlk_gridmenu_draw(struct mlk_gridmenu *menu)
{
	assert(menu);

	size_t pagesz, pagenr, item;
	unsigned int row = 0, col = 0;
	int selected;

	MLK__STYLE_CALL(menu->style, mlk_gridmenu_style, draw_frame, menu);

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

		selected = i == menu->selected % pagesz;

		MLK__STYLE_CALL(menu->style, mlk_gridmenu_style, draw_text,
		    menu, menu->items[item], row, col, selected);

		if (++col >= menu->ncols) {
			++row;
			col = 0;
		}
	}
}
