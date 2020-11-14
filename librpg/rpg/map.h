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

#include <core/texture.h>

#include "walksprite.h"

struct sprite;
struct state;

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
 * \brief Describe a tile in a tileset.
 */
struct map_tile {
	short id;                       /*!< (*) Tile index. */
	short x;                        /*!< (*) Collision region starts in y. */
	short y;                        /*!< (*) Collision region starts in y. */
	unsigned short w;               /*!< (*) Collision width. */
	unsigned short h;               /*!< (*) Collision height. */
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
	int origin_x;                   /*!< (+) Where the player starts in X. */
	int origin_y;                   /*!< (+) Where the player starts in Y. */
	unsigned int real_w;            /*!< (-) Real width in pixels. */
	unsigned int real_h;            /*!< (-) Real height in pixels. */
	unsigned int w;                 /*!< (-) Map width in cells. */
	unsigned int h;                 /*!< (-) Map height in cells. */
	unsigned short tile_w;          /*!< (-) Pixels per cell (width). */
	unsigned short tile_h;          /*!< (-) Pixels per cell (height). */
	struct sprite *tileset;         /*!< (+&) Tileset to use. */
	struct texture picture;         /*!< (-) Map drawn into a texture. */
	struct map_tile *tiles;         /*!< (+&?) Per tile properties (must be sorted). */
	size_t tilesz;                  /*!< (+) Number of tile properties. */

	/* View options. */
	enum map_flags flags;           /*!< (+) View options. */

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
 * Force map repaint on its texture.
 *
 * \pre map != NULL
 * \param map the map to repaint
 * \warning This function does not render anything on the screen.
 */
void
map_repaint(struct map *map);

/**
 * Convert the map into a game state.
 *
 * Both objects must exist until the state is no longer used.
 *
 * \pre map != NULL
 * \pre state != NULL
 * \param map the map to use
 * \param state the state to fill
 * \post state->data is set to map
 * \post state->handle is set
 * \post state->update is set
 * \post state->draw is set
 */
void
map_state(struct map *map, struct state *state);

/**
 * Dispose map resources.
 *
 * \pre map != NULL
 * \param map the map to close
 */
void
map_finish(struct map *map);

#endif /* !MOLKO_MAP_H */
