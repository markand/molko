/*
 * tileset-loader-file.c -- tileset file loader implementation
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

#include <assert.h>
#include <string.h>

#include <mlk/core/alloc.h>
#include <mlk/core/animation.h>
#include <mlk/core/image.h>
#include <mlk/core/sprite.h>
#include <mlk/core/texture.h>
#include <mlk/core/util.h>

#include "loader-file_p.h"
#include "tileset-loader-file.h"
#include "tileset-loader.h"
#include "tileset.h"

#define THIS(loader) \
	MLK_UTIL_CONTAINER_OF(loader, struct mlk_tileset_loader_file, iface)

static void
trash(struct mlk_tileset_loader_file *file)
{
	mlk__loader_file_clear(file->lf);

	mlk_alloc_free(file->tilecollisions);
	mlk_alloc_free(file->tileanimations);

	file->tilecollisions = NULL;
	file->tileanimations = NULL;
}

static void *
expand(void **array, size_t n, size_t w)
{
	void *ptr;

	if (!*array)
		ptr = mlk_alloc_new0(n, w);
	else
		ptr = mlk_alloc_expand(*array, n);

	if (ptr)
		*array = ptr;

	return ptr;
}

static struct mlk_texture *
new_texture(struct mlk_tileset_loader *self, struct mlk_tileset *tileset, const char *ident)
{
	(void)tileset;

	struct mlk_tileset_loader_file *file = THIS(self);

	return mlk__loader_file_texture_open(file->lf, ident);
}

static struct mlk_sprite *
new_sprite(struct mlk_tileset_loader *self, struct mlk_tileset *tileset)
{
	(void)tileset;

	struct mlk_tileset_loader_file *file = THIS(self);

	return mlk__loader_file_sprite_new(file->lf);
}

static struct mlk_animation *
new_animation(struct mlk_tileset_loader *self, struct mlk_tileset *tileset)
{
	(void)tileset;

	struct mlk_tileset_loader_file *file = THIS(self);

	return mlk__loader_file_animation_new(file->lf);
}

struct mlk_tileset_collision *
expand_collisions(struct mlk_tileset_loader *self,
                  struct mlk_tileset *tileset,
                  struct mlk_tileset_collision *array,
                  size_t arraysz)
{
	(void)tileset;
	(void)array;

	struct mlk_tileset_loader_file *file = THIS(self);

	return expand((void **)&file->tilecollisions, arraysz, sizeof (struct mlk_tileset_collision));
}

struct mlk_tileset_animation *
expand_animations(struct mlk_tileset_loader *self,
                  struct mlk_tileset *tileset,
                  struct mlk_tileset_animation *array,
                  size_t arraysz)
{
	(void)tileset;
	(void)array;

	struct mlk_tileset_loader_file *file = THIS(self);

	return expand((void **)&file->tileanimations, arraysz, sizeof (struct mlk_tileset_animation));
}

static void
clear(struct mlk_tileset_loader *self, struct mlk_tileset *tileset)
{
	(void)tileset;

	struct mlk_tileset_loader_file *file = THIS(self);

	trash(file);
}

static void
finish(struct mlk_tileset_loader *self)
{
	struct mlk_tileset_loader_file *file = THIS(self);

	trash(file);
	mlk__loader_file_free(file->lf);

	file->lf = NULL;
}

int
mlk_tileset_loader_file_init(struct mlk_tileset_loader_file *file, const char *filename)
{
	assert(file);
	assert(filename);

	memset(file, 0, sizeof (*file));

	if (!(file->lf = mlk__loader_file_new(filename)))
		return -1;

	file->iface.new_texture = new_texture;
	file->iface.new_sprite = new_sprite;
	file->iface.new_animation = new_animation;
	file->iface.expand_collisions = expand_collisions;
	file->iface.expand_animations = expand_animations;
	file->iface.clear = clear;
	file->iface.finish = finish;

	return 0;
}
