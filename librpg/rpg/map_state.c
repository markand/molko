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

#include <core/event.h>
#include <core/game.h>
#include <core/painter.h>
#include <core/state.h>
#include <core/texture.h>
#include <core/window.h>

#include <ui/debug.h>

#include "map.h"
#include "map_state.h"
#include "walksprite.h"

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
 * Convenient macros to access the state data.
 */
#define MAP()           (&map_state_data.map)
#define PLAYER()        (&map_state_data.player)
#define VIEW()          (&map_state_data.view)

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

/*
 * Additional data that is not necessary to expose in map_state_data.
 */
static struct {
	struct {
		enum movement moving;
		struct walksprite ws;
	} player;

	struct {
		int x;
		int y;
		unsigned int w;
		unsigned int h;
	} margin;
} cache;

static void
center(void)
{
	VIEW()->x = PLAYER()->x - (VIEW()->w / 2);
	VIEW()->y = PLAYER()->y - (VIEW()->h / 2);

	if (VIEW()->x < 0)
		VIEW()->x = 0;
	else if ((unsigned int)VIEW()->x > MAP()->data.real_w - VIEW()->w)
		VIEW()->x = MAP()->data.real_w - VIEW()->w;

	if (VIEW()->y < 0)
		VIEW()->y = 0;
	else if ((unsigned int)VIEW()->y > MAP()->data.real_h - VIEW()->h)
		VIEW()->y = MAP()->data.real_h - VIEW()->h;
}

static void
enter(void)
{
	/* Adjust map properties. */
	struct map *m = &map_state_data.map.map;

	map_repaint(m);
	MAP()->data.real_w = m->picture.w;
	MAP()->data.real_h = m->picture.h;

	/* Adjust view. */
	VIEW()->w = window.w;
	VIEW()->h = window.h;

	/* Adjust margin. */
	cache.margin.w = VIEW()->w - (MARGIN_WIDTH * 2);
	cache.margin.h = VIEW()->h - (MARGIN_HEIGHT * 2);

	/* Center the view by default. */
	center();

	/* Final bits. */
	walksprite_init(&cache.player.ws, &PLAYER()->sprite, 300);
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
		cache.player.moving |= MOVING_UP;
		break;
	case KEY_RIGHT:
		cache.player.moving |= MOVING_RIGHT;
		break;
	case KEY_DOWN:
		cache.player.moving |= MOVING_DOWN;
		break;
	case KEY_LEFT:
		cache.player.moving |= MOVING_LEFT;
		break;
	default:
		break;
	}

	PLAYER()->angle = orientations[cache.player.moving];
}

static void
handle_keyup(const union event *event)
{
	switch (event->key.key) {
	case KEY_UP:
		cache.player.moving &= ~(MOVING_UP);
		break;
	case KEY_RIGHT:
		cache.player.moving &= ~(MOVING_RIGHT);
		break;
	case KEY_DOWN:
		cache.player.moving &= ~(MOVING_DOWN);
		break;
	case KEY_LEFT:
		cache.player.moving &= ~(MOVING_LEFT);
		break;
	default:
		break;
	}
}

static void
move_right(unsigned int delta)
{
	PLAYER()->x += delta;

	if (PLAYER()->x > (int)(cache.margin.x + cache.margin.w)) {
		VIEW()->x = (PLAYER()->x - VIEW()->w) + MARGIN_WIDTH;

		if (VIEW()->x >= (int)(MAP()->data.real_w - VIEW()->w))
			VIEW()->x = MAP()->data.real_w - VIEW()->w;
	}

	if (PLAYER()->x > (int)MAP()->data.real_w - 48)
		PLAYER()->x = MAP()->data.real_w - 48;
}

static void
move_left(unsigned int delta)
{
	PLAYER()->x -= delta;

	if (PLAYER()->x < cache.margin.x) {
		VIEW()->x = PLAYER()->x - MARGIN_WIDTH;

		if (VIEW()->x < 0)
			VIEW()->x = 0;
	}

	if (PLAYER()->x < 0)
		PLAYER()->x = 0;
}

static void
move_down(unsigned int delta)
{
	PLAYER()->y += delta;

	if (PLAYER()->y > (int)(cache.margin.y + cache.margin.h)) {
		VIEW()->y = (PLAYER()->y - VIEW()->h) + MARGIN_HEIGHT;

		if (VIEW()->y >= (int)(MAP()->data.real_h - VIEW()->h))
			VIEW()->y = MAP()->data.real_h - VIEW()->h;
	}

	if (PLAYER()->y > (int)MAP()->data.real_h - 48)
		PLAYER()->y = MAP()->data.real_h - 48;
}

static void
move_up(unsigned int delta)
{
	PLAYER()->y -= delta;

	if (PLAYER()->y < cache.margin.y) {
		VIEW()->y = PLAYER()->y - MARGIN_HEIGHT;

		if (VIEW()->y < 0)
			VIEW()->y = 0;
	}

	if (PLAYER()->y < 0)
		PLAYER()->y = 0;
}

static void
move(unsigned int ticks)
{
	/* This is the amount of pixels the player must move. */
	const int delta = SPEED * ticks / SEC;

	/* This is the rectangle within the view where users must be. */
	cache.margin.x = VIEW()->x + MARGIN_WIDTH;
	cache.margin.y = VIEW()->y + MARGIN_HEIGHT;

	int dx = 0;
	int dy = 0;

	if (cache.player.moving == 0)
		return;

	if (cache.player.moving & MOVING_UP)
		dy = -1;
	if (cache.player.moving & MOVING_DOWN)
		dy = 1;
	if (cache.player.moving & MOVING_LEFT)
		dx = -1;
	if (cache.player.moving & MOVING_RIGHT)
		dx = 1;

	/* Move the player and adjust view if needed. */
	if (dx > 0)
		move_right(delta);
	else if (dx < 0)
		move_left(delta);

	if (dy > 0)
		move_down(delta);
	else if (dy < 0)
		move_up(delta);

	walksprite_update(&cache.player.ws, ticks);
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
	struct debug_report report = {0};

	map_draw(&map_state_data.map.map, VIEW()->x, VIEW()->y);
	walksprite_draw(
		&cache.player.ws,
		PLAYER()->angle,
		PLAYER()->x - VIEW()->x,
		PLAYER()->y - VIEW()->y);

	debug_printf(&report, "position: %d, %d", PLAYER()->x,
	    PLAYER()->y);
	debug_printf(&report, "view: %d, %d", VIEW()->x,
	    VIEW()->y);
}

struct map_state_data map_state_data;

struct state map_state = {
	.enter = enter,
	.leave = leave,
	.update = update,
	.handle = handle,
	.draw = draw
};
