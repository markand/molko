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
#include <core/maths.h>
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
#define SPEED 220
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
	map->margin_w = map->view_w - (MARGIN_WIDTH * 2) - map->player_sprite->cellw;
	map->margin_h = map->view_h - (MARGIN_HEIGHT * 2) - map->player_sprite->cellh;

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
	case KEY_TAB:
		map->flags ^= MAP_FLAGS_SHOW_GRID | MAP_FLAGS_SHOW_COLLIDE;
		map_repaint(map);
		break;
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

static int
cmp_tile(const struct map_tile *t1, const struct map_tile *t2)
{
	if (t1->id < t2->id)
		return -1;
	if (t1->id > t2->id)
		return 1;

	return 0;
}

static struct map_tile *
find_tile_by_id(const struct map *map, unsigned short id)
{
	typedef int (*cmp)(const void *, const void *);

	const struct map_tile key = {
		.id = id
	};

	return bsearch(&key, map->tiles, map->tilesz, sizeof (struct map_tile), (cmp)cmp_tile);
}

static struct map_tile *
find_tile_in_layer(const struct map *map, const struct map_layer *layer, int pr, int pc)
{
	unsigned short id;

	if ((id = layer->tiles[pc + pr * map->w]) == 0)
		return NULL;

	return find_tile_by_id(map, id - 1);
}

static bool
can_be_used(const struct map *map, const struct map_tile *block, int drow, int dcol)
{
	if (drow) {
		/* Object outside of left-right bounds. */
		if (block->x + (int)block->w <= map->player_x ||
		    block->x                 >= map->player_x + (int)map->player_sprite->cellw)
			return false;

		if ((drow < 0 && block->y            >= map->player_y + (int)map->player_sprite->cellh) ||
		    (drow > 0 && block->y + block->h <= map->player_y + (int)map->player_sprite->cellh))
			return false;
	} else if (dcol) {
		/* Object outside of up-down bounds. */
		if (block->y + (int)block->h <= map->player_y ||
		    block->y                 >= map->player_y + (int)map->player_sprite->cellh)
			return false;

		if ((dcol < 0 && block->x            >= map->player_x + (int)map->player_sprite->cellw) ||
		    (dcol > 0 && block->x + block->w <= map->player_x + (int)map->player_sprite->cellw))
			return false;
	}

	return true;
}

static bool
find_tile_absolute(const struct map *map, struct map_tile *block, int row, int col, int drow, int dcol)
{
	struct map_tile *tile, tmp;

	if (row < 0 || (unsigned int)row >= map->h || col < 0 || (unsigned int) col >= map->w)
		return false;

	if (!(tile = find_tile_in_layer(map, &map->layers[1], row, col)))
		tile = find_tile_in_layer(map, &map->layers[0], row, col);

	if (!tile)
		return false;

	/* Convert to absolute values. */
	tmp.id = tile->id;
	tmp.x = tile->x + col * map->tileset->cellw;
	tmp.y = tile->y + row * map->tileset->cellh;
	tmp.w = tile->w;
	tmp.h = tile->h;

	/* This tile is not usable */
	if (!tile || !can_be_used(map, &tmp, drow, dcol))
		return false;

	memcpy(block, &tmp, sizeof (tmp));

	return true;
}

static void
find_block_y(const struct map *map, struct map_tile *block, int pr, int pc, int drow)
{
	int ncols = map->player_sprite->cellw / map->tileset->cellw;
	int nrows = map->player_sprite->cellh / map->tileset->cellh;
	int start, end;

	if (drow < 0) {
		start = 0;
		end = pr;
		block->x = block->y = block->h = 0;
		block->w = map->real_w;
	} else {
		start = pr + nrows;
		end = map->h;
		block->x = block->h = 0;
		block->y = map->real_h;
		block->w = map->real_w;
	}

	for (int r = start; r <= end; ++r) {
		for (int c = 0; c <= ncols; ++c) {
			struct map_tile tmp;

			if (!find_tile_absolute(map, &tmp, r, pc + c, drow, 0))
				continue;
			if ((drow < 0 && tmp.y + tmp.h > block->y + block->h) ||
			    (drow > 0 && tmp.y < block->y))
				memcpy(block, &tmp, sizeof (tmp));
		}
	}
}

static void
find_block_x(const struct map *map, struct map_tile *block, int pr, int pc, int dcol)
{
	int ncols = map->player_sprite->cellw / map->tileset->cellw;
	int nrows = map->player_sprite->cellh / map->tileset->cellh;
	int start, end;

	if (dcol < 0) {
		start = 0;
		end = pc;
		block->x = block->y = block->w = 0;
		block->h = map->real_h;
	} else {
		start = pc + ncols;
		end = map->w;
		block->x = map->real_w;
		block->y = block->w = 0;
		block->h = block->h;
	}

	for (int c = start; c <= end; ++c) {
		for (int r = 0; r <= nrows; ++r) {
			struct map_tile tmp;

			if (!find_tile_absolute(map, &tmp, pr + r, c, 0, dcol))
				continue;
			if ((dcol < 0 && tmp.x + tmp.w > block->x + block->w) ||
			    (dcol > 0 && tmp.x < block->x))
				memcpy(block, &tmp, sizeof (tmp));
		}
	}
}

