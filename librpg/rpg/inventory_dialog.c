/*
 * inventory_dialog.h -- dialog for items
 *
 * Copyright (c) 2020 David Demelier <markand@malikania.fr>
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
#include <stdio.h>

#include <core/event.h>
#include <core/game.h>
#include <core/maths.h>
#include <core/painter.h>
#include <core/texture.h>
#include <core/window.h>

#include "inventory.h"
#include "inventory_dialog.h"
#include "item.h"

#define ITEM_SIZE       64
#define ITEM_PADDING    32

#define GRID_WIDTH      ((INVENTORY_COLS_MAX * ITEM_SIZE) +             \
                        ((INVENTORY_COLS_MAX + 1) * ITEM_PADDING))
#define GRID_HEIGHT     ((INVENTORY_ROWS_MAX * ITEM_SIZE) +             \
                        ((INVENTORY_ROWS_MAX + 1) * ITEM_PADDING))

#define LABEL_WIDTH     (GRID_WIDTH)
#define LABEL_HEIGHT    (64)

#define BUTTON_HEIGHT   (32)
#define BUTTON_WIDTH    ((GRID_WIDTH) / 4)

/*
 * The frame looks like this:
 *
 * +----------------------------+
 * | [] [] [] [] [] [] [] [] [] |
 * | [] [] [] [] [] [] [] [] [] |
 * | [] [] [] [] [] [] [] [] [] |
 * +----------------------------+
 * | Item name                  |
 * | Item description           |
 * +----------------------------+
 *                       [ Sort ]
 *
 * Where space between cells is determined with ITEM_PADDING macro.
 */

static unsigned int
total_width(void)
{
	return GRID_WIDTH;
}

static unsigned int
total_height(void)
{
	return GRID_HEIGHT + LABEL_HEIGHT + BUTTON_HEIGHT;
}

static void
compute_box_position(const struct inventory_dialog *dlg, int r, int c, int *x, int *y)
{
	assert(dlg);
	assert(x);
	assert(y);

	*x = dlg->fgrid.x + ((c * ITEM_SIZE) + ((c + 1) * ITEM_PADDING));
	*y = dlg->fgrid.y + ((r * ITEM_SIZE) + ((r + 1) * ITEM_PADDING));
}

static void
draw_grid_item_frame(int x, int y)
{
	struct frame frame = {
		.x = x,
		.y = y,
		.w = ITEM_SIZE,
		.h = ITEM_SIZE,
		.style = FRAME_STYLE_BOX
	};

	frame_draw(&frame);
}

static void
draw_grid_item_icon(struct item *item, int x, int y)
{
	struct texture *icon = item->icon;

	texture_scale(icon, 0, 0, icon->w, icon->h, x, y, ITEM_SIZE, ITEM_SIZE, 0.0);
}

static void
draw_grid_item_amount(struct inventory_slot *slot, int x, int y)
{
	char nstr[16];
	struct label label = {
		.text = nstr,
		.x = x,
		.y = y
	};

	/* Draw the number of item in this slot. */
	snprintf(nstr, sizeof (nstr), "%d", slot->amount);
	label_draw(&label);
}

static void
draw_grid_item(struct inventory_slot *slot, int x, int y, bool selected)
{
	draw_grid_item_frame(x, y);

	if (slot->item) {
		draw_grid_item_icon(slot->item, x, y);
		draw_grid_item_amount(slot, x, y);
	}

	if (selected) {
		x -= 16;
		y += (ITEM_SIZE / 2) - 4;
		painter_draw_circle(x, y, 8);
	}
}

static void
draw_grid_items(const struct inventory_dialog *dlg)
{
	int x, y;
	bool selected;

	for (unsigned int r = 0; r < INVENTORY_ROWS_MAX; ++r) {
		for (unsigned int c = 0; c < INVENTORY_COLS_MAX; ++c) {
			selected = r == dlg->selrow && c == dlg->selcol;
			compute_box_position(dlg, r, c, &x, &y);
			draw_grid_item(&dlg->inv->items[r][c], x, y, selected);
		}
	}
}

static void
draw_label(struct inventory_dialog *dlg)
{
	assert(dlg);

	struct item *item = dlg->inv->items[dlg->selrow][dlg->selcol].item;

	frame_draw(&dlg->fname);
	frame_draw(&dlg->fdesc);

	if (item) {
		dlg->lname.text = item->name;
		dlg->ldesc.text = item->summary;
		label_draw(&dlg->lname);
		label_draw(&dlg->ldesc);
	}
}

