/*
 * map-loader.c -- abstract map loader
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
#include <stdio.h>
#include <string.h>

#include <mlk/util/util.h>

#include <mlk/core/err.h>
#include <mlk/core/trace.h>
#include <mlk/core/util.h>

#include "map-loader.h"
#include "map.h"

static int
parse_layer_tiles(struct mlk_map_loader *loader, struct mlk_map *map, const char *layer_name, FILE *fp)
{
	enum mlk_map_layer_type layer_type;
	size_t amount, current;
	unsigned int *tiles;

	if (strcmp(layer_name, "background") == 0)
		layer_type = MLK_MAP_LAYER_TYPE_BG;
	else if (strcmp(layer_name, "foreground") == 0)
		layer_type = MLK_MAP_LAYER_TYPE_FG;
	else if (strcmp(layer_name, "above") == 0)
		layer_type = MLK_MAP_LAYER_TYPE_ABOVE;
	else
		return mlk_errf("invalid layer type: %s", layer_name);

	amount = map->columns * map->rows;
	current = 0;

	/*
	 * The next line after a layer declaration is a list of plain integer
	 * that fill the layer tiles.
	 */
	if (!(tiles = loader->alloc_tiles(loader, map, layer_type, amount)))
		return -1;

	for (unsigned int tile; fscanf(fp, "%u\n", &tile) && current < amount; ++current)
		tiles[current] = tile;

	map->layers[layer_type].tiles = tiles;

	return 0;
}

static int
parse_objects(struct mlk_map_loader *loader,
              struct mlk_map *map,
              const char *line,
              FILE *fp)
{
	(void)line;

	char fmt[64] = {0}, exec[256] = {0};
	int x = 0, y = 0, isblock = 0;
	unsigned int w = 0, h = 0;
	struct mlk_map_block *array, *block, *blocks = NULL;
	size_t blocksz = 0;

	snprintf(fmt, sizeof (fmt), "%%d|%%d|%%u|%%u|%%d|%%%zu[^\n]\n", sizeof (exec) - 1);

	while (fscanf(fp, fmt, &x, &y, &w, &h, &isblock, exec) >= 5) {
		if (!loader->load_object) {
			mlk_tracef("ignoring object %d,%d,%u,%u,%d,%s", x, y, w, h, isblock, exec);
			continue;
		}

		loader->load_object(loader, map, x, y, w, h, exec);

		/*
		 * Actions do not have concept of collisions because they are
		 * not only used on maps. The map structure has its very own
		 * object to manage collisions but the .map file use the same
		 * directive for simplicity. So create a block region if the
		 * directive has one.
		 */
		if (isblock) {
			if (!(array = loader->expand_blocks(loader, blocks, blocksz + 1)))
				return -1;

			blocks = array;
			block = &blocks[blocksz++];
			block->x = x;
			block->y = y;
			block->w = w;
			block->h = h;
		}
	}

	/* Reference the blocks array from map_file. */
	map->blocks = blocks;
	map->blocksz = blocksz;

	return 0;
}

static int
parse_layer(struct mlk_map_loader *loader,
            struct mlk_map *map,
            const char *line,
            FILE *fp)
{
	char fmt[32] = {0}, layer_name[32] = {0};

	/* Check if weight/height has been specified. */
	if (map->columns == 0 || map->rows == 0)
		return mlk_errf("missing map dimensions before layer");

	/* Determine layer type. */
	snprintf(fmt, sizeof (fmt), "layer|%%%zus", sizeof (layer_name) - 1);

	if (sscanf(line, fmt, layer_name) <= 0)
		return mlk_errf("missing layer type definition");
	if (strcmp(layer_name, "actions") == 0)
		return parse_objects(loader, map, line, fp);

	return parse_layer_tiles(loader, map, layer_name, fp);
}

static int
parse_tileset(struct mlk_map_loader *loader,
              struct mlk_map *map,
              const char *line,
              FILE *fp)
{
	(void)fp;

	const char *p;

	if (!(p = strchr(line, '|')))
		return mlk_errf("could not parse tileset");
	if (!(map->tileset = loader->init_tileset(loader, map, p + 1)))
		return -1;

	return 0;
}

static int
parse_columns(struct mlk_map_loader *loader,
              struct mlk_map *map,
              const char *line,
              FILE *fp)
{
	(void)loader;
	(void)fp;

	if (sscanf(line, "columns|%u", &map->columns) != 1 || map->columns == 0)
		return mlk_errf("null map columns");

	return 0;
}

static int
parse_rows(struct mlk_map_loader *loader,
           struct mlk_map *map,
           const char *line,
           FILE *fp)
{
	(void)loader;
	(void)fp;

	if (sscanf(line, "rows|%u", &map->rows) != 1 || map->rows == 0)
		return mlk_errf("null map rows");

	return 0;
}

static int
parse_origin(struct mlk_map_loader *loader,
             struct mlk_map *map,
             const char *line,
             FILE *fp)
{
	(void)loader;
	(void)fp;

	if (sscanf(line, "origin|%d|%d", &map->player_x, &map->player_y) != 2)
		return mlk_errf("invalid origin");

	return 0;
}

static int
parse_line(struct mlk_map_loader *loader,
           struct mlk_map *map,
           const char *line,
           FILE *fp)
{
	static const struct {
		const char *property;
		int (*read)(struct mlk_map_loader *, struct mlk_map *, const char *, FILE *);
	} props[] = {
		{ "columns",    parse_columns           },
		{ "rows",       parse_rows              },
		{ "tileset",    parse_tileset           },
		{ "origin",     parse_origin            },
		{ "layer",      parse_layer             },
	};

	for (size_t i = 0; i < MLK_UTIL_SIZE(props); ++i)
		if (strncmp(line, props[i].property, strlen(props[i].property)) == 0)
			return props[i].read(loader, map, line, fp);

	return 0;
}

static int
check(struct mlk_map *map)
{
	/*
	 * We don't need to check width/height because parsing layers and
	 * tilesets already check for their presence, so only check layers.
	 */
	if (!map->layers[0].tiles)
		return mlk_errf("missing background layer");
	if (!map->layers[1].tiles)
		return mlk_errf("missing foreground layer");

	return 0;
}

static int
parse(struct mlk_map_loader *loader, struct mlk_map *map, FILE *fp)
{
	char line[128];

	while (fgets(line, sizeof (line), fp)) {
		/* Remove \n if any */
		line[strcspn(line, "\r\n")] = '\0';

		if (parse_line(loader, map, line, fp) < 0)
			return -1;
	}

	return check(map);
}

int
mlk_map_loader_open(struct mlk_map_loader *loader, struct mlk_map *map, const char *path)
{
	assert(loader);
	assert(map);
	assert(path);

	FILE *fp;

	memset(map, 0, sizeof (*map));

	if (!(fp = fopen(path, "r")))
		return mlk_errf("%s", strerror(errno));

	return parse(loader, map, fp);
}

int
mlk_map_loader_openmem(struct mlk_map_loader *loader, struct mlk_map *map, const void *data, size_t datasz)
{
	assert(loader);
	assert(map);
	assert(data);

	FILE *fp;

	memset(map, 0, sizeof (*map));

	if (!(fp = mlk_util_fmemopen((void *)data, datasz, "r")))
		return -1;

	return parse(loader, map, fp);
}
