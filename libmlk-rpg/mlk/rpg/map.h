/*
 * map.h -- game map
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

#ifndef MLK_RPG_MAP_H
#define MLK_RPG_MAP_H

#include <stddef.h>

#include "walksprite.h"

struct mlk_map;
struct mlk_tileset;

union mlk_event;

enum mlk_map_layer_type {
	MLK_MAP_LAYER_TYPE_BG,
	MLK_MAP_LAYER_TYPE_FG,
	MLK_MAP_LAYER_TYPE_ABOVE,
	MLK_MAP_LAYER_TYPE_LAST
};

struct mlk_map_layer {
	unsigned int *tiles;
};

enum mlk_map_flags {
	MLK_MAP_FLAGS_NONE              = 0,
	MLK_MAP_FLAGS_SHOW_GRID         = (1 << 0),
	MLK_MAP_FLAGS_SHOW_COLLIDE      = (1 << 2)
};

struct mlk_map_block {
	int x;
	int y;
	unsigned int w;
	unsigned int h;
};

struct mlk_map_style {
	unsigned long grid_color;
	unsigned long collision_color;
};

struct mlk_map_delegate {
	void *data;
	void (*update)(struct mlk_map_delegate *self, struct mlk_map *map, unsigned int ticks);
	void (*draw)(struct mlk_map_delegate *self, const struct mlk_map *map);
};

struct mlk_map {
	unsigned int columns;
	unsigned int rows;

	struct mlk_tileset *tileset;

	enum mlk_map_flags flags;

	const struct mlk_map_block *blocks;
	size_t blocksz;

	struct mlk_sprite *player_sprite;
	int player_x;
	int player_y;
	int player_a;
	unsigned int player_movement;
	struct mlk_walksprite player_ws;

	int view_x;
	int view_y;
	unsigned int view_w;
	unsigned int view_h;

	int margin_x;
	int margin_y;
	unsigned int margin_w;
	unsigned int margin_h;

	struct mlk_map_layer layers[MLK_MAP_LAYER_TYPE_LAST];

	struct mlk_button_style *style;
	struct mlk_button_delegate *delegate;
};

extern struct mlk_map_style mlk_map_style;
extern struct mlk_map_delegate mlk_map_delegate;

#if defined(__cplusplus)
extern "C" {
#endif

int
mlk_map_init(struct mlk_map *map);

void
mlk_map_handle(struct mlk_map *map, const union mlk_event *ev);

void
mlk_map_update(struct mlk_map *map, unsigned int ticks);

void
mlk_map_draw(const struct mlk_map *map);

void
mlk_map_finish(struct mlk_map *map);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_RPG_MAP_H */
