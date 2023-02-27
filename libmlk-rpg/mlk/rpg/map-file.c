/*
 * map-file.c -- map file loader
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
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mlk/util/util.h>

#include <mlk/core/alloc.h>
#include <mlk/core/error.h>
#include <mlk/core/image.h>
#include <mlk/core/trace.h>
#include <mlk/core/util.h>

#include "map-file.h"

#define MAX_F(v) MAX_F_(v)
#define MAX_F_(v) "%" #v "[^\n|]"

struct context {
	struct map_file *mf;            /* Map loader. */
	struct map *map;                /* Map object to fill. */
	FILE *fp;                       /* Map file pointer. */
	char basedir[PATH_MAX];         /* Parent map directory */
};

static int
parse_layer_tiles(struct context *ctx, const char *layer_name)
{
	enum map_layer_type layer_type;
	size_t amount, current;

	if (strcmp(layer_name, "background") == 0)
		layer_type = MAP_LAYER_TYPE_BACKGROUND;
	else if (strcmp(layer_name, "foreground") == 0)
		layer_type = MAP_LAYER_TYPE_FOREGROUND;
	else if (strcmp(layer_name, "above") == 0)
		layer_type = MAP_LAYER_TYPE_ABOVE;
	else
		return errorf("invalid layer type: %s", layer_name);

	amount = ctx->map->columns * ctx->map->rows;
	current = 0;

	/*
	 * The next line after a layer declaration is a list of plain integer
	 * that fill the layer tiles.
	 */
	if (!(ctx->mf->layers[layer_type].tiles = mlk_alloc_new0(amount, sizeof (unsigned short))))
		return -1;

	for (int tile; fscanf(ctx->fp, "%d\n", &tile) && current < amount; ++current)
		ctx->mf->layers[layer_type].tiles[current] = tile;

	ctx->map->layers[layer_type].tiles = ctx->mf->layers[layer_type].tiles;

	return 0;
}

static int
parse_actions(struct context *ctx)
{
	char exec[128 + 1];
	int x = 0, y = 0, block = 0;
	unsigned int w = 0, h = 0;

	while (fscanf(ctx->fp, "%d|%d|%u|%u|%d|%128[^\n]\n", &x, &y, &w, &h, &block, exec) >= 5) {
		struct map_block *reg;

		if (!ctx->mf->load_action) {
			mlk_tracef("ignoring action %d,%d,%u,%u,%d,%s", x, y, w, h, block, exec);
			continue;
		}

		ctx->mf->load_action(ctx->map, x, y, w, h, exec);

		/*
		 * Actions do not have concept of collisions because they are
		 * not only used on maps. The map structure has its very own
		 * object to manage collisions but the .map file use the same
		 * directive for simplicity. So create a block region if the
		 * directive has one.
		 */
		if (block) {
			if (!(reg = mlk_alloc_pool_new(&ctx->mf->blocks)))
				return -1;

			reg->x = x;
			reg->y = y;
			reg->w = w;
			reg->h = h;
		}
	}

	/* Reference the blocks array from map_file. */
	ctx->map->blocks = ctx->mf->blocks.data;
	ctx->map->blocksz = ctx->mf->blocks.size;

	return 0;
}

static int
parse_layer(struct context *ctx, const char *line)
{
	char layer_name[32 + 1] = {0};

	/* Check if weight/height has been specified. */
	if (ctx->map->columns == 0 || ctx->map->rows == 0)
		return errorf("missing map dimensions before layer");

	/* Determine layer type. */
	if (sscanf(line, "layer|%32s", layer_name) <= 0)
		return errorf("missing layer type definition");

	if (strcmp(layer_name, "actions") == 0)
		return parse_actions(ctx);

	return parse_layer_tiles(ctx, layer_name);
}

static int
parse_tileset(struct context *ctx, const char *line)
{
	char path[PATH_MAX] = {0}, *p;
	struct map_file *mf = ctx->mf;
	struct tileset_file *tf = &mf->tileset_file;

	if (!(p = strchr(line, '|')))
		return errorf("could not parse tileset");

	snprintf(path, sizeof (path), "%s/%s", ctx->basedir, p + 1);

	if (tileset_file_open(tf, &mf->tileset, path) < 0)
		return -1;

	ctx->map->tileset = &mf->tileset;

	return 0;
}

