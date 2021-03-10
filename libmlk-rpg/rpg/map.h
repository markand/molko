/*
 * map.h -- game map
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

#ifndef MOLKO_RPG_MAP_H
#define MOLKO_RPG_MAP_H

#include <stddef.h>

#include <core/action.h>
#include <core/core.h>

#include "walksprite.h"

struct tileset;

union event;

enum map_layer_type {
	MAP_LAYER_TYPE_BACKGROUND,
	MAP_LAYER_TYPE_FOREGROUND,
	MAP_LAYER_TYPE_ABOVE,
	MAP_LAYER_TYPE_NUM
};

struct map_layer {
	unsigned short *tiles;
};

enum map_flags {
	MAP_FLAGS_NONE          = 0,
	MAP_FLAGS_SHOW_GRID     = (1 << 0),
	MAP_FLAGS_SHOW_COLLIDE  = (1 << 2)
};

struct map_block {
	int x;
	int y;
	unsigned int w;
	unsigned int h;
};

struct map {
	const char *title;              /*!< (+) Map title name. */
	unsigned int columns;           /*!< (-) Number of columns. */
	unsigned int rows;              /*!< (-) Number of rows. */

	/* Tileset. */
	struct tileset *tileset;        /*!< (+&?) Tileset to use. */

	/* View options. */
	enum map_flags flags;           /*!< (+) View options. */

	/* Extra collisions blocks. */
	struct map_block *blocks;       /*!< (+&?) Extra collisions. */
	size_t blocksz;                 /*!< (+) Number of collisions. */

	/* List of actions. */
	struct action_stack astack_par; /*!< (+) Parallel actions. */
	struct action_stack astack_seq; /*!< (+) Blocking actions. */

	/* Player. */
	struct sprite *player_sprite;   /*!< (+) The sprite to use */
	struct walksprite player_ws;    /*!< (-) Walking sprite for moving the player. */
	int player_x;                   /*!< (+) Player position in x */
	int player_y;                   /*!< (+) Player position in y */
	int player_angle;               /*!< (+) Player angle (see walksprite) */
	unsigned int player_movement;   /*!< (*) Current player movements. */

	/* View to zoom/locate. */
	int view_x;                     /*!< (+) Position in x */
	int view_y;                     /*!< (+) Position in y */
	unsigned int view_w;            /*!< (+) View width */
	unsigned int view_h;            /*!< (+) View height */

	/* View margin. */
	int margin_x;                   /*!< (+) View margin in x. */
	int margin_y;                   /*!< (+) View margin in y. */
	unsigned int margin_w;          /*!< (+) Margin width. */
	unsigned int margin_h;          /*!< (+) Margin height. */

	/* Different tile layers. */
	struct map_layer layers[MAP_LAYER_TYPE_NUM];
};

CORE_BEGIN_DECLS

int
map_init(struct map *map);

void
map_handle(struct map *map, const union event *ev);

void
map_update(struct map *map, unsigned int ticks);

void
map_draw(const struct map *map);

void
map_finish(struct map *map);

CORE_END_DECLS

#endif /* !MOLKO_RPG_MAP_H */
