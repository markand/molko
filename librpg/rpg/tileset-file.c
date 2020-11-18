/*
 * tileset-file.c -- tileset file loader
 *
 * Copyright (c) 2020 David Demelier <markand@malikania.fr>
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

#define _XOPEN_SOURCE 700
#include <assert.h>
#include <errno.h>
#include <libgen.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <core/alloc.h>
#include <core/animation.h>
#include <core/error.h>
#include <core/image.h>
#include <core/util.h>

#include "tileset-file.h"
#include "tileset.h"

#define MAX_F(v) MAX_F_(v)
#define MAX_F_(v) "%" #v "[^|]"

struct tileset_file_animation {
	struct texture texture;
	struct sprite sprite;
	struct animation animation;
};

struct context {
	struct tileset_file *tf;
	struct tileset *tileset;
	FILE *fp;

	char basedir[PATH_MAX];

	/*
	 * The following properties aren't stored in the tileset because they
	 * are not needed after loading.
	 */
	unsigned int tilewidth;
	unsigned int tileheight;

	/* Number of rows/columns in the image. */
	unsigned int nrows;
	unsigned int ncolumns;
};

static int
tiledef_cmp(const void *d1, const void *d2)
{
	const struct tileset_tiledef *mtd1 = d1;
	const struct tileset_tiledef *mtd2 = d2;

	if (mtd1->id < mtd2->id)
		return -1;
	if (mtd1->id > mtd2->id)
		return 1;

	return 0;
}

static int
anim_cmp(const void *d1, const void *d2)
{
	const struct tileset_animation *mtd1 = d1;
	const struct tileset_animation *mtd2 = d2;

	if (mtd1->id < mtd2->id)
		return -1;
	if (mtd1->id > mtd2->id)
		return 1;

	return 0;
}

static bool
parse_tilewidth(struct context *ctx, const char *line)
{
	if (sscanf(line, "tilewidth|%u", &ctx->tilewidth) != 1 || ctx->tilewidth == 0)
		return errorf("tilewidth is null");

	return true;
}

static bool
parse_tileheight(struct context *ctx, const char *line)
{
	if (sscanf(line, "tileheight|%u", &ctx->tileheight) != 1 || ctx->tileheight == 0)
		return errorf("tileheight is null");

	return true;
}

static bool
parse_tiledefs(struct context *ctx, const char *line)
{
	(void)line;

	short x, y;
	unsigned short id, w, h;
	struct tileset_tiledef *tiledefs = NULL;
	size_t tiledefsz = 0;

	while (fscanf(ctx->fp, "%hu|%hd|%hd|%hu|%hu\n", &id, &x, &y, &w, &h) == 5) {
		tiledefs = allocator.realloc(tiledefs, ++tiledefsz * sizeof (*tiledefs));
		tiledefs[tiledefsz - 1].id = id;
		tiledefs[tiledefsz - 1].x = x;
		tiledefs[tiledefsz - 1].y = y;
		tiledefs[tiledefsz - 1].w = w;
		tiledefs[tiledefsz - 1].h = h;
	}

	qsort(tiledefs, tiledefsz, sizeof (*tiledefs), tiledef_cmp);
	ctx->tileset->tiledefs = ctx->tf->tiledefs = tiledefs;
	ctx->tileset->tiledefsz = tiledefsz;

	return true;
}

