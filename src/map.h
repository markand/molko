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

#include <stdbool.h>

#include "sprite.h"

/**
 * \brief Max title length for a map.
 */
#define MAP_TITLE_MAX   32

struct texture;

/**
 * \brief Map layer.
 */
struct map_layer {
	unsigned short *tiles;          /*!< (RO) Array of tiles, depending on the map size. */
};

/**
 * \brief Map object.
 *
 * This structure only defines the map characteristics. It does not have any
 * logic and is left for game state.
 */
struct map {
	char title[MAP_TITLE_MAX];      /*!< (RW) The map title */
	struct texture *tileset;        /*!< (RW) Tileset to use */
	struct texture *picture;        /*!< (RO) Map drawn into a picture */
	struct sprite sprite;           /*!< (RO) Sprite to render */
	int origin_x;                   /*!< (RO) Where the player starts in X */
	int origin_y;                   /*!< (RO) Where the player starts in Y */
	unsigned int width;             /*!< (RO) Map width in cells */
	unsigned int height;            /*!< (RO) Map height in cells */
	unsigned short tilewidth;       /*!< (RO) Pixels per cell (width) */
	unsigned short tileheight;      /*!< (RO) Pixels per cell (height) */
	struct map_layer layers[2];     /*!< (RO) Layers (background, foreground) */
};

/**
 * Open a map.
 *
 * \pre map != NULL
 * \pre path != NULL
 * \param map the map to fill
 * \param path the path to the map
 * \return true if successfully loaded
 */
bool
map_open(struct map *map, const char *path);

/**
 * Render a map.
 *
 * \pre map != NULL
 * \param map the map to render
 * \param srcx the x coordinate region
 * \param srcy the y coordinate region
 */
void
map_draw(struct map *map, int srcx, int srcy);

/**
 * Force map repaint on its texture.
 *
 * \pre map != NULL
 * \param map the map to repaint
 * \warning This function does not render anything on the screen
 */
void
map_repaint(struct map *map);

/**
 * Close the map and its resources.
 *
 * \pre map != NULL
 * \param map the map to render
 */
void
map_close(struct map *map);

#endif /* !MOLKO_MAP_H */
