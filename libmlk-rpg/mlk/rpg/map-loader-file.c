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

#include "map-loader-file.h"
#include "map-loader.h"
#include "map.h"
#include "tileset-loader.h"

static struct mlk_tileset *
init_tileset(struct mlk_map_loader *self,
             struct mlk_map *map,
             const char *ident)
{
	(void)map;

	struct mlk_map_loader_file *file = self->data;
	char path[MLK_PATH_MAX];

	snprintf(path, sizeof (path), "%s/%s", file->directory, ident);

	if (mlk_tileset_loader_open(file->tileset_loader, &file->tileset, path) < 0)
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

void
mlk_map_loader_file_init(struct mlk_map_loader_file *file,
                         struct mlk_map_loader *loader,
                         const char *filename)
{
	assert(file);
	assert(loader);

	char filepath[MLK_PATH_MAX];

	/* Determine base filename base directory. */
	mlk_util_strlcpy(filepath, filename, sizeof (filepath));
	mlk_util_strlcpy(file->directory, mlk_util_dirname(filepath), sizeof (file->directory));

	loader->data = file;
	loader->init_tileset = init_tileset;
	loader->alloc_tiles = alloc_tiles;
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

	mlk_alloc_free(file->blocks);
	file->blocks = NULL;
}
