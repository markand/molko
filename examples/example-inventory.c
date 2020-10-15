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

#include <core/clock.h>
#include <core/event.h>
#include <core/image.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/sys.h>
#include <core/texture.h>
#include <core/util.h>
#include <core/window.h>

#include <ui/theme.h>

#include <rpg/item.h>
#include <rpg/inventory.h>
#include <rpg/inventory_dialog.h>

/* https://shikashiassets.itch.io/shikashis-fantasy-icons-pack */
#include <assets/images/fish.h>
#include <assets/images/potion.h>
#include <assets/images/sword.h>

#define W 1280
#define H 720

static struct {
	const unsigned char *data;
	const size_t datasz;
	struct texture icon;
	struct item item;
} items[] = {
	{
		.data = images_potion,
		.datasz = sizeof (images_potion),
		.item = {
			.name = "Potion",
			.summary = "Recover 100 HP.",
			.stackable = ITEM_STACK_MAX
		}
	},
	{
		.data = images_fish,
		.datasz = sizeof (images_fish),
		.item = {
			.name = "Fish",
			.summary = "Recover 1000 HP.",
			.stackable = ITEM_STACK_MAX
		}
	},
	{
		.data = images_sword,
		.datasz = sizeof (images_sword),
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
	    !window_init("Example - Inventory", W, H) ||
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

	/* Add items manually to be able to sort. */
	inv.items[1][5].item = &items[0].item;
	inv.items[1][5].amount = 12;

	inv.items[1][2].item = &items[0].item;
	inv.items[1][2].amount = 9;

	inv.items[2][7].item = &items[1].item;
	inv.items[2][7].amount = 9;

	inv.items[2][8].item = &items[1].item;
	inv.items[2][8].amount = 3;

	inv.items[2][4].item = &items[2].item;
	inv.items[2][4].amount = 1;

	run(&dlg);
}

int
main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	init();
	basic();
	quit();
}
