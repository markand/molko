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

#ifndef MOLKO_MAP_H
#define MOLKO_MAP_H

/**
 * \file map.h
 * \brief Game map.
 */

#include <stddef.h>

#include <core/action.h>

#include "walksprite.h"

struct tileset;

union event;

/**
 * \brief Map layer type.
 */
enum map_layer_type {
	MAP_LAYER_TYPE_BACKGROUND,      /*!< Background layer. */
	MAP_LAYER_TYPE_FOREGROUND,      /*!< Foreground layer. */
	MAP_LAYER_TYPE_ABOVE,           /*!< Above foreground layer. */
	MAP_LAYER_TYPE_NUM              /*!< Number of layers. */
};

/**
 * \brief Map layer.
 */
struct map_layer {
	unsigned short *tiles;          /*!< (+&) Array of tiles, depending on the map size. */
};

enum map_flags {
	MAP_FLAGS_NONE          = 0,            /*!< No flags. */
	MAP_FLAGS_SHOW_GRID     = (1 << 0),     /*!< Show grid pattern. */
	MAP_FLAGS_SHOW_COLLIDE  = (1 << 2)      /*!< Show collision layer. */
};

/**
 * \brief Map object.
 *
 * The map object is used to move a player within the map according to the
 * tilesets and collisions masks.
 *
 * By itself, a map does not know how to be loaded from a file and must be done
 * from an helper like \ref map_file.
 */
struct map {
	const char *title;              /*!< (+) Map title name. */
	unsigned int columns;           /*!< (-) Number of columns. */
	unsigned int rows;              /*!< (-) Number of rows. */

	/* Tileset. */
	struct tileset *tileset;        /*!< (+&?) Tileset to use. */

	/* View options. */
	enum map_flags flags;           /*!< (+) View options. */

	/* List of actions. */
	struct action_stack actions;    /*!< (+) */

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

	/**
	 * Different tile layers.
	 */
	struct map_layer layers[MAP_LAYER_TYPE_NUM];
};

/**
 * Initialize the map.
 *
 * This function will re-generate the terrain and center the view to the player
 * position.
 *
 * \pre map != NULL
 * \param map the map to initialize
 */
bool
map_init(struct map *map);

/**
 * Handle an event.
 *
 * \pre map != NULL
 * \pre ev != NULL
 * \param map the map
 * \param ev the event to handle
 */
void
map_handle(struct map *map, const union event *ev);

/**
 * Update the map.
 *
 * \pre map != NULL
 * \param map the map
 * \param ticks ellapsed milliseconds since last frame
 */
void
map_update(struct map *map, unsigned int ticks);

/**
 * Render a map.
 *
 * \pre map != NULL
 * \param map the map to render
 */
void
map_draw(const struct map *map);

/**
 * Dispose map resources.
 *
 * \pre map != NULL
 * \param map the map to close
 */
void
map_finish(struct map *map);

#endif /* !MOLKO_MAP_H */