static int
parse_title(struct context *ctx, const char *line)
{
	if (sscanf(line, "title|" MAX_F(MAP_FILE_TITLE_MAX), ctx->mf->title) != 1 || strlen(ctx->mf->title) == 0)
		return errorf("null map title");

	ctx->map->title = ctx->mf->title;

	return 0;
}

static int
parse_columns(struct context *ctx, const char *line)
{
	if (sscanf(line, "columns|%u", &ctx->map->columns) != 1 || ctx->map->columns == 0)
		return errorf("null map columns");

	return 0;
}

static int
parse_rows(struct context *ctx, const char *line)
{
	if (sscanf(line, "rows|%u", &ctx->map->rows) != 1 || ctx->map->rows == 0)
		return errorf("null map rows");

	return 0;
}

static int
parse_origin(struct context *ctx, const char *line)
{
	if (sscanf(line, "origin|%d|%d", &ctx->map->player_x, &ctx->map->player_y) != 2)
		return errorf("invalid origin");

	return 0;
}

static int
parse_line(struct context *ctx, const char *line)
{
	static const struct {
		const char *property;
		int (*read)(struct context *, const char *);
	} props[] = {
		{ "title",      parse_title             },
		{ "columns",    parse_columns           },
		{ "rows",       parse_rows              },
		{ "tileset",    parse_tileset           },
		{ "origin",     parse_origin            },
		{ "layer",      parse_layer             },
	};

	for (size_t i = 0; i < MLK_UTIL_SIZE(props); ++i)
		if (strncmp(line, props[i].property, strlen(props[i].property)) == 0)
			return props[i].read(ctx, line);

	return 0;
}

static int
parse(struct context *ctx, const char *path)
{
	char line[1024];
	char basedir[PATH_MAX];

	mlk_util_strlcpy(basedir, path, sizeof (basedir));
	mlk_util_strlcpy(ctx->basedir, mlk_util_dirname(basedir), sizeof (ctx->basedir));

	while (fgets(line, sizeof (line), ctx->fp)) {
		/* Remove \n if any */
		line[strcspn(line, "\r\n")] = '\0';

		if (parse_line(ctx, line) < 0)
			return -1;
	}

	return 0;
}

static int
check(struct map *map)
{
	/*
	 * Check that we have parsed every required components.
	 */
	if (!map->title)
		return errorf("missing title");

	/*
	 * We don't need to check width/height because parsing layers and
	 * tilesets already check for their presence, so only check layers.
	 */
	if (!map->layers[0].tiles)
		return errorf("missing background layer");
	if (!map->layers[1].tiles)
		return errorf("missing foreground layer");
	if (!tileset_ok(map->tileset))
		return errorf("missing tileset");

	return 0;
}

int
map_file_open(struct map_file *file, struct map *map, const char *path)
{
	assert(file);
	assert(path);
	assert(map);

	struct context ctx = {
		.mf = file,
		.map = map,
	};
	int ret = 0;

	memset(map, 0, sizeof (*map));
	mlk_alloc_pool_init(&file->blocks, 16, sizeof (*map->blocks), NULL);

	if (!(ctx.fp = fopen(path, "r")))
		goto fail;
	if ((ret = parse(&ctx, path)) < 0 || (ret = check(map)) < 0)
		goto fail;

	fclose(ctx.fp);

	return 0;

fail:
	map_finish(map);
	map_file_finish(file);

	if (ctx.fp)
		fclose(ctx.fp);

	return -1;
}

void
map_file_finish(struct map_file *file)
{
	assert(file);

	mlk_alloc_free(file->layers[0].tiles);
	mlk_alloc_free(file->layers[1].tiles);
	mlk_alloc_free(file->layers[2].tiles);

	tileset_file_finish(&file->tileset_file);
	mlk_alloc_pool_finish(&file->blocks);

	memset(file, 0, sizeof (*file));
}
