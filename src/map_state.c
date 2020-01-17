/*
 * map_state.c -- state when player is on a map
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

#include <stdio.h>

#include "event.h"
#include "game.h"
#include "map.h"
#include "map_state.h"
#include "painter.h"
#include "state.h"
#include "texture.h"
#include "walksprite.h"
#include "window.h"

/*
 * This is the speed the player moves on the map.
 *
 * SPEED represents the number of pixels it must move per SEC.
 * SEC simply represends the number of milliseconds in one second.
 */
#define SPEED 100
#define SEC   1000

/*
 * Those are margins within the edge of the screen. The camera always try to
 * keep those padding between the player and the screen.
 */
#define MARGIN_WIDTH    80
#define MARGIN_HEIGHT   80

/*
 * This structure defines the possible movement of the player as flags since
 * it's possible to make diagonal movements.
 */
enum movement {
	MOVING_UP       = 1 << 0,
	MOVING_RIGHT    = 1 << 1,
	MOVING_DOWN     = 1 << 2,
	MOVING_LEFT     = 1 << 3
};

/*
 * A bit of explanation within this array. The structure walksprite requires
 * an orientation between 0-7 depending on the user direction.
 *
 * Since keys for moving the character may be pressed at the same time, we need
 * a conversion table from "key pressed" to "orientation".
 *
 * When an orientation is impossible, it is set to -1. Example, when both left
 * and right are pressed.
 *
 * MOVING_UP    = 0001 = 0x1
 * MOVING_RIGHT = 0010 = 0x2
 * MOVING_DOWN  = 0100 = 0x3
 * MOVING_LEFT  = 1000 = 0x4
 */
static unsigned int orientations[16] = {
	[0x1] = 0,
	[0x2] = 2,
	[0x3] = 1,
	[0x4] = 4,
	[0x6] = 3,
	[0x8] = 6,
	[0x9] = 7,
	[0xC] = 5
};

static struct {
	struct map map;
	struct walksprite player_sprite;
	int player_x;
	int player_y;
	int player_orientation;
	int view_x;
	int view_y;
	enum movement moving;
} data;

static void
enter(void)
{
}

static void
leave(void)
{
}

static void
handle_keydown(const union event *event)
{
	switch (event->key.key) {
	case KEY_UP:
		data.moving |= MOVING_UP;
		break;
	case KEY_RIGHT:
		data.moving |= MOVING_RIGHT;
		break;
	case KEY_DOWN:
		data.moving |= MOVING_DOWN;
		break;
	case KEY_LEFT:
		data.moving |= MOVING_LEFT;
		break;
	default:
		break;
	}

	data.player_orientation = orientations[data.moving];
}

static void
handle_keyup(const union event *event)
{
	switch (event->key.key) {
	case KEY_UP:
		data.moving &= ~(MOVING_UP);
		break;
	case KEY_RIGHT:
		data.moving &= ~(MOVING_RIGHT);
		break;
	case KEY_DOWN:
		data.moving &= ~(MOVING_DOWN);
		break;
	case KEY_LEFT:
		data.moving &= ~(MOVING_LEFT);
		break;
	default:
		break;
	}
}

static void
move(unsigned int ticks)
{
	/* This is the amount of pixels the player must move. */
	const int delta = SPEED * ticks / SEC;

	/* This is the view width which simply equals to the screen. */
	const unsigned int view_w = window_width();
	const unsigned int view_h = window_height();

	/* This is the rectangle within the view where users must be. */
	const int margin_x = data.view_x + MARGIN_WIDTH;
	const int margin_y = data.view_y + MARGIN_HEIGHT;
	const unsigned int margin_w = window_width() - (MARGIN_WIDTH * 2);
	const unsigned int margin_h = window_height() - (MARGIN_HEIGHT * 2);

	/* This is map size. */
	const unsigned int map_w = texture_width(data.map.picture);
	const unsigned int map_h = texture_height(data.map.picture);

	int dx = 0;
	int dy = 0;

	if (data.moving == 0)
		return;

	if (data.moving & MOVING_UP)
		dy = -1;
	if (data.moving & MOVING_DOWN)
		dy = 1;
	if (data.moving & MOVING_LEFT)
		dx = -1;
	if (data.moving & MOVING_RIGHT)
		dx = 1;

	/* Move the player and adjust view if needed. */
	if (dx > 0) {
		data.player_x += delta;

		if (data.player_x > (int)(margin_x + margin_w)) {
			data.view_x = (data.player_x - view_w) + MARGIN_WIDTH;

			if (data.view_x >= (int)(map_w - view_w))
				data.view_x = map_w - view_w;
		}

		if (data.player_x > (int)map_w - 48)
			data.player_x = map_w - 48;
	} else if (dx < 0) {
		data.player_x -= delta;

		if (data.player_x < margin_x) {
			data.view_x = data.player_x - MARGIN_WIDTH;

			if (data.view_x < 0)
				data.view_x = 0;
		}

		if (data.player_x < 0)
			data.player_x = 0;
	}

	if (dy > 0) {
		data.player_y += delta;

		if (data.player_y > (int)(margin_y + margin_h)) {
			data.view_y = (data.player_y - view_h) + MARGIN_HEIGHT;

			if (data.view_y >= (int)(map_h - view_h))
				data.view_y = map_h - view_h;
		}

		if (data.player_y > (int)map_h - 48)
			data.player_y = map_h - 48;
	} else if (dy < 0) {
		data.player_y -= delta;

		if (data.player_y < margin_y) {
			data.view_y = data.player_y - MARGIN_HEIGHT;

			if (data.view_y < 0)
				data.view_y = 0;
		}

		if (data.player_y < 0)
			data.player_y = 0;
	}

	walksprite_update(&data.player_sprite, ticks);
}

static void
handle(const union event *event)
{
	switch (event->type) {
	case EVENT_KEYDOWN:
		handle_keydown(event);
		break;
	case EVENT_KEYUP:
		handle_keyup(event);
		break;
	default:
		break;
	}
}

static void
update(unsigned int ticks)
{
	move(ticks);
}

static void
draw(void)
{
	painter_set_color(0x000000ff);
	painter_clear();
	map_repaint(&data.map);
	map_draw(&data.map, data.view_x, data.view_y);
	walksprite_draw(&data.player_sprite, data.player_orientation, data.player_x - data.view_x, data.player_y - data.view_y);
	painter_present();
}

static void
center(void)
{
	/* This is map size. */
	const unsigned int win_w = window_width();
	const unsigned int win_h = window_height();
	const unsigned int map_w = texture_width(data.map.picture);
	const unsigned int map_h = texture_height(data.map.picture);

	data.view_x = data.player_x - (win_w / 2);
	data.view_y = data.player_y - (win_h / 2);

	if (data.view_x < 0)
		data.view_x = 0;
	else if (data.view_x > (int)(map_w - win_w))
		data.view_x = map_w - win_w;

	if (data.view_y < 0)
		data.view_y = 0;
	else if (data.view_y > (int)(map_h - win_h))
		data.view_y = map_h - win_h;
}

void
map_state_start(struct map *map, struct sprite *s)
{
	/* Copy essential. */
	data.map = *map;
	data.player_x = map->origin_x;
	data.player_y = map->origin_y;

	center();
	walksprite_init(&data.player_sprite, s, 250);
	game_switch(&map_state);
}

struct state map_state = {
	.enter = enter,
	.leave = leave,
	.update = update,
	.handle = handle,
	.draw = draw
};
