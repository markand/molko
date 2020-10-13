/*
 * mainmenu_state.c -- game main menu
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
#include <stdlib.h>
#include <string.h>

#include <core/event.h>
#include <core/font.h>
#include <core/game.h>
#include <core/image.h>
#include <core/map_state.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/state.h>
#include <core/sys.h>
#include <core/texture.h>
#include <core/window.h>

#include <adventure/assets/fonts/pirata-one.h>
#include <adventure/assets/maps/overworld.h>
#include <adventure/assets/tilesets/world.h>
#include <adventure/assets/sprites/john.h>

#include "mainmenu_state.h"
#include "splashscreen_state.h"

#define SPEED 120
#define SEC   1000

enum substate {
	SUBSTATE_MOVING,
	SUBSTATE_WAITING
};

static int x;
static int y;
static unsigned int selection;
static int destination;
static enum substate substate;
static struct texture image;
static struct texture tileset;

/* Menu items. */
static struct {
	struct texture texture;
	int x;
	int y;
} items[3];

static void
enter(void)
{
	struct font font;

	if (!font_openmem(&font, fonts_pirata_one, sizeof (fonts_pirata_one), 30))
		panic();

	substate = SUBSTATE_MOVING;
	x = splashscreen_state_data.x;
	y = splashscreen_state_data.y;
	destination = window.h / 4;

	/* TODO: change continue color if no game exists. */
	font_render(&font, &items[0].texture, "New game");
	items[0].x = (window.w / 2) - (items[0].texture.w / 2);
	items[0].y = window.h * 0.75;

	font_render(&font, &items[1].texture, "Continue");
	items[1].x = items[0].x;
	items[1].y = items[0].y + items[0].texture.h;

	font_render(&font, &items[2].texture, "Quit");
	items[2].x = items[0].x;
	items[2].y = items[1].y + items[1].texture.h;

	font_finish(&font);
}

static void
new(void)
{
	/* Prepare map. */
	if (!map_data_openmem(&map_state_data.map.data, maps_overworld, sizeof (maps_overworld)))
		panicf("Unable to open map 'test'");

	// TODO: this is temporary.
	if (!image_openmem(&tileset, tilesets_world, sizeof (tilesets_world)))
		panic();
	
	if (!map_init(&map_state_data.map.map, &map_state_data.map.data, &tileset))
		panic();

	/* Prepare image and sprite. */
	if (!(image_openmem(&image, sprites_john, sizeof (sprites_john))))
		panic();

	sprite_init(&map_state_data.player.sprite, &image, 48, 48);
	game_switch(&map_state, false);
}

static void
resume(void)
{
}

static void
quit(void)
{
	game_quit();
}

static void
perform(void)
{
	assert(selection < 3);

	static void (*handlers[])(void) = {
		[0] = new,
		[1] = resume,
		[2] = quit
	};

	handlers[selection]();
}

static void
handle(const union event *event)
{
	if (substate != SUBSTATE_WAITING)
		return;

	switch (event->type) {
	case EVENT_KEYDOWN:
		switch (event->key.key) {
		case KEY_UP:
			selection = selection == 0 ? 2 : selection - 1;
			break;
		case KEY_DOWN:
			selection = (selection + 1) % 3;
			break;
		case KEY_ENTER:
			perform();
		default:
			break;
		}
		break;
	default:
		break;
	}
}

static void
update(unsigned int ticks)
{
	switch (substate) {
	case SUBSTATE_MOVING:
		y -= SPEED * ticks / SEC;

		if (y <= destination) {
			y = destination;
			substate = SUBSTATE_WAITING;
		}

		break;
	default:
		break;
	}
}

static void
draw(void)
{
	painter_set_color(0xffffffff);
	painter_clear();
	texture_draw(&splashscreen_state_data.text, x, y);

	if (substate == SUBSTATE_WAITING) {
		texture_draw(&items[0].texture, items[0].x, items[0].y);
		texture_draw(&items[1].texture, items[1].x, items[1].y);
		texture_draw(&items[2].texture, items[2].x, items[2].y);

		/* Selection cursor. */

		/* TODO: a sword here. */
		painter_set_color(0x000000ff);
		painter_draw_rectangle(items[selection].x - 30,
		    items[selection].y + 11, 15, 15);
	}
}

static void
leave(void)
{
	texture_finish(&items[0].texture);
	texture_finish(&items[1].texture);
	memset(items, 0, sizeof (items));
}

struct state mainmenu_state = {
	.enter = enter,
	.handle = handle,
	.update = update,
	.draw = draw,
	.leave = leave
};
