/*
 * map-loader.h -- abstract map loader
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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

#include <stddef.h>

enum mlk_map_layer_type;

struct mlk_map;
struct mlk_map_block;
struct mlk_sprite;
struct mlk_texture;
struct mlk_tileset;

/**
 * \file mlk/rpg/map-loader.h
 * \brief Abstract map loader
 *
 * This module provides a generic way to open maps. It uses a callback similar
 * to the ::mlk_tileset_loader.
 */

/**
 * \struct mlk_map_loader
 * \brief Abstract map loader structure
 */
struct mlk_map_loader {
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
	struct mlk_tileset * (*new_tileset)(struct mlk_map_loader *self,
	                                    struct mlk_map *map,
	                                    const char *ident);

	/**
	 * (read-write)
	 *
	 * Open a texture from the given ident name.
	 *
	 * \param self this loader
	 * \param ident the texture name (or path)
	 * \return a borrowed texture or NULL on failure
	 */
	struct mlk_texture * (*new_texture)(struct mlk_map_loader *self,
	                                    struct mlk_map *map,
	                                    const char *ident);

	/**
	 * (read-write)
	 *
	 * Return a sprite that the loader needs.
	 *
	 * \param self this loader
	 * \return a unused sprite
	 * \return a borrowed sprite or NULL on failure
	 */
	struct mlk_sprite * (*new_sprite)(struct mlk_map_loader *self,
	                                  struct mlk_map *map);

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
	unsigned int * (*new_tiles)(struct mlk_map_loader *self,
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
	void (*new_object)(struct mlk_map_loader *self,
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

	/**
	 * (read-write, optional)
	 *
	 * Cleanup resources allocated for this mao.
	 *
	 * This is different than finalizing the loader itself, it should be
	 * re-usable after calling this function.
	 *
	 * \param self this loader
	 * \param map the underlying map to cleanup
	 */
	void (*clear)(struct mlk_map_loader *self, struct mlk_map *map);

	/**
	 * (read-write, optional)
	 *
	 * Cleanup the map loader.
	 *
	 * \param self this loader
	 */
	void (*finish)(struct mlk_map_loader *self);
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
 */
int
mlk_map_loader_openmem(struct mlk_map_loader *loader,
                       struct mlk_map *map,
                       const void *data,
                       size_t datasz);

/**
 * Cleanup data for this map.
 *
 * The loader is re-usable after calling this function to load a new map.
 *
 * Invokes ::mlk_map_loader::clear.
 *
 * \pre loader != NULL
 * \param loader the loader interface
 * \param map the map used with this loader
 */
void
mlk_map_loader_clear(struct mlk_map_loader *loader, struct mlk_map *map);

/**
 * Finalize the loader itself.
 *
 * The underlying interface should also clear map resources by convenience
 * if the user forgot to call ::mlk_map_loader_clear.
 *
 * Invokes ::mlk_map_loader::finish.
 *
 * \pre loader != NULL
 * \param loader the loader to finalize
 */
void
mlk_map_loader_finish(struct mlk_map_loader *loader);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_RPG_MAP_FILE_H */
