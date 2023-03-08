/*
 * map-loader.h -- abstract map loader
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

#ifndef MLK_RPG_MAP_LOADER_H
#define MLK_RPG_MAP_LOADER_H

#include <mlk/util/sysconfig.h>

#include <stddef.h>

enum mlk_map_layer_type;

struct mlk_map;
struct mlk_map_block;
struct mlk_tileset;

/**
 * \file mlk/rpg/map-loader.h
 * \brief Abstract map loader
 *
 * This module provides a generic way to open maps. It uses a callback similar
 * to the ::mlk_tileset_loader.
 */
struct mlk_map_loader {
	/**
	 * (read-write, borrowed, optional)
	 *
	 * Arbitrary user data for callbacks.
	 */
	void *data;

	/**
	 * (read-write)
	 *
	 * Obtain a tileset that this map requires.
	 *
	 * \param self this loader
	 * \param map the underlying map being loaded
	 * \param ident the texture name (or path)
	 * \return the tileset to use or NULL on failure
	 */
	struct mlk_tileset * (*init_tileset)(struct mlk_map_loader *self,
	                                     struct mlk_map *map,
	                                     const char *ident);

	/**
	 * (read-write)
	 *
	 * Allocate the number of tiles required to fill a layer.
	 *
	 * \param self this loader
	 * \param map the underlying map being loaded
	 * \param type the layer type to allocate
	 * \param n the number of tile items (rows * columns)
	 * \return a pointer to a usable area or NULL on failure
	 */
	unsigned int * (*alloc_tiles)(struct mlk_map_loader *self,
	                              struct mlk_map *map,
	                              enum mlk_map_layer_type type,
	                              size_t n);

	/**
	 * (read-write, optional)
	 *
	 * Load a map object from the special object layer.
	 *
	 * \param self this loader
	 * \param map the underlying map being loaded
	 * \param x the x object coordinate
	 * \param y the y object coordinate
	 * \param w the object width
	 * \param h the object height
	 * \param argument optional data to pass to the object
	 */
	void (*load_object)(struct mlk_map_loader *self,
	                    struct mlk_map *map,
	                    int x,
	                    int y,
	                    unsigned int w,
	                    unsigned int h,
	                    const char *argument);

	/**
	 * (read-write)
	 *
	 * Expand the array required to populate extra map collision blocks.
	 *
	 * \param self this loader
	 * \param map the underlying map being loaded
	 * \param blocks the current blocks array
	 * \param blocksz the number of element to *append* (not the new size)
	 * \return a pointer to a usable area or NULL on failure
	 */
	struct mlk_map_block * (*expand_blocks)(struct mlk_map_loader *self,
                                                struct mlk_map *map,
                                                struct mlk_map_block *blocks,
                                                size_t blocksz);
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Try to open a map from the given path.
 *
 * \pre loader != NULL
 * \pre map != NULL
 * \pre path != NULL
 * \param loader the loader interface
 * \param map the map destination
 * \param path the path to the map file (usually ending in .map)
 * \return 0 on success or -1 on error
 */
int
mlk_map_loader_open(struct mlk_map_loader *loader,
                    struct mlk_map *map,
                    const char *path);

#if defined(MLK_HAVE_FMEMOPEN)

/**
 * Try to open a map from the given path.
 *
 * \pre loader != NULL
 * \pre map != NULL
 * \param loader the loader interface
 * \param map the map destination
 * \param data the map content
 * \param datasz the map content length
 * \return 0 on success or -1 on error
 * \note This function is available only if MLK_HAVE_FMEMOPEN is defined.
 */
int
mlk_map_loader_openmem(struct mlk_map_loader *loader,
                       struct mlk_map *map,
                       const void *data,
                       size_t datasz);

#endif

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_RPG_MAP_FILE_H */
