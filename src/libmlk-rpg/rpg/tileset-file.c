/*
 * tileset-file.c -- tileset file loader
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <port/port.h>

#include <core/alloc.h>
#include <core/animation.h>
#include <core/error.h>
#include <core/image.h>
#include <core/util.h>
#include <core/zfile.h>

#include "rpg_p.h"
#include "tileset-file.h"
#include "tileset.h"

#define MAX_F(v) MAX_F_(v)
#define MAX_F_(v) "%" #v "[^\n|]"

/*
 * This is how memory for animations is allocated in the tileset_file
 * structure.
 *
 * As animations require a texture and a sprite to be present, we need to store
 * them locally in the tileset_file structure.
 *
 * tileset_file->anims[0] array (struct tileset_animation_block):
 *
 * [0]            [1]            [N]
 *  | texture      | texture      | texture
 *  | sprite       | sprite       | sprite
 *  | animation    | animation    | animation
 *
 * tileset_file->anims[1] array (struct tileset_animation):
 *
 * [0]            [1]            [N]
 *  | id           | id           | id
 *  | animation ^  | animation ^  | animation ^
 *
 * The second array is the exposed array through the tileset->anims pointer,
 * animations are referenced from the first array. This is because user may need
 * or replace the tileset by itself and as such we need to keep track of the
 * resource the tileset_file has allocated itself.
 */

