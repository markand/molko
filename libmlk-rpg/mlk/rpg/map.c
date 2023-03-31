/*
 * map.c -- game map
 *
 * Copyright (c) 2020-2023 David Demelier <markand@malikania.fr>
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

#include <mlk/core/event.h>
#include <mlk/core/image.h>
#include <mlk/core/maths.h>
#include <mlk/core/painter.h>
#include <mlk/core/sprite.h>
#include <mlk/core/sys.h>
#include <mlk/core/texture.h>
#include <mlk/core/window.h>

#include <mlk/ui/debug.h>

#include "map.h"
#include "tileset.h"

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
#define MARGIN_WIDTH    160
#define MARGIN_HEIGHT   90

#define WIDTH(map)      ((map)->columns * (map)->tileset->sprite->cellw)
#define HEIGHT(map)     ((map)->rows * (map)->tileset->sprite->cellh)

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
 * Check if this block is usable for collision detection. For example if the
 * player is moving upwards but the collision shape is below it is unnecessary
 * to check.
 */
static int
is_block_relevant(const struct mlk_map *map,
                    const struct mlk_map_block *block,
                    int drow,
                    int dcol)
{
	if (drow) {
		/* Object outside of left-right bounds. */
		if (block->x + (int)block->w <= map->player_x ||
		    block->x                 >= map->player_x + (int)map->player_sprite->cellw)
			return 0;

		if ((drow < 0 && block->y            >= map->player_y + (int)map->player_sprite->cellh) ||
		    (drow > 0 && block->y + block->h <= map->player_y + map->player_sprite->cellh))
			return 0;
	} else if (dcol) {
		/* Object outside of up-down bounds. */
		if (block->y + (int)block->h <= map->player_y ||
		    block->y                 >= map->player_y + (int)map->player_sprite->cellh)
			return 0;

		if ((dcol < 0 && block->x            >= map->player_x + (int)map->player_sprite->cellw) ||
		    (dcol > 0 && block->x + block->w <= map->player_x + map->player_sprite->cellw))
			return 0;
	}

	return 1;
}

/*
 * Determine if this collision shape is "closer" to the player by checking the
 * new block coordinates with the previous one.
 */
static int
is_block_better(const struct mlk_map_block *now,
                const struct mlk_map_block *new,
                int drow,
                int dcol)
{
	return ((drow < 0 && new->y + new->h > now->y + now->h) ||
	        (drow > 0 && new->y < now->y) ||
	        (dcol < 0 && new->x + new->w > now->x + now->w) ||
		(dcol > 0 && new->x < now->x));

}

static void
center(struct mlk_map *map)
{
	map->view_x = map->player_x - (int)(map->view_w / 2);
	map->view_y = map->player_y - (int)(map->view_h / 2);

	if (map->view_x < 0)
		map->view_x = 0;
	else if ((unsigned int)map->view_x > WIDTH(map) - map->view_w)
		map->view_x = WIDTH(map) - map->view_w;

	if (map->view_y < 0)
		map->view_y = 0;
	else if ((unsigned int)map->view_y > HEIGHT(map) - map->view_h)
		map->view_y = HEIGHT(map) - map->view_h;
}

static void
init(struct mlk_map *map)
{
	/* Adjust view. */
	map->view_w = mlk_window.w;
	map->view_h = mlk_window.h;

	/* Adjust margin. */
	map->margin_w = map->view_w - (MARGIN_WIDTH * 2) - map->player_sprite->cellw;
	map->margin_h = map->view_h - (MARGIN_HEIGHT * 2) - map->player_sprite->cellh;

	/* Center the view by default. */
	center(map);

	/* Final bits. */
	map->player_ws.sprite = map->player_sprite;
	map->player_ws.delay = 150;
	mlk_walksprite_init(&map->player_ws);
}

static void
handle_keydown(struct mlk_map *map, const union mlk_event *event)
{
	switch (event->key.key) {
	case MLK_KEY_UP:
		map->player_movement |= MOVING_UP;
		break;
	case MLK_KEY_RIGHT:
		map->player_movement |= MOVING_RIGHT;
		break;
	case MLK_KEY_DOWN:
		map->player_movement |= MOVING_DOWN;
		break;
	case MLK_KEY_LEFT:
		map->player_movement |= MOVING_LEFT;
		break;
	default:
		break;
	}

	map->player_a = orientations[map->player_movement];
}

