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

#include "button.h"
#include "frame.h"
#include "game.h"
#include "inventory.h"
#include "inventory_dialog.h"
#include "painter.h"
#include "rect.h"
#include "size.h"
#include "window.h"

#define ITEM_SIZE	64
#define ITEM_PADDING	32

static struct {
	struct inventory *inv;
	struct rect geo;
	struct button sort;
} data = {
	.sort = {
		.text = "Sort"
	}
};

/*
 * The frame looks like this:
 *
 * +----------------------------+
 * | [] [] [] [] [] [] [] [] [] |
 * | [] [] [] [] [] [] [] [] [] |
 * | [] [] [] [] [] [] [] [] [] |
 * +----------------------------+
 *
 * Where space between cells is determined with ITEM_PADDING macro.
 */
static void
compute_frame_size(void)
{
	struct size winsz;
	struct rect winrc;

	/* 1. Compute size. */
	data.geo.w  = (INVENTORY_COLS_MAX * ITEM_SIZE);
	data.geo.w += (INVENTORY_COLS_MAX + 1) * ITEM_PADDING;
	data.geo.h  = (INVENTORY_ROWS_MAX * ITEM_SIZE);
	data.geo.h += (INVENTORY_ROWS_MAX + 1) * ITEM_PADDING;

	/* 2. Center on the screen. */
	winsz = window_size();
	winrc.w = winsz.w;
	winrc.h = winsz.h;
	winrc.x = winrc.y = 0;

	rect_center(&data.geo, &winrc);
}

static void
compute_button_positions(void)
{
	data.sort.geo.w = data.geo.w / 4;
	data.sort.geo.h = 32;
	data.sort.geo.x = data.geo.x + (data.geo.w - data.sort.geo.w);
	data.sort.geo.y = data.geo.y + data.geo.h;
}

void
inventory_dialog_open(struct inventory *inv)
{
	assert(inv);

	data.inv = inv;

	compute_frame_size();
	compute_button_positions();
}

void
inventory_dialog_handle(const union event *event)
{
	assert(event);

	button_handle(&data.sort, event);

	if (data.sort.state == BUTTON_STATE_ACTIVATED) {
		inventory_sort(data.inv);
		button_reset(&data.sort);
	}
}

void
inventory_dialog_update(unsigned int ticks)
{
	(void)ticks;
}

static void
draw_frame(void)
{
	struct frame frame = {
		.geo = data.geo
	};

	frame_draw(&frame);
}

static void
draw_items(void)
{
	struct frame frame = {
		.style = FRAME_STYLE_BOX,
		.geo = {
			.y = data.geo.y + ITEM_PADDING,
			.w = ITEM_SIZE,
			.h = ITEM_SIZE
		}
	};
	struct texture *tex;

	for (int r = 0; r < INVENTORY_ROWS_MAX; ++r) {
		frame.geo.x = data.geo.x + ITEM_PADDING;

		for (int c = 0; c < INVENTORY_COLS_MAX; ++c) {
			frame_draw(&frame);
			frame.geo.x += ITEM_SIZE + ITEM_PADDING;


		}

		frame.geo.y += ITEM_SIZE + ITEM_PADDING;
	}
}

static void
draw_buttons(void)
{
	button_draw(&data.sort);
}

void
inventory_dialog_draw(void)
{
	draw_frame();
	draw_items();
	draw_buttons();
}
