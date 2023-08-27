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
#include <stdio.h>
#include <string.h>

#include <mlk/util/util.h>

#include <mlk/core/alloc.h>
#include <mlk/core/image.h>
#include <mlk/core/sprite.h>
#include <mlk/core/texture.h>
#include <mlk/core/util.h>

#include "loader-file_p.h"
#include "map-loader-file.h"
#include "map.h"
#include "tileset-loader-file.h"
#include "tileset-loader.h"

#define THIS(loader) \
	MLK_CONTAINER_OF(loader, struct mlk_map_loader_file, iface)

static void
trash(struct mlk_map_loader_file *file)
{
	mlk__loader_file_clear(file->lf);

	for (int i = 0; i < MLK_MAP_LAYER_TYPE_LAST; ++i) {
		mlk_alloc_free(file->tiles[i]);
		file->tiles[i] = NULL;
	}

	mlk_alloc_free(file->blocks);

	file->blocks = NULL;
}

static struct mlk_texture *
new_texture(struct mlk_map_loader *self, struct mlk_map *map, const char *ident)
{
	(void)map;

	struct mlk_map_loader_file *file = THIS(self);

	return mlk__loader_file_texture_open(file->lf, ident);
}

static struct mlk_sprite *
new_sprite(struct mlk_map_loader *self, struct mlk_map *map)
{
	(void)map;

	struct mlk_map_loader_file *file = THIS(self);

	return mlk__loader_file_sprite_new(file->lf);
}

static struct mlk_tileset *
new_tileset(struct mlk_map_loader *self, struct mlk_map *map, const char *ident)
{
	(void)map;

	struct mlk_map_loader_file *file = THIS(self);
	char path[MLK_PATH_MAX] = {};

	snprintf(path, sizeof (path), "%s/%s", mlk__loader_file_directory(file->lf), ident);

	/*
	 * Cleanup existing resources in case the tileset appears multiple times
	 * in the map.
	 */
	mlk_tileset_loader_clear(file->tileset_loader, &file->tileset);

	if (mlk_tileset_loader_open(file->tileset_loader, &file->tileset, path) < 0)
		return NULL;

	return &file->tileset;
}

static unsigned int *
new_tiles(struct mlk_map_loader *self,
          struct mlk_map *map,
          enum mlk_map_layer_type type,
          size_t n)
{
	(void)map;

	struct mlk_map_loader_file *file = THIS(self);

	return file->tiles[type] = mlk_alloc_new0(n, sizeof (unsigned int));
}

static struct mlk_map_block *
expand_blocks(struct mlk_map_loader *self,
              struct mlk_map *map,
              struct mlk_map_block *blocks,
              size_t blocksz)
{
	(void)map;

	struct mlk_map_loader_file *file = THIS(self);
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
clear(struct mlk_map_loader *self, struct mlk_map *map)
{
	(void)map;

	struct mlk_map_loader_file *file = THIS(self);

	trash(file);
}

static void
finish(struct mlk_map_loader *self)
{
	struct mlk_map_loader_file *file = THIS(self);

	trash(file);
	mlk__loader_file_free(file->lf);

	file->lf = NULL;
}

int
mlk_map_loader_file_init(struct mlk_map_loader_file *file,
                         struct mlk_tileset_loader *tileset_loader,
                         const char *filename)
{
	assert(file);
	assert(tileset_loader);
	assert(filename);

	memset(file, 0, sizeof (*file));

	if (!(file->lf = mlk__loader_file_new(filename)))
		return -1;

	file->tileset_loader = tileset_loader;
	file->iface.new_tileset = new_tileset;
	file->iface.new_texture = new_texture;
	file->iface.new_sprite = new_sprite;
	file->iface.new_tiles = new_tiles;
	file->iface.expand_blocks = expand_blocks;
	file->iface.clear = clear;
	file->iface.finish = finish;

	return 0;
}
