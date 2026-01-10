/*
 * tileset-loader.c -- abstract tileset loader
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
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mlk/util/util.h>

#include <mlk/core/animation.h>
#include <mlk/core/err.h>
#include <mlk/core/sprite.h>
#include <mlk/core/util.h>

#include "tileset-loader.h"
#include "tileset.h"

static int
collision_cmp(const void *d1, const void *d2)
{
	const struct mlk_tileset_collision *c1 = d1;
	const struct mlk_tileset_collision *c2 = d2;

	if (c1->id < c2->id)
		return -1;
	if (c1->id > c2->id)
		return 1;

	return 0;
}

static int
animation_cmp(const void *d1, const void *d2)
{
	const struct mlk_tileset_animation *a1 = d1;
	const struct mlk_tileset_animation *a2 = d2;

	if (a1->id < a2->id)
		return -1;
	if (a1->id > a2->id)
		return 1;

	return 0;
}

static int
parse_tilewidth(struct mlk_tileset_loader *loader,
                struct mlk_tileset *tileset,
                const char *line,
                FILE *fp)
{
	(void)loader;
	(void)tileset;
	(void)fp;

	if (sscanf(line, "tilewidth|%u", &loader->tilewidth) != 1 || loader->tilewidth == 0)
		return mlk_errf("tilewidth is null or invalid");

	return 0;
}

static int
parse_tileheight(struct mlk_tileset_loader *loader,
                 struct mlk_tileset *tileset,
                 const char *line,
                 FILE *fp)
{
	(void)loader;
	(void)tileset;
	(void)fp;

	if (sscanf(line, "tileheight|%u", &loader->tileheight) != 1 || loader->tileheight == 0)
		return mlk_errf("tileheight is null or invalid");

	return 0;
}

static int
parse_collisions(struct mlk_tileset_loader *loader,
                 struct mlk_tileset *tileset,
                 const char *line,
                 FILE *fp)
{
	(void)line;

	struct mlk_tileset_collision *array, *collision, *collisions = NULL;
	unsigned int id, w, h;
	int x, y;
	size_t collisionsz = 0;

	while (fscanf(fp, "%u|%d|%d|%u|%u\n", &id, &x, &y, &w, &h) == 5) {
		if (!(array = loader->expand_collisions(loader, tileset, collisions, collisionsz + 1)))
			return -1;

		collisions = array;
		collision = &collisions[collisionsz++];
		collision->id = id;
		collision->x = x;
		collision->y = y;
		collision->w = w;
		collision->h = h;
	}

	/*
	 * Sort and link this array in the final tileset, user has ownership of
	 * the data.
	 */
	qsort(collisions, collisionsz, sizeof (*collisions), collision_cmp);
	tileset->collisions = collisions;
	tileset->collisionsz = collisionsz;

	return 0;
}

static int
parse_animations(struct mlk_tileset_loader *loader,
                 struct mlk_tileset *tileset,
                 const char *line,
                 FILE *fp)
{
	(void)line;

	char fmt[64], filename[MLK_PATH_MAX];
	unsigned int id, delay;
	struct mlk_tileset_animation *array, *tileanimation, *tileanimations = NULL;
	struct mlk_texture *texture;
	struct mlk_sprite *sprite;
	struct mlk_animation *animation;
	size_t tileanimationsz = 0;

	/* Create a format string for fscanf. */
	snprintf(fmt, sizeof (fmt), "%%u|%%%zu[^|]|%%u", sizeof (filename) - 1);

	/*
	 * When parsing animations, we have to create three different
	 * structures:
	 *
	 * 1. The texture itself.
	 * 2. The sprite object that will use the above texture.
	 * 3. The animation object.
	 * 4. Link the animation to the tileset animation.
	 */
	while (fscanf(fp, fmt, &id, filename, &delay) == 3) {
		if (!(texture = loader->new_texture(loader, tileset, filename)))
			return -1;
		if (!(sprite = loader->new_sprite(loader, tileset)))
			return -1;
		if (!(animation = loader->new_animation(loader, tileset)))
			return -1;
		if (!(array = loader->expand_animations(loader, tileset, tileanimations, tileanimationsz + 1)))
			return -1;

		/* Bind the texture to the new sprite. */
		sprite->texture = texture;
		sprite->cellw = loader->tilewidth;
		sprite->cellh = loader->tileheight;
		mlk_sprite_init(sprite);

		/* Bind the sprite to the new animation. */
		animation->sprite = sprite;
		animation->delay = delay;

		/* Add the animation to the array. */
		tileanimations = array;
		tileanimation = &tileanimations[tileanimationsz++];
		tileanimation->id = id;
		tileanimation->animation = animation;
	}

	/*
	 * Sort and link this array in the final tileset, user has ownership of
	 * the data.
	 */
	qsort(tileanimations, tileanimationsz, sizeof (*tileanimations), animation_cmp);
	tileset->animations = tileanimations;
	tileset->animationsz = tileanimationsz;

	return 0;
}

