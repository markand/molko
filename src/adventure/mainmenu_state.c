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

#include <error.h>
#include <event.h>
#include <font.h>
#include <game.h>
#include <image.h>
#include <map_state.h>
#include <painter.h>
#include <state.h>
#include <sys.h>
#include <texture.h>
#include <window.h>

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

/* Menu items. */
static struct {
	struct texture *texture;
	int x;
	int y;
} items[3];

static void
enter(void)
{
	struct font *font = font_openf(sys_datapath("fonts/pirata-one.ttf"), 30);

	if (!font)
		error_fatal();

	substate = SUBSTATE_MOVING;
	x = splashscreen_state_data.x;
	y = splashscreen_state_data.y;
	destination = window_height() / 4;

	/* TODO: change continue color if no game exists. */
	items[0].texture = font_render(font, "New game", 0x000000ff);
	items[0].x = (window_width() / 2) - (texture_width(items[0].texture) / 2);
	items[0].y = window_height() * 0.75;

	items[1].texture = font_render(font, "Continue", 0x000000ff);
	items[1].x = items[0].x;
	items[1].y = items[0].y + texture_height(items[0].texture);

	items[2].texture = font_render(font, "Quit", 0x000000ff);
	items[2].x = items[0].x;
	items[2].y = items[1].y + texture_height(items[1].texture);
}

static void
new(void)
{
	struct map map;
	struct texture *image;

	/* Prepare map. */
	if (!map_open(&map, sys_datapath("maps/test.map")))
		error_fatal();

	memcpy(&map_state_data.map.map, &map, sizeof (map));

	/* Prepare image and sprite. */
	if (!(image = image_openf(sys_datapath("sprites/test-walk.png"))))
		error_fatal();

	sprite_init(&map_state_data.player.sprite, image, 48, 48);
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
	texture_draw(splashscreen_state_data.text, x, y);

	if (substate == SUBSTATE_WAITING) {
		texture_draw(items[0].texture, items[0].x, items[0].y);
		texture_draw(items[1].texture, items[1].x, items[1].y);
		texture_draw(items[2].texture, items[2].x, items[2].y);

		/* Selection cursor. */

		/* TODO: a sword here. */
		painter_set_color(0x000000ff);
		painter_draw_rectangle(true, items[selection].x - 30,
		    items[selection].y + 11, 15, 15);
	}
}

static void
leave(void)
{
	texture_close(items[0].texture);
	texture_close(items[1].texture);
	memset(items, 0, sizeof (items));
}

struct state mainmenu_state = {
	.enter = enter,
	.handle = handle,
	.update = update,
	.draw = draw,
	.leave = leave
};
