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
#include <stdio.h>

#include <core/texture.h>

/**
 * \brief Max title length for a map.
 */
#define MAP_TITLE_MAX   32

/**
 * \brief Max filename for tilesets.
 */
#define MAP_TILESET_MAX FILENAME_MAX

/**
 * \brief Map layer.
 */
struct map_layer {
	unsigned short *tiles;          /*!< (+) Array of tiles, depending on the map size. */
};

/**
 * \brief Map definition structure.
 *
 * This structure only defines the map characteristics. It does not have any
 * logic and is left for game state.
 */
struct map_data {
	char title[MAP_TITLE_MAX];      /*!< (+) The map title. */
	char tileset[MAP_TILESET_MAX];  /*!< (+) Name of tileset to use. */
	int origin_x;                   /*!< (+) Where the player starts in X. */
	int origin_y;                   /*!< (+) Where the player starts in Y. */
	unsigned int real_w;            /*!< (-) Real width in pixels. */
	unsigned int real_h;            /*!< (-) Real height in pixels. */
	unsigned int w;                 /*!< (-) Map width in cells. */
	unsigned int h;                 /*!< (-) Map height in cells. */
	unsigned short tile_w;          /*!< (-) Pixels per cell (width). */
	unsigned short tile_h;          /*!< (-) Pixels per cell (height). */
	struct map_layer layers[2];     /*!< (+) Layers (background, foreground). */
};

/**
 * \brief High level map object.
 *
 * This structure reference a map and perform drawing operations.
 */
struct map {
	struct map_data *data;          /*!< (+&) Map data. */
	struct texture *tileset;        /*!< (+&) Tileset to use. */
	struct texture picture;         /*!< (-) Map drawn into a picture. */
};

/**
 * Open a map defintion.
 *
 * \pre data != NULL
 * \pre path != NULL
 * \param data the map defintion to fill
 * \param path the path to the map
 * \return True if successfully loaded.
 */
bool
map_data_open(struct map_data *data, const char *path);

/**
 * Open map data definition from memory.
 *
 *\pre data != NULL
 *\pre buf != NULL
 *\param data the map definition to fill
 *\param buf the source buffer
 *\param bufsz the source buffer size
 */
bool
map_data_openmem(struct map_data *data, const void *buf, size_t bufsz);

/**
 * Dispose the map definition data.
 *
 * \pre data != NULL
 * \param data the map definition
 */
void
map_data_finish(struct map_data *data);

/**
 * Initialize this map.
 *
 * \pre map != NULL
 * \pre data != NULL
 * \pre tileset != NULL && texture_ok(tileset)
 * \param map the map to initialize
 * \param data the definition to reference
 * \param tileset the tileset to use
 * \return False on errors.
 */
bool
map_init(struct map *map,
         struct map_data *data,
         struct texture *tileset);

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
 * \warning This function does not render anything on the screen.
 */
void
map_repaint(struct map *map);

/**
 * Dispose map resources.
 *
 * \pre map != NULL
 * \param map the map to close
 */
void
map_finish(struct map *map);

#endif /* !MOLKO_MAP_H */