/*
 * Fill dimensions of the adjacent tile from the current player position. The
 * arguments drow/dcolumn indicate the desired movement (e.g. -1, +1) but only
 * one at a time must be set.
 *
 * The argument block will be set to a rectangle that will collide with the
 * player according to its position and destination. It is never null because
 * if no tile definition is found it is set to the screen edges.
 */
static void
find_tile_collision(const struct map *map, struct map_tile *block, int drow, int dcol)
{
	assert((drow != 0 && dcol == 0) || (drow == 0 && dcol != 0));

	const int pcol = map->player_x / map->tileset->cellw;
	const int prow = map->player_y / map->tileset->cellh;

	if (drow)
		find_block_y(map, block, prow, pcol, drow);
	else if (dcol)
		find_block_x(map, block, prow, pcol, dcol);
}

static void
move_x(struct map *map, int delta)
{
	struct map_tile block;

	find_tile_collision(map, &block, 0, delta < 0 ? -1 : +1);

	if (delta < 0 && map->player_x + delta < (int)(block.x + block.w))
		delta = map->player_x - block.x - block.w;
	else if (delta > 0 && (int)(map->player_x + map->player_sprite->cellw + delta) >= block.x)
		delta = block.x - map->player_x - (int)(map->player_sprite->cellw);

	map->player_x += delta;

	if (map->player_x < map->margin_x || map->player_x >= (int)(map->margin_x + map->margin_w))
		map->view_x += delta;

	if (map->view_x < 0)
		map->view_x = 0;
	else if (map->view_x >= (int)(map->real_w - map->view_w))
		map->view_x = map->real_w - map->view_w;
}

static void
move_y(struct map *map, int delta)
{
	struct map_tile block;

	find_tile_collision(map, &block, delta < 0 ? -1 : +1, 0);

	if (delta < 0 && map->player_y + delta < (int)(block.y + block.h))
		delta = map->player_y - block.y - block.h;
	else if (delta > 0 && (int)(map->player_y + map->player_sprite->cellh + delta) >= block.y)
		delta = block.y - map->player_y - (int)(map->player_sprite->cellh);

	map->player_y += delta;

	if (map->player_y < map->margin_y || map->player_y >= (int)(map->margin_y + map->margin_h))
		map->view_y += delta;

	if (map->view_y < 0)
		map->view_y = 0;
	else if (map->view_y >= (int)(map->real_h - map->view_h))
		map->view_y = map->real_h - map->view_h;
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
	if (dx)
		move_x(map, dx * delta);
	if (dy)
		move_y(map, dy * delta);

	walksprite_update(&map->player_ws, ticks);
}

static void
draw_layer(struct map *map, const struct map_layer *layer)
{
	assert(map);
	assert(layer);

	struct texture colbox = {0};
	const size_t ntiles = map->w * map->h;

	/* Show collision box if requested. */
	if (map->flags & MAP_FLAGS_SHOW_COLLIDE && texture_new(&colbox, 16, 16)) {
		texture_set_blend_mode(&colbox, TEXTURE_BLEND_BLEND);
		texture_set_alpha_mod(&colbox, 100);
		PAINTER_BEGIN(&colbox);
		painter_set_color(0xa53030ff);
		painter_clear();
		PAINTER_END();
	}

	for (size_t i = 0; i < ntiles; ++i) {
		const struct map_tile *tile;
		int mx, my, mr, mc, sr, sc, id;

		if (layer->tiles[i] == 0)
			continue;

		id = layer->tiles[i] - 1;

		/* Map row/column. */
		mc = i % map->w;
		mr = i / map->w;

		/* Map row/column real positions. */
		mx = mc * map->tileset->cellw;
		my = mr * map->tileset->cellh;

		/* Sprite row/column. */
		sc = (id) % map->tileset->ncols;
		sr = (id) / map->tileset->ncols;

		sprite_draw(map->tileset, sr, sc, mx, my);

		if ((tile = find_tile_by_id(map, id)) && texture_ok(&colbox))
			texture_scale(&colbox, 0, 0, 5, 5, mx + tile->x, my + tile->y, tile->w, tile->h, 0);

		if (map->flags & MAP_FLAGS_SHOW_GRID) {
			painter_set_color(0x202e37ff);
			painter_draw_line(mx, my, mx + map->tileset->cellw, my);
			painter_draw_line(mx + map->tileset->cellw - 1, my, mx + map->tileset->cellw - 1, my + map->tileset->cellh);
		}
	}

	texture_finish(&colbox);
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
	map_draw(state->data);
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

	struct texture box = {0};

	/* Draw the texture about background/foreground. */
	texture_scale(&map->picture, map->view_x, map->view_y, window.w, window.h,
	    0, 0, window.w, window.h, 0.0);

	walksprite_draw(
		&map->player_ws,
		map->player_angle,
		map->player_x - map->view_x,
		map->player_y - map->view_y);

	/* Draw collide box around player if requested. */
	if (map->flags & MAP_FLAGS_SHOW_COLLIDE &&
	    texture_new(&box, map->player_sprite->cellw, map->player_sprite->cellh)) {
		texture_set_alpha_mod(&box, 100);
		texture_set_blend_mode(&box, TEXTURE_BLEND_BLEND);
		PAINTER_BEGIN(&box);
		painter_set_color(0x4f8fbaff);
		painter_clear();
		PAINTER_END();
		texture_draw(&box, map->player_x - map->view_x, map->player_y - map->view_y);
		texture_finish(&box);
	}
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