static void
handle_keyup(struct mlk_map *map, const union mlk_event *event)
{
	switch (event->key.key) {
	case MLK_KEY_F12:
		map->flags ^= MLK_MAP_FLAGS_SHOW_GRID | MLK_MAP_FLAGS_SHOW_COLLIDE;
		break;
	case MLK_KEY_UP:
		map->player_movement &= ~(MOVING_UP);
		break;
	case MLK_KEY_RIGHT:
		map->player_movement &= ~(MOVING_RIGHT);
		break;
	case MLK_KEY_DOWN:
		map->player_movement &= ~(MOVING_DOWN);
		break;
	case MLK_KEY_LEFT:
		map->player_movement &= ~(MOVING_LEFT);
		break;
	default:
		break;
	}
}

// TODO: merge this code in tileset maybe.
static int
collision_cmp(const void *d1, const void *d2)
{
	const struct mlk_tileset_collision *c1 = d1;
	const struct mlk_tileset_collision *c2 = d2;

	if (c1->id < c2->id)
		return -1;
	if (c1->id > c2->id)
		return 1;

	return 0;
}

static inline struct mlk_tileset_collision *
find_collision_by_id(const struct mlk_map *map, unsigned int id)
{
	const struct mlk_tileset_collision key = {
		.id = id
	};

	return bsearch(&key, map->tileset->collisions, map->tileset->collisionsz,
	    sizeof (key), collision_cmp);
}

static struct mlk_tileset_collision *
find_collision_by_row_column_in_layer(const struct mlk_map *map,
                                    const struct mlk_map_layer *layer,
                                    int row,
                                    int col)
{
	unsigned int id;

	if (row < 0 || (unsigned int)row >= map->rows ||
	    col < 0 || (unsigned int)col >= map->columns)
		return 0;

	if ((id = layer->tiles[col + row * map->columns]) == 0)
		return NULL;

	return find_collision_by_id(map, id - 1);
}

static struct mlk_tileset_collision *
find_collision_by_row_column(const struct mlk_map *map, int row, int col)
{
	struct mlk_tileset_collision *tc;

	/* TODO: probably a for loop when we have indefinite layers. */
	if (!(tc = find_collision_by_row_column_in_layer(map, &map->layers[1], row, col)))
		tc = find_collision_by_row_column_in_layer(map, &map->layers[0], row, col);

	return tc;
}

static void
find_block_iterate(const struct mlk_map *map,
                   struct mlk_map_block *block,
                   int rowstart,
                   int rowend,
                   int colstart,
                   int colend,
                   int drow,
                   int dcol)
{
	assert(map);
	assert(block);

	const struct mlk_tileset_collision *tc;
	const struct mlk_map_block *b;
	struct mlk_map_block tmp;

	/* First, check with tiledefs. */
	for (int r = rowstart; r <= rowend; ++r) {
		for (int c = colstart; c <= colend; ++c) {
			if (!(tc = find_collision_by_row_column(map, r, c)))
				continue;

			/* Convert to absolute values. */
			tmp.x = tc->x + c * map->tileset->sprite->cellw;
			tmp.y = tc->y + r * map->tileset->sprite->cellh;
			tmp.w = tc->w;
			tmp.h = tc->h;

			/* This tiledef is out of context. */
			if (!is_block_relevant(map, &tmp, drow, dcol))
				continue;

			if (is_block_better(block, &tmp, drow, dcol)) {
				block->x = tmp.x;
				block->y = tmp.y;
				block->w = tmp.w;
				block->h = tmp.h;
			}
		}
	}

	/* Now check if there are objects closer than tiledefs. */
	for (size_t i = 0; i < map->blocksz; ++i) {
		b = &map->blocks[i];

		if (is_block_relevant(map, b, drow, dcol) &&
		    is_block_better(block, b, drow, dcol)) {
			block->x = b->x;
			block->y = b->y;
			block->w = b->w;
			block->h = b->h;
		}
	}
}

