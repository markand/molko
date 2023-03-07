/*
 * map-loader-file.h -- map file loader implementation
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

#ifndef MLK_RPG_MAP_LOADER_FILE_H
#define MLK_RPG_MAP_LOADER_FILE_H

/**
 * \file map-loader-file.h
 * \brief Map file loader implementation
 */

#include <mlk/util/util.h>

#include "map.h"
#include "tileset.h"

struct mlk_map_loader;
struct mlk_tileset_loader;

/**
 * \struct mlk_map_loader_file
 * \brief Map loader file structure
 */
struct mlk_map_loader_file {
	/**
	 * (read-only)
	 *
	 * Computed map file directory.
	 */
	char directory[MLK_PATH_MAX];

	/**
	 * (read-write, borrowed)
	 *
	 * The tileset loader to use when finding tilesets in maps.
	 */
	struct mlk_tileset_loader *tileset_loader;

	/** \cond MLK_PRIVATE_DECLS */
	unsigned int *tiles[MLK_MAP_LAYER_TYPE_LAST];
	struct mlk_tileset tileset;
	struct mlk_map_block *blocks;
	/** \endcond MLK_PRIVATE_DECLS */
};

/**
 * Initialize the map loader.
 *
 * All loader member functions will be set and ::mlk_map_loader::data will be
 * set to file loader.
 *
 * \pre file != NULL
 * \pre loader != NULL
 * \pre filename != NULL
 * \param file the file loader
 * \param loader the abstract loader interface
 * \param filename path to the map file
 */
void
mlk_map_loader_file_init(struct mlk_map_loader_file *file,
                         struct mlk_map_loader *loader,
                         const char *filename);

/**
 * Cleanup allocated resources by this file loader.
 *
 * \pre file != NULL
 * \param file the file loader
 * \warning the map loaded with this loader must not be used
 */
void
mlk_map_loader_file_finish(struct mlk_map_loader_file *file);

#endif /* !MLK_RPG_MAP_LOADER_FILE_H */
