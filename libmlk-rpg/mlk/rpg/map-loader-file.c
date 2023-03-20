/*
 * map-loader-file.c -- map file loader implementation
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

#include <assert.h>

#include <mlk/core/alloc.h>
#include <mlk/core/image.h>
#include <mlk/core/sprite.h>
#include <mlk/core/texture.h>

#include "map-loader-file.h"
#include "map-loader.h"
#include "map.h"
#include "tileset-loader.h"

static inline void *
allocate(void ***array, size_t width)
{
	void **ptr, *elem;

	/* Not yet allocated? Allocate a new pointer element. */
	if (!*array)
		ptr = mlk_alloc_new0(1, sizeof (void *));
	else
		ptr = mlk_alloc_expand(*array, 1);

	if (!ptr)
		return NULL;

	/* Now allocate the element itself because. */
	if (!(elem = mlk_alloc_new0(1, width)))
		return NULL;

	/* Store it into the array of elements. */
	ptr[mlk_alloc_getn(ptr) - 1] = elem;
	*array = ptr;

	return elem;
}

static struct mlk_texture *
init_texture(struct mlk_map_loader *self,
             struct mlk_map *map,
             const char *ident)
{
	(void)map;

	struct mlk_map_loader_file *file = self->data;
	struct mlk_texture *texture;
	char path[MLK_PATH_MAX];

	snprintf(path, sizeof (path), "%s/%s", file->directory, ident);

	/* No need to deallocate, already done in finish anyway. */
	if (!(texture = allocate((void ***)&file->textures, sizeof (struct mlk_texture))))
		return NULL;
	if (mlk_image_open(texture, path) < 0)
		return NULL;

	return texture;
}

static struct mlk_sprite *
init_sprite(struct mlk_map_loader *self, struct mlk_map *map)
{
	(void)map;

	struct mlk_map_loader_file *file = self->data;

	return allocate((void ***)&file->sprites, sizeof (struct mlk_sprite));
}

static struct mlk_tileset *
init_tileset(struct mlk_map_loader *self,
             struct mlk_map *map,
             const char *ident)
{
	(void)map;

	struct mlk_map_loader_file *file = self->data;
	struct mlk_tileset_loader tileset_loader;
	char path[MLK_PATH_MAX];

	snprintf(path, sizeof (path), "%s/%s", file->directory, ident);

	/*
	 * Just make sure that we don't leak in case tileset directory is listed
	 * more than once.
	 */
	mlk_tileset_loader_file_finish(&file->tileset_loader_file);
	mlk_tileset_loader_file_init(&file->tileset_loader_file, &tileset_loader, path);

	if (mlk_tileset_loader_open(&tileset_loader, &file->tileset, path) < 0)
		return NULL;

	return &file->tileset;
}

static unsigned int *
alloc_tiles(struct mlk_map_loader *self,
            struct mlk_map *map,
            enum mlk_map_layer_type type,
            size_t n)
{
	(void)map;

	struct mlk_map_loader_file *file = self->data;

	return file->tiles[type] = mlk_alloc_new0(n, sizeof (unsigned int));
}

static struct mlk_map_block *
expand_blocks(struct mlk_map_loader *self,
              struct mlk_map *map,
              struct mlk_map_block *blocks,
              size_t blocksz)
{
	(void)map;

	struct mlk_map_loader_file *file = self->data;
	struct mlk_map_block *ptr;

	if (!file->blocks)
		ptr = mlk_alloc_new0(1, sizeof (*ptr));
	else
		ptr = mlk_alloc_expand(file->blocks, blocksz);

	if (ptr)
		file->blocks = blocks;

	return ptr;
}

static void
finish(void ***ptr, void (*finish)(void *))
{
	size_t len;

	/* Already cleared. */
	if (!*ptr)
		return;

	len = mlk_alloc_getn(*ptr);

	for (size_t i = 0; i < len; ++i)
		finish((*ptr)[i]);

	mlk_alloc_free(*ptr);
	*ptr = NULL;
}

static void
finish_texture(void *element)
{
	mlk_texture_finish(element);
	mlk_alloc_free(element);
}

void
mlk_map_loader_file_init(struct mlk_map_loader_file *file,
                         struct mlk_map_loader *loader,
                         const char *filename)
{
	assert(file);
	assert(loader);

	char filepath[MLK_PATH_MAX];

	if (!file->directory[0]) {
		/* Determine base filename base directory. */
		mlk_util_strlcpy(filepath, filename, sizeof (filepath));
		mlk_util_strlcpy(file->directory, mlk_util_dirname(filepath), sizeof (file->directory));
	}
	if (!loader->data)
		loader->data = file;
	if (!loader->init_tileset)
		loader->init_tileset = init_tileset;
	if (!loader->init_texture)
		loader->init_texture = init_texture;
	if (!loader->init_sprite)
		loader->init_sprite = init_sprite;
	if (!loader->alloc_tiles)
		loader->alloc_tiles = alloc_tiles;
	if (!loader->expand_blocks)
		loader->expand_blocks = expand_blocks;
}

void
mlk_map_loader_file_finish(struct mlk_map_loader_file *file)
{
	assert(file);

	for (int i = 0; i < MLK_MAP_LAYER_TYPE_LAST; ++i) {
		mlk_alloc_free(file->tiles[i]);
		file->tiles[i] = NULL;
	}

	mlk_tileset_loader_file_finish(&file->tileset_loader_file);
	mlk_alloc_free(file->blocks);
	file->blocks = NULL;

	finish((void ***)&file->sprites, mlk_alloc_free);
	finish((void ***)&file->textures, finish_texture);
}
