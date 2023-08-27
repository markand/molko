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
 *
 * This loader implements the map loader interface from a file into a directory
 * (usually ending with `.map` extension).  It will find and allocate resources
 * on the go depending on the file map content.
 *
 * For convenience, this loader will also use ::mlk_tileset_loader for loading
 * and associating the tileset.
 *
 * Example of use:
 *
 * ```c
 * struct mlk_tileset_loader_file tileset_loader;
 * struct mlk_map_loader_file map_loader;
 * struct mlk_map map;
 *
 * // The loader needs to know the map location to retrieve relative files.
 * const char *map_path = "/path/to/world.map";
 *
 * // Initialize the loader, it will be filled with custom internal functions.
 * if (mlk_map_loader_file_init(&map_loader, &tileset_loader.iface, map_path) < 0)
 *     mlk_panic();
 *
 * // Load the map from the file on disk.
 * if (mlk_map_loader_open(&loader, &map, map_path) < 0)
 *     mlk_panic();
 *
 * // Don't forget to initialize the map before use.
 * mlk_map_init(&map);
 *
 * // Destroy the resources.
 * mlk_map_finish(&map);
 * mlk_map_loader_file_finish(&map_loader);
 * mlk_tileset_loader_file_finish(&tileset_loader);
 * ```
 */

#include "map-loader.h"
#include "map.h"
#include "tileset.h"

struct mlk_tileset_loader;

struct mlk_map_loader_file {
	/**
	 * (read-write)
	 *
	 * Underlying map loader.
	 */
	struct mlk_map_loader iface;

	/** \cond MLK_PRIVATE_DECLS */
	unsigned int *tiles[MLK_MAP_LAYER_TYPE_LAST];
	struct mlk_tileset_loader *tileset_loader;
	struct mlk_tileset tileset;
	struct mlk_map_block *blocks;
	struct mlk__loader_file *lf;
	/** \endcond MLK_PRIVATE_DECLS */
};

/**
 * Initialize the loader with internal functions and internal data to allocate
 * and find appropriate resources relative to the map filename.
 *
 * After loading the map with this underlying loader, it should be kept until
 * the map is no longer used.
 *
 * The tileset loader is borrowed and not destroyed when this loader is
 * destroyed, user must do it manually.
 *
 * \pre file != NULL
 * \pre filename != NULL
 * \param file the file loader
 * \param tileset_loader tileset loader interface (borrowed)
 * \param filename path to the map file
 * \return 0 on success or -1 on error
 */
int
mlk_map_loader_file_init(struct mlk_map_loader_file *file,
                         struct mlk_tileset_loader *tileset_loader,
                         const char *filename);

#endif /* !MLK_RPG_MAP_LOADER_FILE_H */
