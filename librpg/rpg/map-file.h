/*
 * map-file.h -- map file loader
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

#ifndef MOLKO_MAP_FILE_H
#define MOLKO_MAP_FILE_H

/**
 * \file map-file.h
 * \brief Map file loader.
 *
 * Because a map is a complicated object that require several components to be
 * loaded, the way it is loaded from a source is implemented separately to keep
 * the \ref map.h module as simple as possible.
 *
 * This module load map files generated from mlk-map tool and expect some
 * convention from the user:
 *
 * - A tileset image must be present in the same directory as the map itself.
 *
 * This module allocates some dynamic data that are stored in the map object as
 * such the map_file structure must be kept until the map is no longer used
 * itself.
 */

#include <stdbool.h>

#include <core/plat.h>
#include <core/sprite.h>
#include <core/texture.h>

#include "map.h"

/**
 * \brief Maximum title map length in file.
 */
#define MAP_FILE_TITLE_MAX 64

/**
 * \brief Context for loading maps.
 *
 * Member fields should not be modified directly but must be modified in the
 * underlying associated map object once loaded.
 *
 * Since the map object does not own resources, this map file will dynamically
 * load and store them from the file and thus must not be destroyed until the
 * map is no longer use itself.
 */
struct map_file {
	/**
	 * (*) Map title loaded from file.
	 */
	char title[MAP_FILE_TITLE_MAX];

	/**
	 * (*) Map layers stored dynamically here.
	 */
	struct map_layer layers[MAP_LAYER_TYPE_NUM];

	struct map_tiledef *tiledefs;   /*!< (*) Per tile properties. */
	struct texture tileset;         /*!< (*) Tileset image. */
	struct sprite sprite;           /*!< (*) Tileset sprite. */
};

/**
 * Try to open a map from a file path.
 *
 * \pre file != NULL
 * \pre path != NULL
 * \pre map != NULL
 * \param file the loader to use
 * \param path the path to the map file
 * \param map the map to set
 * \warning Keep file object until map is no longer used.
 */
bool
map_file_open(struct map_file *file, const char *path, struct map *map) PLAT_NODISCARD;

/**
 * Close resources from the loader.
 *
 * \pre file != NULL
 * \param file the file to loader to destroy
 * \warning Destroy the map itself before calling this function.
 */
void
map_file_finish(struct map_file *file);

#endif /* !MOLKO_MAP_FILE_H */
