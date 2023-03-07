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

#include "tileset-loader-file.h"
#include "tileset-loader.h"
#include "tileset.h"

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

static struct mlk_texture *
init_texture(struct mlk_tileset_loader *self, const char *ident)
{
	struct mlk_tileset_loader_file *file = self->data;
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
init_sprite(struct mlk_tileset_loader *self)
{
	struct mlk_tileset_loader_file *file = self->data;

	return allocate((void ***)&file->sprites, sizeof (struct mlk_sprite));
}

static struct mlk_animation *
init_animation(struct mlk_tileset_loader *self)
{
	struct mlk_tileset_loader_file *file = self->data;

	return allocate((void ***)&file->animations, sizeof (struct mlk_animation));
}

struct mlk_tileset_collision *
expand_collisions(struct mlk_tileset_loader *self,
                  struct mlk_tileset_collision *array,
                  size_t arraysz)
{
	(void)array;

	struct mlk_tileset_loader_file *file = self->data;

	return expand((void **)&file->tilecollisions, arraysz, sizeof (struct mlk_tileset_collision));
}

struct mlk_tileset_animation *
expand_animations(struct mlk_tileset_loader *self,
                  struct mlk_tileset_animation *array,
                  size_t arraysz)
{
	(void)array;

	struct mlk_tileset_loader_file *file = self->data;

	return expand((void **)&file->tileanimations, arraysz, sizeof (struct mlk_tileset_animation));
}

void
mlk_tileset_loader_file_init(struct mlk_tileset_loader_file *file,
                      struct mlk_tileset_loader *loader,
                      const char *filename)
{
	assert(file);
	assert(loader);
	assert(filename);

	char filepath[MLK_PATH_MAX];

	memset(file, 0, sizeof (*file));
	memset(loader, 0, sizeof (*loader));

	/* Determine base filename base directory. */
	mlk_util_strlcpy(filepath, filename, sizeof (filepath));
	mlk_util_strlcpy(file->directory, mlk_util_dirname(filepath), sizeof (file->directory));

	loader->data = file;
	loader->init_texture = init_texture;
	loader->init_sprite = init_sprite;
	loader->init_animation = init_animation;
	loader->expand_collisions = expand_collisions;
	loader->expand_animations = expand_animations;
}

void
mlk_tileset_loader_file_finish(struct mlk_tileset_loader_file *file)
{
	assert(file);

	/* Finalize individual elements. */
	finish((void ***)&file->textures, finish_texture);
	finish((void ***)&file->sprites, mlk_alloc_free);
	finish((void ***)&file->animations, mlk_alloc_free);

	/* Clear array of collisions/animations .*/
	mlk_alloc_free(file->tilecollisions);
	mlk_alloc_free(file->tileanimations);

	memset(file, 0, sizeof (*file));
}
