/*
 * map.c -- game map
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
#include <stdlib.h>
#include <string.h>

#include <core/error.h>
#include <core/event.h>
#include <core/image.h>
#include <core/painter.h>
#include <core/sprite.h>
#include <core/state.h>
#include <core/sys.h>
#include <core/texture.h>
#include <core/window.h>

#include <ui/debug.h>

#include "map.h"

/*
 * This is the speed the player moves on the map.
 *
 * SPEED represents the number of pixels it must move per SEC.
 * SEC simply represends the number of milliseconds in one second.
 */
#define SPEED 200
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

static void
center(struct map *map)
{
	map->view_x = map->player_x - (int)(map->view_w / 2);
	map->view_y = map->player_y - (int)(map->view_h / 2);

	if (map->view_x < 0)
		map->view_x = 0;
	else if ((unsigned int)map->view_x > map->real_w - map->view_w)
		map->view_x = map->real_w - map->view_w;

	if (map->view_y < 0)
		map->view_y = 0;
	else if ((unsigned int)map->view_y > map->real_h - map->view_h)
		map->view_y = map->real_h - map->view_h;
}

static void
start(struct map *map)
{
	map_repaint(map);

	/* Adjust view. */
	map->view_w = window.w;
	map->view_h = window.h;

	/* Adjust margin. */
	map->margin_w = map->view_w - (MARGIN_WIDTH * 2);
	map->margin_h = map->view_h - (MARGIN_HEIGHT * 2);

	/* Center the view by default. */
	center(map);

	/* Final bits. */
	walksprite_init(&map->player_ws, map->player_sprite, 150);
}

static void
handle_keydown(struct map *map, const union event *event)
{
	switch (event->key.key) {
	case KEY_UP:
		map->player_movement |= MOVING_UP;
		break;
	case KEY_RIGHT:
		map->player_movement |= MOVING_RIGHT;
		break;
	case KEY_DOWN:
		map->player_movement |= MOVING_DOWN;
		break;
	case KEY_LEFT:
		map->player_movement |= MOVING_LEFT;
		break;
	default:
		break;
	}

	map->player_angle = orientations[map->player_movement];
}

static void
handle_keyup(struct map *map, const union event *event)
{
	switch (event->key.key) {
	case KEY_UP:
		map->player_movement &= ~(MOVING_UP);
		break;
	case KEY_RIGHT:
		map->player_movement &= ~(MOVING_RIGHT);
		break;
	case KEY_DOWN:
		map->player_movement &= ~(MOVING_DOWN);
		break;
	case KEY_LEFT:
		map->player_movement &= ~(MOVING_LEFT);
		break;
	default:
		break;
	}
}

static void
move_up(struct map *map, int delta)
{
	map->player_y -= delta;

	if (map->player_y < map->margin_y) {
		map->view_y = map->player_y - MARGIN_HEIGHT;

		if (map->view_y < 0)
			map->view_y = 0;
	}

	if (map->player_y < 0)
		map->player_y = 0;
}

static void
move_right(struct map *map, int delta)
{
	if (map->player_x + map->player_sprite->cellw + delta > map->real_w)
		map->player_x = map->real_w - map->player_sprite->cellw;
	else
		map->player_x += delta;

	if (map->player_x + map->player_sprite->cellw > map->margin_x + map->margin_w) {
		map->view_x += delta;

		if (map->view_x + map->view_w > map->real_w)
			map->view_x = map->real_w - map->view_w;
	}
}

static void
move_down(struct map *map, int delta)
{
	if (map->player_y + map->player_sprite->cellh + delta > map->real_h)
		map->player_y = map->real_h - map->player_sprite->cellh;
	else
		map->player_y += delta;

	if (map->player_y + map->player_sprite->cellh > map->margin_y + map->margin_h) {
		map->view_y += delta;

		if (map->view_y + map->view_h > map->real_h)
			map->view_y = map->real_h - map->view_h;
	}
}

