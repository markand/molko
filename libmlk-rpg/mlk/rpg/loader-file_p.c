/*
 * loader-file_p.c -- resource loader for map/tileset
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

#include <utlist.h>

#include <mlk/util/util.h>

#include <mlk/core/alloc.h>
#include <mlk/core/animation.h>
#include <mlk/core/image.h>
#include <mlk/core/sprite.h>
#include <mlk/core/texture.h>

#include "loader-file_p.h"

struct texture_node {
	struct mlk_texture texture;
	struct texture_node *next;
};

struct sprite_node {
	struct mlk_sprite sprite;
	struct sprite_node *next;
};

struct animation_node {
	struct mlk_animation animation;
	struct animation_node *next;
};

struct mlk__loader_file {
	char directory[MLK_PATH_MAX];
	struct texture_node *textures;
	struct sprite_node *sprites;
	struct animation_node *animations;
};

static inline void
free_textures(struct mlk__loader_file *loader)
{
	struct texture_node *node, *tmp;

	LL_FOREACH_SAFE(loader->textures, node, tmp) {
		mlk_texture_finish(&node->texture);
		mlk_alloc_free(node);
	}

	loader->textures = NULL;
}

static inline void
free_sprites(struct mlk__loader_file *loader)
{
	struct sprite_node *node, *tmp;

	LL_FOREACH_SAFE(loader->sprites, node, tmp)
		mlk_alloc_free(node);

	loader->sprites = NULL;
}

static inline void
free_animations(struct mlk__loader_file *loader)
{
	struct animation_node *node, *tmp;

	LL_FOREACH_SAFE(loader->animations, node, tmp)
		mlk_alloc_free(node);

	loader->animations = NULL;
}

struct mlk__loader_file *
mlk__loader_file_new(const char *file)
{
	assert(file);

	struct mlk__loader_file *loader;
	char filepath[MLK_PATH_MAX];

	if (!(loader = mlk_alloc_new0(1, sizeof (*loader))))
		return NULL;

	mlk_util_strlcpy(filepath, file, sizeof (filepath));
	mlk_util_strlcpy(loader->directory, mlk_util_dirname(filepath), sizeof (loader->directory));

	return loader;
}

const char *
mlk__loader_file_directory(const struct mlk__loader_file *loader)
{
	assert(loader);

	return loader->directory;
}

struct mlk_texture *
mlk__loader_file_texture_open(struct mlk__loader_file *loader, const char *ident)
{
	struct texture_node *node;
	char path[MLK_PATH_MAX];

	if (!(node = mlk_alloc_new0(1, sizeof (*node))))
		return NULL;

	snprintf(path, sizeof (path), "%s/%s", loader->directory, ident);

	if (mlk_image_open(&node->texture, path) < 0) {
		mlk_alloc_free(node);
		return NULL;
	}

	LL_APPEND(loader->textures, node);

	return &node->texture;
}

struct mlk_sprite *
mlk__loader_file_sprite_new(struct mlk__loader_file *loader)
{
	struct sprite_node *node;

	if (!(node = mlk_alloc_new0(1, sizeof (*node))))
		return NULL;

	LL_APPEND(loader->sprites, node);

	return &node->sprite;
}

struct mlk_animation *
mlk__loader_file_animation_new(struct mlk__loader_file *loader)
{
	struct animation_node *node;

	if (!(node = mlk_alloc_new0(1, sizeof (*node))))
		return NULL;

	LL_APPEND(loader->animations, node);

	return &node->animation;
}

void
mlk__loader_file_clear(struct mlk__loader_file *loader)
{
	free_textures(loader);
	free_sprites(loader);
	free_animations(loader);
}

void
mlk__loader_file_free(struct mlk__loader_file *loader)
{
	if (!loader)
		return;

	mlk__loader_file_clear(loader);
	mlk_alloc_free(loader);
}
