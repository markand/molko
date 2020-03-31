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

#include "button.h"
#include "frame.h"
#include "game.h"
#include "inventory.h"
#include "inventory_dialog.h"
#include "item.h"
#include "painter.h"
#include "texture.h"
#include "window.h"

#define ITEM_SIZE       64
#define ITEM_PADDING    32

#define GRID_WIDTH      ((INVENTORY_COLS_MAX * ITEM_SIZE) +             \
                        ((INVENTORY_COLS_MAX + 1) * ITEM_PADDING))
#define GRID_HEIGHT     ((INVENTORY_ROWS_MAX * ITEM_SIZE) +             \
                        ((INVENTORY_ROWS_MAX + 1) * ITEM_PADDING))

#define LABEL_WIDTH     (GRID_WIDTH)
#define LABEL_HEIGHT    (32)

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
draw_grid_item_label(struct inventory_slot *slot, int x, int y)
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
draw_grid_item(struct inventory_slot *slot, int x, int y)
{
	draw_grid_item_frame(x, y);

	if (slot->item) {
		draw_grid_item_icon(slot->item, x, y);
		draw_grid_item_label(slot, x, y);
	}
}

static void
draw_grid_items(const struct inventory_dialog *dlg)
{
	int x;
	int y;

	y = ITEM_PADDING;

	for (int r = 0; r < INVENTORY_ROWS_MAX; ++r) {
		x = dlg->fgrid.x + ITEM_PADDING;

		for (int c = 0; c < INVENTORY_COLS_MAX; ++c) {
			draw_grid_item(&dlg->inv->items[r][c], x, y);
			x += ITEM_SIZE + ITEM_PADDING;
		}

		y += ITEM_SIZE + ITEM_PADDING;
	}
}

static void
draw_label(struct inventory_dialog *dlg)
{
	assert(dlg);

	struct item *item = dlg->inv->items[dlg->selrow][dlg->selcol].item;

	frame_draw(&dlg->flabel);

	if (item) {
		dlg->ldesc.text = item->summary;
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
	dlg->fgrid.x = 0; /* TODO */
	dlg->fgrid.y = 0;

	/* Label frame position. */
	dlg->flabel.w = dlg->ldesc.w = LABEL_WIDTH;
	dlg->flabel.h = dlg->ldesc.h = LABEL_HEIGHT;
	dlg->flabel.x = dlg->ldesc.x = 0; /* TODO */
	dlg->flabel.y = dlg->ldesc.y = GRID_HEIGHT;
	dlg->ldesc.x += ITEM_PADDING;
	dlg->ldesc.flags = LABEL_NO_HCENTER;

	/* Button sort. */
	dlg->bsort.x = 0;
	dlg->bsort.y = GRID_HEIGHT + LABEL_HEIGHT;
	dlg->bsort.w = BUTTON_WIDTH;
	dlg->bsort.h = BUTTON_HEIGHT;
	dlg->bsort.text = "Sort";
}

void
inventory_dialog_handle(struct inventory_dialog *dlg, const union event *event)
{
	assert(event);

#if 0
	button_handle(&data.sort, event);

	if (data.sort.state == BUTTON_STATE_ACTIVATED) {
		inventory_sort(data.inv);
		button_reset(&data.sort);
	}
#endif
}

void
inventory_dialog_update(struct inventory_dialog *dlg, unsigned int ticks)
{
	assert(dlg);

	(void)ticks;
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