static int
parse_image(struct mlk_tileset_loader *loader,
            struct mlk_tileset *tileset,
            const char *line,
            FILE *fp)
{
	(void)fp;

	const char *p;
	struct mlk_texture *texture;
	struct mlk_sprite *sprite;

	if (loader->tilewidth == 0 || loader->tileheight == 0)
		return mlk_errf("missing tile dimensions before image");
	if (!(p = strchr(line, '|')))
		return mlk_errf("could not parse image");
	if (!(texture = loader->new_texture(loader, tileset, p + 1)))
		return -1;
	if (!(sprite = loader->new_sprite(loader, tileset)))
		return -1;

	/* Initialize the sprite with the texture. */
	sprite->texture = texture;
	sprite->cellw = loader->tilewidth;
	sprite->cellh = loader->tileheight;
	mlk_sprite_init(sprite);

	/* Link this texture to the final tileset. */
	tileset->sprite = sprite;

	return 0;
}

static int
parse_line(struct mlk_tileset_loader *loader,
           struct mlk_tileset *tileset,
           const char *line,
           FILE *fp)
{
	static const struct {
		const char *property;
		int (*read)(struct mlk_tileset_loader *, struct mlk_tileset *, const char *, FILE *);
	} props[] = {
		{ "tilewidth",  parse_tilewidth         },
		{ "tileheight", parse_tileheight        },
		{ "collisions", parse_collisions        },
		{ "animations", parse_animations        },
		{ "image",      parse_image             }
	};

	for (size_t i = 0; i < MLK_UTIL_SIZE(props); ++i) {
		if (strncmp(line, props[i].property, strlen(props[i].property)) == 0)
			return props[i].read(loader, tileset, line, fp);
	}

	return 0;
}

static int
check(const struct mlk_tileset *tileset)
{
	if (!tileset->sprite)
		return mlk_errf("missing tileset image");

	return 0;
}

static int
parse(struct mlk_tileset_loader *loader, struct mlk_tileset *tileset, FILE *fp)
{
	char line[128];

	while (fgets(line, sizeof (line), fp)) {
		/* Remove \n if any */
		line[strcspn(line, "\r\n")] = 0;

		if (parse_line(loader, tileset, line, fp) < 0)
			return -1;
	}

	return check(tileset);
}

int
mlk_tileset_loader_open(struct mlk_tileset_loader *loader,
                        struct mlk_tileset *tileset,
                        const char *path)
{
	assert(loader);
	assert(tileset);
	assert(path);

	FILE *fp;

	memset(tileset, 0, sizeof (*tileset));

	if (!(fp = fopen(path, "r")))
		return mlk_errf("%s", strerror(errno));

	return parse(loader, tileset, fp);
}

int
mlk_tileset_loader_openmem(struct mlk_tileset_loader *loader,
                           struct mlk_tileset *tileset,
                           const void *data,
                           size_t datasz)
{
	assert(loader);
	assert(tileset);
	assert(data);

	FILE *fp;

	memset(tileset, 0, sizeof (*tileset));

	if (!(fp = mlk_util_fmemopen((void *)data, datasz, "r")))
		return -1;

	return parse(loader, tileset, fp);
}

void
mlk_tileset_loader_clear(struct mlk_tileset_loader *loader,
                         struct mlk_tileset *tileset)
{
	assert(loader);
	assert(tileset);

	if (loader->clear)
		loader->clear(loader, tileset);

	memset(tileset, 0, sizeof (*tileset));
}

void
mlk_tileset_loader_finish(struct mlk_tileset_loader *loader)
{
	assert(loader);

	if (loader->finish)
		loader->finish(loader);
}