static void
find_collision(const struct mlk_map *map, struct mlk_map_block *block, int drow, int dcolumn)
{
	assert((drow && !dcolumn) || (dcolumn && !drow));

	const int playercol = map->player_x / map->tileset->sprite->cellw;
	const int playerrow = map->player_y / map->tileset->sprite->cellh;
	const int ncols = (map->player_sprite->cellw / map->tileset->sprite->cellw) + 1;
	const int nrows = (map->player_sprite->cellh / map->tileset->sprite->cellh) + 1;
	int rowstart, rowend, colstart, colend;

	if (drow) {
		colstart = playercol;
		colend = playercol + ncols;

		if (drow < 0) {
			/* Moving UP. */
			rowstart = 0;
			rowend = playerrow;
			block->x = block->y = block->h = 0;
			block->w = WIDTH(map);
		} else {
			/* Moving DOWN. */
			rowstart = playerrow;
			rowend = HEIGHT(map);
			block->x = block->h = 0;
			block->y = HEIGHT(map);
			block->w = WIDTH(map);
		}
	} else {
		rowstart = playerrow;
		rowend = playerrow + nrows;

		if (dcolumn < 0) {
			/* Moving LEFT. */
			colstart = 0;
			colend = playercol;
			block->x = block->y = block->w = 0;
			block->h = HEIGHT(map);
		} else {
			/* Moving RIGHT. */
			colstart = playercol;
			colend = WIDTH(map);
			block->x = WIDTH(map);
			block->y = block->w = 0;
			block->h = block->h;
		}
	}

	find_block_iterate(map, block, rowstart, rowend, colstart, colend, drow, dcolumn);
}

static void
move_x(struct mlk_map *map, int delta)
{
	struct mlk_map_block block;

	find_collision(map, &block, 0, delta < 0 ? -1 : +1);

	if (delta < 0 && map->player_x + delta < (int)(block.x + block.w))
		delta = map->player_x - block.x - block.w;
	else if (delta > 0 && (int)(map->player_x + map->player_sprite->cellw + delta) >= block.x)
		delta = block.x - map->player_x - (int)(map->player_sprite->cellw);

	map->player_x += delta;

	if ((delta < 0 && map->player_x < map->margin_x) ||
	    (delta > 0 && map->player_x >= (int)(map->margin_x + map->margin_w)))
		map->view_x += delta;

	if (map->view_x < 0)
		map->view_x = 0;
	else if (map->view_x >= (int)(WIDTH(map) - map->view_w))
		map->view_x = WIDTH(map) - map->view_w;
}

static void
move_y(struct mlk_map *map, int delta)
{
	struct mlk_map_block block;

	find_collision(map, &block, delta < 0 ? -1 : +1, 0);

	if (delta < 0 && map->player_y + delta < (int)(block.y + block.h))
		delta = map->player_y - block.y - block.h;
	else if (delta > 0 && (int)(map->player_y + map->player_sprite->cellh + delta) >= block.y)
		delta = block.y - map->player_y - (int)(map->player_sprite->cellh);

	map->player_y += delta;

	if ((delta < 0 && map->player_y < map->margin_y) ||
	    (delta > 0 && map->player_y >= (int)(map->margin_y + map->margin_h)))
		map->view_y += delta;

	if (map->view_y < 0)
		map->view_y = 0;
	else if (map->view_y >= (int)(HEIGHT(map) - map->view_h))
		map->view_y = HEIGHT(map) - map->view_h;
}

static void
move(struct mlk_map *map, unsigned int ticks)
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

	mlk_walksprite_update(&map->player_ws, ticks);
}

static inline void
draw_layer_tile(const struct mlk_map *map,
                const struct mlk_map_layer *layer,
                struct mlk_texture *colbox,
                int start_col,
                int start_row,
                int start_x,
                int start_y,
                unsigned int r,
                unsigned int c)
{
	const struct mlk_tileset_collision *tc;
	int index, id, sc, sr, mx, my;

	index = (start_col + c) + ((start_row + r) * map->columns);

	if ((id = layer->tiles[index]) == 0)
		return;

	id -= 1;

	/* Sprite row/column. */
	sc = (id) % map->tileset->sprite->ncols;
	sr = (id) / map->tileset->sprite->ncols;

	/* On screen coordinates. */
	mx = start_x + (int)c * (int)map->tileset->sprite->cellw;
	my = start_y + (int)r * (int)map->tileset->sprite->cellh;

	mlk_tileset_draw(map->tileset, sr, sc, mx, my);

	/* Draw collision box if colbox is non NULL. */
	if ((tc = find_collision_by_id(map, id)) && mlk_texture_ok(colbox))
		mlk_texture_scale(colbox, 0, 0, 5, 5, mx + tc->x, my + tc->y, tc->w, tc->h, 0);

	if (map->flags & MLK_MAP_FLAGS_SHOW_GRID) {
		mlk_painter_set_color(0x202e37ff);
		mlk_painter_draw_line(mx, my, mx + (int)map->tileset->sprite->cellw, my);
		mlk_painter_draw_line(
		    mx + (int)map->tileset->sprite->cellw - 1, my,
		    mx + (int)map->tileset->sprite->cellw - 1, my + (int)map->tileset->sprite->cellh);
	}
}