static void
move_left(struct map *map, int delta)
{
	if (map->player_x < delta)
		map->player_x = 0;
	else
		map->player_x -= delta;

	if (map->player_x < map->margin_x) {
		if (map->view_x < delta)
			map->view_x = 0;
		else
			map->view_x -= delta;
	}
}

static void
move(struct map *map, unsigned int ticks)
{
	/* This is the amount of pixels the player must move. */
	const int delta = SPEED * ticks / SEC;

	/* This is the rectangle within the view where users must be. */
	map->margin_x = map->view_x + MARGIN_WIDTH;
	map->margin_y = map->view_y + MARGIN_HEIGHT;

	int dx = 0;
	int dy = 0;

	if (map->player_movement == 0)
		return;

	if (map->player_movement & MOVING_UP)
		dy = -1;
	if (map->player_movement & MOVING_DOWN)
		dy = 1;
	if (map->player_movement & MOVING_LEFT)
		dx = -1;
	if (map->player_movement & MOVING_RIGHT)
		dx = 1;

	/* Move the player and adjust view if needed. */
	if (dx > 0)
		move_right(map, delta);
	else if (dx < 0)
		move_left(map, delta);

	if (dy > 0)
		move_down(map, delta);
	else if (dy < 0)
		move_up(map, delta);

	walksprite_update(&map->player_ws, ticks);
}

static void
draw_layer(struct map *map, const struct map_layer *layer)
{
	assert(map);
	assert(layer);

	int x = 0, y = 0;

	for (unsigned int r = 0; r < map->w; ++r) {
		for (unsigned int c = 0; c < map->h; ++c) {
			unsigned int si = r * map->w + c;
			unsigned int sr = (layer->tiles[si] - 1) / map->tileset->ncols;
			unsigned int sc = (layer->tiles[si] - 1) % map->tileset->nrows;

			if (layer->tiles[si] != 0)
				sprite_draw(map->tileset, sr, sc, x, y);

			x += map->tile_w;
		}

		x = 0;
		y += map->tile_h;
	}
}

static void
handle(struct state *state, const union event *ev)
{
	map_handle(state->data, ev);
}

static void
update(struct state *state, unsigned int ticks)
{
	map_update(state->data, ticks);
}

static void
draw(struct state *state)
{
	return map_draw(state->data);
}

bool
map_init(struct map *map)
{
	assert(map);

	if (!texture_new(&map->picture, map->real_w, map->real_h))
		return false;

	start(map);

	return true;
}

void
map_handle(struct map *map, const union event *ev)
{
	assert(map);
	assert(ev);

	switch (ev->type) {
	case EVENT_KEYDOWN:
		handle_keydown(map, ev);
		break;
	case EVENT_KEYUP:
		handle_keyup(map, ev);
		break;
	default:
		break;
	}
}

void
map_update(struct map *map, unsigned int ticks)
{
	assert(map);

	move(map, ticks);
}

void
map_draw(const struct map *map)
{
	assert(map);

	struct debug_report report = {0};

	/* Draw the texture about background/foreground. */
	texture_scale(&map->picture, map->view_x, map->view_y, window.w, window.h,
	    0, 0, window.w, window.h, 0.0);

	walksprite_draw(
		&map->player_ws,
		map->player_angle,
		map->player_x - map->view_x,
		map->player_y - map->view_y);

	debugf(&report, "position: %d, %d", map->player_x,
	    map->player_y);
	debugf(&report, "view: %d, %d", map->view_x,
	    map->view_y);
}

void
map_repaint(struct map *map)
{
	assert(map);

	PAINTER_BEGIN(&map->picture);
	draw_layer(map, &map->layers[MAP_LAYER_TYPE_BACKGROUND]);
	draw_layer(map, &map->layers[MAP_LAYER_TYPE_FOREGROUND]);
	PAINTER_END();
}

void
map_state(struct map *map, struct state *state)
{
	assert(map);
	assert(state);

	memset(state, 0, sizeof (*state));
	state->data = map;
	state->draw = draw;
	state->handle = handle;
	state->update = update;
}

void
map_finish(struct map *map)
{
	assert(map);

	texture_finish(&map->picture);

	memset(map, 0, sizeof (*map));
}