static bool
parse_animations(struct context *ctx, const char *line)
{
	(void)line;

	unsigned short id;
	unsigned int delay;
	char filename[FILENAME_MAX + 1], path[PATH_MAX];
	struct tileset *tileset = ctx->tileset;
	struct tileset_file *tf = ctx->tf;

	while (fscanf(ctx->fp, "%hu|" MAX_F(FILENAME_MAX) "|%u", &id, filename, &delay) == 3) {
		struct tileset_file_animation *tfa;

		/*
		 * We need two arrays because one must contains sprite, texture
		 * and the animation while the tileset user side API only use
		 * one animation reference.
		 */
		tf->tfasz++;
		tf->tfas = allocator.realloc(tf->tfas, tf->tfasz * sizeof (*tf->tfas));
		tfa = &tf->tfas[tf->tfasz - 1];

		/* This is the real user-side tileset array of animations. */
		tf->anims = allocator.realloc(tf->anims, tf->tfasz * sizeof (*tf->anims));

		snprintf(path, sizeof (path), "%s/%s", ctx->basedir, filename);

		if (!image_open(&tfa->texture, path))
			return false;

		/* Initialize animation. */
		sprite_init(&tfa->sprite, &tfa->texture, ctx->tilewidth, ctx->tileheight);
		animation_init(&tfa->animation, &tfa->sprite, delay);

		/* Finally store it in the tiledef. */
		tf->anims[tf->tfasz - 1].id = id;
		tf->anims[tf->tfasz - 1].animation = &tfa->animation;
	}

	qsort(tf->anims, tf->tfasz, sizeof (*tf->anims), anim_cmp);
	tileset->anims = tf->anims;
	tileset->animsz = tf->tfasz;

	return true;
}

static bool
parse_image(struct context *ctx, const char *line)
{
	char path[PATH_MAX], *p;

	if (ctx->tilewidth == 0 || ctx->tileheight == 0)
		return errorf("missing tile dimensions before image");
	if (!(p = strchr(line, '|')))
		return errorf("could not parse image");

	snprintf(path, sizeof (path), "%s/%s", ctx->basedir, p + 1);

	if (!image_open(&ctx->tf->image, path))
		return false;

	sprite_init(&ctx->tf->sprite, &ctx->tf->image, ctx->tilewidth, ctx->tileheight);
	ctx->tileset->sprite = &ctx->tf->sprite;

	return true;
}

static bool
parse_line(struct context *ctx, const char *line)
{
	static const struct {
		const char *property;
		bool (*read)(struct context *, const char *);
	} props[] = {
		{ "tilewidth",  parse_tilewidth         },
		{ "tileheight", parse_tileheight        },
		{ "tiledefs",   parse_tiledefs          },
		{ "animations", parse_animations        },
		{ "image",      parse_image             }
	};

	for (size_t i = 0; i < NELEM(props); ++i) {
		if (strncmp(line, props[i].property, strlen(props[i].property)) == 0)
			return props[i].read(ctx, line);
	}
	
	return true;
}

static bool
parse(struct context *ctx, const char *path)
{
	char line[1024];
	char basedir[PATH_MAX];

	snprintf(basedir, sizeof (basedir), "%s", path);
	snprintf(ctx->basedir, sizeof (ctx->basedir), "%s", dirname(basedir));

	while (fgets(line, sizeof (line), ctx->fp)) {
		/* Remove \n if any */
		line[strcspn(line, "\n")] = '\0';

		if (!parse_line(ctx, line))
			return false;
	}

	return true;
}

static bool
check(const struct tileset *tileset)
{
	if (!tileset->sprite)
		return errorf("missing tileset image");

	return true;
}

bool
tileset_file_open(struct tileset_file *tf, struct tileset *tileset, const char *path)
{
	assert(tf);
	assert(tileset);
	assert(path);

	struct context ctx = {
		.tf = tf,
		.tileset = tileset
	};
	bool ret = true;

	memset(tileset, 0, sizeof (*tileset));

	if (!(ctx.fp = fopen(path, "r")))
		return errorf("%s", strerror(errno));
	if (!(ret = parse(&ctx, path)) || !(ret = check(tileset)))
		tileset_file_finish(tf);

	return ret;
}

void
tileset_file_finish(struct tileset_file *tf)
{
	assert(tf);

	for (size_t i = 0; i < tf->tfasz; ++i)
		texture_finish(&tf->tfas[i].texture);

	texture_finish(&tf->image);

	free(tf->tiledefs);
	free(tf->tfas);
	free(tf->anims);
	memset(tf, 0, sizeof (*tf));
}