void
inventory_dialog_open(struct inventory_dialog *dlg)
{
	assert(dlg);
	assert(dlg->inv);

	int tw, th;

	dlg->state = INVENTORY_DIALOG_SHOWING;

	tw = total_width();
	th = total_height();

	/* Grid frame position. */
	dlg->fgrid.w = GRID_WIDTH;
	dlg->fgrid.h = GRID_HEIGHT;
	dlg->fgrid.x = dlg->x;
	dlg->fgrid.y = dlg->y;

	/* Name label. */
	dlg->fname.w = dlg->lname.w = LABEL_WIDTH;
	dlg->fname.h = dlg->lname.h = LABEL_HEIGHT / 2;
	dlg->fname.x = dlg->lname.x = dlg->x;
	dlg->fname.y = dlg->lname.y = dlg->y + GRID_HEIGHT;
	dlg->lname.x += ITEM_PADDING;
	dlg->lname.align = ALIGN_LEFT;

	/* Description label. */
	dlg->fdesc.w = dlg->ldesc.w = LABEL_WIDTH;
	dlg->fdesc.h = dlg->ldesc.h = LABEL_HEIGHT / 2;
	dlg->fdesc.x = dlg->ldesc.x = dlg->y;
	dlg->fdesc.y = dlg->ldesc.y = dlg->y + GRID_HEIGHT + (LABEL_HEIGHT / 2);
	dlg->ldesc.x += ITEM_PADDING;
	dlg->ldesc.align = ALIGN_LEFT;

	/* Button sort. */
	dlg->bsort.x = dlg->x;
	dlg->bsort.y = dlg->y + GRID_HEIGHT + LABEL_HEIGHT;
	dlg->bsort.w = BUTTON_WIDTH;
	dlg->bsort.h = BUTTON_HEIGHT;
	dlg->bsort.text = "Sort";
}

static void
handle_keydown(struct inventory_dialog *dlg, const struct event_key *ev)
{
	assert(ev && ev->type == EVENT_KEYDOWN);

	switch (ev->key) {
	case KEY_LEFT:
		if (dlg->selcol == 0)
			dlg->selcol = INVENTORY_COLS_MAX - 1;
		else
			dlg->selcol--;
		break;
	case KEY_RIGHT:
		dlg->selcol = (dlg->selcol + 1) % INVENTORY_COLS_MAX;
		break;
	case KEY_UP:
		if (dlg->selrow == 0)
			dlg->selrow = INVENTORY_ROWS_MAX - 1;
		else
			dlg->selrow--;
		break;
	case KEY_DOWN:
		dlg->selrow = (dlg->selrow + 1) % INVENTORY_ROWS_MAX;
		break;
	default:
		break;
	}
}

static void
handle_clickdown(struct inventory_dialog *dlg, const struct event_click *ev)
{
	assert(dlg);
	assert(ev && ev->type == EVENT_CLICKDOWN);

	int x, y;

	for (int r = 0; r < INVENTORY_ROWS_MAX; ++r) {
		for (int c = 0; c < INVENTORY_COLS_MAX; ++c) {
			compute_box_position(dlg, r, c, &x, &y);

			if (maths_is_boxed(x, y, ITEM_SIZE, ITEM_SIZE, ev->x, ev->y)) {
				dlg->selrow = r;
				dlg->selcol = c;
			}
		}
	}
}

void
inventory_dialog_handle(struct inventory_dialog *dlg, const union event *event)
{
	assert(event);

	switch (event->type) {
	case EVENT_KEYDOWN:
		handle_keydown(dlg, &event->key);
		break;
	case EVENT_CLICKDOWN:
		handle_clickdown(dlg, &event->click);
		break;
	default:
		break;
	}

	button_handle(&dlg->bsort, event);

	if (dlg->bsort.state == BUTTON_STATE_ACTIVATED) {
		inventory_sort(dlg->inv);
		button_reset(&dlg->bsort);
	}
}

void
inventory_dialog_update(struct inventory_dialog *dlg, unsigned int ticks)
{
	assert(dlg);

	(void)ticks;
}

void
inventory_dialog_move(struct inventory_dialog *dlg, int x, int y)
{
	assert(dlg);

	dlg->x = x;
	dlg->y = y;
}

void
inventory_dialog_draw(struct inventory_dialog *dlg)
{
	assert(dlg);

	frame_draw(&dlg->fgrid);
	draw_grid_items(dlg);
	draw_label(dlg);
	button_draw(&dlg->bsort);
}

void
inventory_dialog_finish(struct inventory_dialog *dlg)
{
	assert(dlg);

	dlg->state = INVENTORY_DIALOG_NONE;
}