struct tileset_animation_block {
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

static void
tileset_animation_block_finish(void *data)
{
	struct tileset_animation_block *anim = data;

	texture_finish(&anim->texture);
}

static int
tileset_tiledef_cmp(const void *d1, const void *d2)
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
tileset_animation_cmp(const void *d1, const void *d2)
{
	const struct tileset_animation *mtd1 = d1;
	const struct tileset_animation *mtd2 = d2;

	if (mtd1->id < mtd2->id)
		return -1;
	if (mtd1->id > mtd2->id)
		return 1;

	return 0;
}

static int
parse_tilewidth(struct context *ctx, const char *line)
{
	if (sscanf(line, "tilewidth|%u", &ctx->tilewidth) != 1 || ctx->tilewidth == 0)
		return errorf(_("tilewidth is null"));

	return 0;
}

static int
parse_tileheight(struct context *ctx, const char *line)
{
	if (sscanf(line, "tileheight|%u", &ctx->tileheight) != 1 || ctx->tileheight == 0)
		return errorf(_("tileheight is null"));

	return 0;
}

static int
parse_tiledefs(struct context *ctx, const char *line)
{
	(void)line;

	short x, y;
	unsigned short id, w, h;
	struct tileset_tiledef *td;

	alloc_pool_init(&ctx->tf->tiledefs, sizeof (*td), NULL);

	while (fscanf(ctx->fp, "%hu|%hd|%hd|%hu|%hu\n", &id, &x, &y, &w, &h) == 5) {
		td = alloc_pool_new(&ctx->tf->tiledefs);
		td->id = id;
		td->x = x;
		td->y = y;
		td->w = w;
		td->h = h;
	}

	/* Sort the array and expose it through the tileset->tiledefs pointer. */
	qsort(ctx->tf->tiledefs.data, ctx->tf->tiledefs.size, ctx->tf->tiledefs.elemsize, tileset_tiledef_cmp);
	ctx->tileset->tiledefs = ctx->tf->tiledefs.data;
	ctx->tileset->tiledefsz = ctx->tf->tiledefs.size;

	return 0;
}

static int
parse_animations(struct context *ctx, const char *line)
{
	(void)line;

	unsigned short id;
	unsigned int delay;
	char filename[FILENAME_MAX + 1];
	struct tileset_animation_block *anim;

	alloc_pool_init(&ctx->tf->anims[0], sizeof (struct tileset_animation_block), tileset_animation_block_finish);
	alloc_pool_init(&ctx->tf->anims[1], sizeof (struct tileset_animation), NULL);

	/*
	 * 1. Create the first array of animation, sprite and texture that are
	 *    owned by the tileset_file structure.
	 */
	while (fscanf(ctx->fp, "%hu|" MAX_F(FILENAME_MAX) "|%u", &id, filename, &delay) == 3) {
		anim = alloc_pool_new(&ctx->tf->anims[0]);

		if (image_open(&anim->texture, util_pathf("%s/%s", ctx->basedir, filename)) < 0)
			return -1;

		sprite_init(&anim->sprite, &anim->texture, ctx->tilewidth, ctx->tileheight);
		animation_init(&anim->animation, &anim->sprite, delay);
	}

	/*
	 * 2. Create the second array that only consist of pointers to
	 *    animations referencing the first array.
	 */
	for (size_t i = 0; i < ctx->tf->anims[0].size; ++i) {
		struct tileset_animation_block *anim = alloc_pool_get(&ctx->tf->anims[0], i);
		struct tileset_animation *ta;

		if (!(ta = alloc_pool_new(&ctx->tf->anims[1])))
			return -1;

		ta->id = id;
		ta->animation = &anim->animation;
	}

	/*
	 * 3. Finally expose the second array through the tileset->anims pointer
	 *    and sort it.
	 */
	qsort(ctx->tf->anims[1].data, ctx->tf->anims[1].size, ctx->tf->anims[1].elemsize, tileset_animation_cmp);
	ctx->tileset->anims  = ctx->tf->anims[1].data;
	ctx->tileset->animsz = ctx->tf->anims[1].size;

	return 0;
}

static int
parse_image(struct context *ctx, const char *line)
{
	char *p;

	if (ctx->tilewidth == 0 || ctx->tileheight == 0)
		return errorf(_("missing tile dimensions before image"));
	if (!(p = strchr(line, '|')))
		return errorf(_("could not parse image"));

	if (image_open(&ctx->tf->image, util_pathf("%s/%s", ctx->basedir, p + 1)) < 0)
		return -1;

	sprite_init(&ctx->tf->sprite, &ctx->tf->image, ctx->tilewidth, ctx->tileheight);
	ctx->tileset->sprite = &ctx->tf->sprite;

	return 0;
}

static int
parse_line(struct context *ctx, const char *line)
{
	static const struct {
		const char *property;
		int (*read)(struct context *, const char *);
	} props[] = {
		{ "tilewidth",  parse_tilewidth         },
		{ "tileheight", parse_tileheight        },
		{ "tiledefs",   parse_tiledefs          },
		{ "animations", parse_animations        },
		{ "image",      parse_image             }
	};

	for (size_t i = 0; i < UTIL_SIZE(props); ++i) {
		if (strncmp(line, props[i].property, strlen(props[i].property)) == 0)
			return props[i].read(ctx, line);
	}
	
	return 0;
}

static int
parse(struct context *ctx, const char *path)
{
	char line[1024];
	char basedir[PATH_MAX];

	port_strlcpy(basedir, path, sizeof (basedir));
	port_strlcpy(ctx->basedir, port_dirname(basedir), sizeof (ctx->basedir));

	while (fgets(line, sizeof (line), ctx->fp)) {
		/* Remove \n if any */
		line[strcspn(line, "\r\n")] = '\0';

		if (parse_line(ctx, line) < 0)
			return -1;
	}

	return 0;
}

static int
check(const struct tileset *tileset)
{
	if (!tileset->sprite)
		return errorf(_("missing tileset image"));

	return 0;
}

int
tileset_file_open(struct tileset_file *tf, struct tileset *tileset, const char *path)
{
	assert(tf);
	assert(tileset);
	assert(path);

	struct context ctx = {
		.tf = tf,
		.tileset = tileset
	};
	struct zfile zf;
	int ret = 0;

	memset(tileset, 0, sizeof (*tileset));

	if (zfile_open(&zf, path) < 0)
		return errorf("%s: %s", path, strerror(errno));

	ctx.fp = zf.fp;

	if ((ret = parse(&ctx, path)) < 0 || (ret = check(tileset)) < 0)
		tileset_file_finish(tf);

	zfile_close(&zf);

	return ret;
}

void
tileset_file_finish(struct tileset_file *tf)
{
	assert(tf);

	alloc_pool_finish(&tf->tiledefs);
	alloc_pool_finish(&tf->anims[0]);
	alloc_pool_finish(&tf->anims[1]);

	texture_finish(&tf->image);

	memset(tf, 0, sizeof (*tf));
}