static void
draw_layer(const struct mlk_map *map, const struct mlk_map_layer *layer)
{
	assert(map);
	assert(layer);

	/* Beginning of view in row/column. */
	const unsigned int start_col = map->view_x / map->tileset->sprite->cellw;
	const unsigned int start_row = map->view_y / map->tileset->sprite->cellh;

	/* Convert into x/y coordinate. */
	const int start_x = 0 - (map->view_x % (int)map->tileset->sprite->cellw);
	const int start_y = 0 - (map->view_y % (int)map->tileset->sprite->cellh);

	/* Number of row/columns to draw starting from there. */
	const unsigned int ncols = (map->view_w / map->tileset->sprite->cellw) + 2;
	const unsigned int nrows = (map->view_h / map->tileset->sprite->cellh) + 2;

	struct mlk_texture colbox = {0};

	if (!layer->tiles)
		return;

	/* Show collision box if requested. */
	if (map->flags & MLK_MAP_FLAGS_SHOW_COLLIDE && mlk_texture_new(&colbox, 16, 16) == 0) {
		mlk_texture_set_blend_mode(&colbox, MLK_TEXTURE_BLEND_BLEND);
		mlk_texture_set_alpha_mod(&colbox, 100);
		MLK_PAINTER_BEGIN(&colbox);
		mlk_painter_set_color(0xa53030ff);
		mlk_painter_clear();
		MLK_PAINTER_END();
	}

	for (unsigned int r = 0; r < nrows; ++r) {
		for (unsigned int c = 0; c < ncols; ++c) {
			if (start_col + c >= map->columns ||
			    start_row + r >= map->rows)
				continue;

			draw_layer_tile(map, layer, &colbox, start_col, start_row, start_x, start_y, r, c);
		}
	}

	mlk_texture_finish(&colbox);
}

static void
draw_collide(const struct mlk_map *map)
{
	struct mlk_texture box = {0};

	if (map->flags & MLK_MAP_FLAGS_SHOW_COLLIDE && mlk_texture_new(&box, 64, 64) == 0) {
		/* Draw collide box around player if requested. */
		mlk_texture_set_alpha_mod(&box, 100);
		mlk_texture_set_blend_mode(&box, MLK_TEXTURE_BLEND_BLEND);
		MLK_PAINTER_BEGIN(&box);
		mlk_painter_set_color(0x4f8fbaff);
		mlk_painter_clear();
		MLK_PAINTER_END();
		mlk_texture_scale(&box, 0, 0, 64, 64,
		    map->player_x - map->view_x, map->player_y - map->view_y,
			      map->player_sprite->cellw, map->player_sprite->cellh, 0.f);

		/* Do the same for every objects. */
		MLK_PAINTER_BEGIN(&box);
		mlk_painter_set_color(0xa8ca58ff);
		mlk_painter_clear();
		MLK_PAINTER_END();

		for (size_t i = 0; i < map->blocksz; ++i) {
			mlk_texture_scale(&box, 0, 0, 64, 64,
			    map->blocks[i].x - map->view_x, map->blocks[i].y - map->view_y,
			    map->blocks[i].w, map->blocks[i].h,
			    0.f);
		}

		mlk_texture_finish(&box);
	}
}

int
mlk_map_init(struct mlk_map *map)
{
	assert(map);

	init(map);
	mlk_tileset_start(map->tileset);

	return 0;
}

void
mlk_map_handle(struct mlk_map *map, const union mlk_event *ev)
{
	assert(map);
	assert(ev);

	switch (ev->type) {
	case MLK_EVENT_KEYDOWN:
		handle_keydown(map, ev);
		break;
	case MLK_EVENT_KEYUP:
		handle_keyup(map, ev);
		break;
	default:
		break;
	}
}

void
mlk_map_update(struct mlk_map *map, unsigned int ticks)
{
	assert(map);

	mlk_tileset_update(map->tileset, ticks);
	move(map, ticks);
}

void
mlk_map_draw(const struct mlk_map *map)
{
	assert(map);

	/* Draw the texture about background/foreground. */
	draw_layer(map, &map->layers[MLK_MAP_LAYER_TYPE_BG]);
	draw_layer(map, &map->layers[MLK_MAP_LAYER_TYPE_FG]);

	mlk_walksprite_draw(
		&map->player_ws,
		map->player_a,
		map->player_x - map->view_x,
		map->player_y - map->view_y);

	draw_layer(map, &map->layers[MLK_MAP_LAYER_TYPE_ABOVE]);
	draw_collide(map);
}

void
mlk_map_finish(struct mlk_map *map)
{
	assert(map);
}
