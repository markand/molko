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

#include "loader-file_p.h"
#include "map-loader-file.h"
#include "map-loader.h"
#include "map.h"
#include "tileset-loader-file.h"
#include "tileset-loader.h"
#include "tileset.h"

struct self {
	/* Resources allocator. */
	struct mlk__loader_file *loader;

	/* Own allocated tiles. */
	unsigned int *tiles[MLK_MAP_LAYER_TYPE_LAST];

	/*
	 * We use a tileset file loader if new_tileset function isn't present in
	 * this map loader.
	 */
	struct mlk_tileset_loader tileset_loader;
	struct mlk_tileset tileset;

	/* Own allocated blocks. */
	struct mlk_map_block *blocks;
};

static struct self *
self_new(const char *path)
{
	struct self *self;

	if (!(self = mlk_alloc_new0(1, sizeof (*self))))
		return NULL;
	if (!(self->loader = mlk__loader_file_new(path))) {
		mlk_alloc_free(self);
		return NULL;
	}

	return self;
}

static void
self_free(struct self *self)
{
	mlk__loader_file_free(self->loader);

	for (int i = 0; i < MLK_MAP_LAYER_TYPE_LAST; ++i)
		mlk_alloc_free(self->tiles[i]);

	mlk_tileset_loader_file_finish(&self->tileset_loader);
	mlk_alloc_free(self->blocks);
}

static struct mlk_texture *
new_texture(struct mlk_map_loader *loader,
            struct mlk_map *map,
            const char *ident)
{
	(void)map;

	struct self *self = loader->data;

	return mlk__loader_file_texture_open(self->loader, ident);
}

static struct mlk_sprite *
new_sprite(struct mlk_map_loader *loader, struct mlk_map *map)
{
	(void)map;

	struct self *self = loader->data;

	return mlk__loader_file_sprite_new(self->loader);
}

static struct mlk_tileset *
new_tileset(struct mlk_map_loader *loader,
            struct mlk_map *map,
            const char *ident)
{
	(void)map;

	struct self *self = loader->data;
	char path[MLK_PATH_MAX];

	snprintf(path, sizeof (path), "%s/%s", mlk__loader_file_directory(self->loader), ident);

	/*
	 * Just make sure that we don't leak in case tileset directory is listed
	 * more than once.
	 */
	mlk_tileset_loader_file_finish(&self->tileset_loader);
	mlk_tileset_loader_file_init(&self->tileset_loader, path);

	if (mlk_tileset_loader_open(&self->tileset_loader, &self->tileset, path) < 0)
		return NULL;

	return &self->tileset;
}

static unsigned int *
new_tiles(struct mlk_map_loader *loader,
          struct mlk_map *map,
          enum mlk_map_layer_type type,
          size_t n)
{
	(void)map;

	struct self *self = loader->data;

	return self->tiles[type] = mlk_alloc_new0(n, sizeof (unsigned int));
}

static struct mlk_map_block *
expand_blocks(struct mlk_map_loader *loader,
              struct mlk_map *map,
              struct mlk_map_block *blocks,
              size_t blocksz)
{
	(void)map;

	struct self *self = loader->data;
	struct mlk_map_block *ptr;

	if (!self->blocks)
		ptr = mlk_alloc_new0(1, sizeof (*ptr));
	else
		ptr = mlk_alloc_expand(self->blocks, blocksz);

	if (ptr)
		self->blocks = blocks;

	return ptr;
}

int
mlk_map_loader_file_init(struct mlk_map_loader *loader, const char *filename)
{
	assert(loader);
	assert(filename);

	struct self *self;

	memset(loader, 0, sizeof (*loader));

	if (!(self = self_new(filename)))
		return -1;

	loader->data = self;
	loader->new_tileset = new_tileset;
	loader->new_texture = new_texture;
	loader->new_sprite = new_sprite;
	loader->new_tiles = new_tiles;
	loader->expand_blocks = expand_blocks;

	return 0;
}

void
mlk_map_loader_file_finish(struct mlk_map_loader *loader)
{
	assert(loader);

	struct self *self = loader->data;

	if (self)
		self_free(self);

	memset(loader, 0, sizeof (*loader));
}
