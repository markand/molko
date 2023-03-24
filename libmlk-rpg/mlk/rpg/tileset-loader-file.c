/*
 * tileset-file.c -- tileset file loader implementation
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
#include <string.h>

#include <mlk/core/alloc.h>
#include <mlk/core/animation.h>
#include <mlk/core/image.h>
#include <mlk/core/sprite.h>
#include <mlk/core/texture.h>

#include "loader-file_p.h"
#include "tileset-loader-file.h"
#include "tileset-loader.h"
#include "tileset.h"

struct self {
	/* Resources allocator. */
	struct mlk__loader_file *loader;

	/* Arrays reallocated on purpose. */
	struct mlk_tileset_collision *tilecollisions;
	struct mlk_tileset_animation *tileanimations;
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

	/* Clear array of collisions/animations .*/
	mlk_alloc_free(self->tilecollisions);
	mlk_alloc_free(self->tileanimations);
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
new_texture(struct mlk_tileset_loader *loader, struct mlk_tileset *tileset, const char *ident)
{
	(void)tileset;

	struct self *self = loader->data;

	return mlk__loader_file_texture_open(self->loader, ident);
}

static struct mlk_sprite *
new_sprite(struct mlk_tileset_loader *loader, struct mlk_tileset *tileset)
{
	(void)tileset;

	struct self *self = loader->data;

	return mlk__loader_file_sprite_new(self->loader);
}

static struct mlk_animation *
new_animation(struct mlk_tileset_loader *loader, struct mlk_tileset *tileset)
{
	(void)tileset;

	struct self *self = loader->data;

	return mlk__loader_file_animation_new(self->loader);
}

struct mlk_tileset_collision *
expand_collisions(struct mlk_tileset_loader *loader,
                  struct mlk_tileset *tileset,
                  struct mlk_tileset_collision *array,
                  size_t arraysz)
{
	(void)tileset;
	(void)array;

	struct self *self = loader->data;

	return expand((void **)&self->tilecollisions, arraysz, sizeof (struct mlk_tileset_collision));
}

struct mlk_tileset_animation *
expand_animations(struct mlk_tileset_loader *loader,
                  struct mlk_tileset *tileset,
                  struct mlk_tileset_animation *array,
                  size_t arraysz)
{
	(void)tileset;
	(void)array;

	struct self *self = loader->data;

	return expand((void **)&self->tileanimations, arraysz, sizeof (struct mlk_tileset_animation));
}

int
mlk_tileset_loader_file_init(struct mlk_tileset_loader *loader, const char *filename)
{
	assert(loader);
	assert(filename);

	struct self *self;

	memset(loader, 0, sizeof (*loader));

	if (!(self = self_new(filename)))
		return -1;

	loader->data = self;
	loader->new_texture = new_texture;
	loader->new_sprite = new_sprite;
	loader->new_animation = new_animation;
	loader->expand_collisions = expand_collisions;
	loader->expand_animations = expand_animations;

	return 0;
}

void
mlk_tileset_loader_file_finish(struct mlk_tileset_loader *loader)
{
	assert(loader);

	struct self *self = loader->data;

	if (self)
		self_free(self);

	memset(loader, 0, sizeof (*loader));
}
