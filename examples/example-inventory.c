/*
 * example-inventory.c -- show how to use inventory dialog
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

#include <clock.h>
#include <event.h>
#include <image.h>
#include <inventory.h>
#include <inventory_dialog.h>
#include <item.h>
#include <painter.h>
#include <panic.h>
#include <sys.h>
#include <texture.h>
#include <theme.h>
#include <util.h>
#include <window.h>

/* https://shikashiassets.itch.io/shikashis-fantasy-icons-pack */
#include "assets/images/fish.h"
#include "assets/images/potion.h"
#include "assets/images/sword.h"

#define W 1920
#define H 1080

static struct {
	const unsigned char *data;
	const size_t datasz;
	struct texture icon;
	struct item item;
} items[] = {
	{
		.data = potion,
		.datasz = sizeof (potion),
		.item = {
			.name = "Potion",
			.summary = "Recover 100 HP.",
			.stackable = ITEM_STACK_MAX
		}
	},
	{
		.data = fish,
		.datasz = sizeof (fish),
		.item = {
			.name = "Fish",
			.summary = "Recover 1000 HP.",
			.stackable = ITEM_STACK_MAX
		}
	},
	{
		.data = sword,
		.datasz = sizeof (sword),
		.item = {
			.name = "Sword",
			.summary = "A very basic sword.",
			.stackable = 1
		}
	},
};

static void
init(void)
{
	if (!sys_init() ||
	    !window_init("Example - Message", W, H) ||
	    !theme_init())
		panic();

	for (size_t i = 0; i < NELEM(items); ++i) {
		if (!image_openmem(&items[i].icon, items[i].data, items[i].datasz))
			panic();

		items[i].item.icon = &items[i].icon;
	}
}

static void
quit(void)
{
	theme_finish();
	window_finish();
	sys_finish();
}

static void
run(struct inventory_dialog *dlg)
{
	struct clock clock = {0};

	clock_start(&clock);
	inventory_dialog_open(dlg);

	while (dlg->state) {
		union event ev;
		unsigned int elapsed = clock_elapsed(&clock);

		clock_start(&clock);

		while (event_poll(&ev)) {
			switch (ev.type) {
			case EVENT_QUIT:
				inventory_dialog_finish(dlg);
				break;
			default:
				inventory_dialog_handle(dlg, &ev);
				break;
			}
		}

		inventory_dialog_update(dlg, elapsed);
		painter_set_color(0xffffffff);
		painter_clear();
		inventory_dialog_draw(dlg);
		painter_present();

		if ((elapsed = clock_elapsed(&clock)) < 20)
			delay(20 - elapsed);
	}
}

static void
basic(void)
{
	struct inventory inv = { 0 };
	struct inventory_dialog dlg = {
		.inv = &inv,
		.x = 60,
		.y = 60
	};

	inventory_push(&inv, &items[0].item, 10);     /* Potion */
	inventory_push(&inv, &items[1].item, 4);      /* Fish */
	inventory_push(&inv, &items[2].item, 1);      /* Sword */

	run(&dlg);
}

int
main(int argc, char **argv)
{
	init();
	basic();
	quit();
}
