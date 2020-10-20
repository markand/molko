/*
 * map.c -- game map
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <core/error.h>
#include <core/error_p.h>
#include <core/image.h>
#include <core/painter.h>
#include <core/sprite.h>
#include <core/sys.h>
#include <core/texture.h>
#include <core/window.h>

#include "map.h"

/* Create %<v>c string literal for scanf */
#define MAX_F(v) MAX_F_(v)
#define MAX_F_(v) "%" #v "c"

static void
parse_layer(struct map_data *data, const char *line, FILE *fp)
{
	char layer_name[32 + 1] = { 0 };
	struct map_layer *layer;
	size_t amount, current;

	/* Determine layer. */
	if (sscanf(line, "layer|%32s", layer_name) <= 0)
		return;
	if (strcmp(layer_name, "background") == 0)
		layer = &data->layers[0];
	else if (strcmp(layer_name, "foreground") == 0)
		layer = &data->layers[1];
	else
		return;

	/* Check if weight/height has been specified. */
	if (data->w == 0 || data->h == 0)
		return;

	amount = data->w * data->h;
	current = 0;

	if (!(layer->tiles = calloc(amount, sizeof (unsigned short))))
		return;

	for (int tile; fscanf(fp, "%d", &tile) && current < amount; ++current)
		layer->tiles[current] = tile;
}

static void
parse(struct map_data *data, const char *line, FILE *fp)
{
	if (strncmp(line, "title", 5) == 0)
		sscanf(line, "title|" MAX_F(MAP_TITLE_MAX), data->title);
	else if (strncmp(line, "width", 5) == 0)
		sscanf(line, "width|%u", &data->w);
	else if (strncmp(line, "height", 6) == 0)
		sscanf(line, "height|%u", &data->h);
	else if (strncmp(line, "tilewidth", 9) == 0)
		sscanf(line, "tilewidth|%hu", &data->tile_w);
	else if (strncmp(line, "tileheight", 10) == 0)
		sscanf(line, "tileheight|%hu", &data->tile_h);
	else if (strncmp(line, "origin", 6) == 0)
		sscanf(line, "origin|%d|%d", &data->origin_x, &data->origin_y);
	else if (strncmp(line, "tileset", 7) == 0)
		sscanf(line, "tileset|" MAX_F(MAP_TILESET_MAX), data->tileset);
	else if (strncmp(line, "layer", 5) == 0)
		parse_layer(data, line, fp);
}

static bool
check(struct map_data *data)
{
	if (strlen(data->title) == 0)
		return errorf("data has no title");
	if (data->w == 0 || data->h == 0)
		return errorf("data has null sizes");
	if (data->tile_w == 0 || data->tile_h == 0)
		return errorf("data has null tile sizes");
	if (!data->layers[0].tiles || !data->layers[1].tiles)
		return errorf("could not allocate data");

	return true;
}

static void
draw_layer(struct map *map, const struct map_layer *layer)
{
	assert(map);
	assert(layer);

	struct sprite sprite;
	int x = 0, y = 0;

	sprite_init(&sprite, map->tileset, map->data->tile_w, map->data->tile_h);

	for (unsigned int r = 0; r < map->data->w; ++r) {
		for (unsigned int c = 0; c < map->data->h; ++c) {
			unsigned int si = r * map->data->w + c;
			unsigned int sr = (layer->tiles[si] - 1) / sprite.ncols;
			unsigned int sc = (layer->tiles[si] - 1) % sprite.nrows;

			if (layer->tiles[si] != 0)
				sprite_draw(&sprite, sr, sc, x, y);

			x += map->data->tile_w;
		}

		x = 0;
		y += map->data->tile_h;
	}
}

bool
map_data_open_fp(struct map_data *data, FILE *fp)
{
	assert(data);

	char line[1024];

	if (!fp)
		return false;

	memset(data, 0, sizeof (*data));

	while (fgets(line, sizeof (line), fp)) {
		/* Remove \n if any */
		line[strcspn(line, "\n")] = '\0';
		parse(data, line, fp);
	}

	fclose(fp);

	if (!check(data)) {
		map_data_finish(data);
		return false;
	}

	/* Compute real size. */
	data->real_w = data->w * data->tile_w;
	data->real_h = data->h * data->tile_h;

	return true;
}

bool
map_data_open(struct map_data *data, const char *path)
{
	assert(data);
	assert(path);

	return map_data_open_fp(data, fopen(path, "r"));
}

bool
map_data_openmem(struct map_data *data, const void *buf, size_t bufsz)
{
	assert(data);
	assert(buf);

	return map_data_open_fp(data, fmemopen((void *)buf, bufsz, "r"));
}

void
map_data_finish(struct map_data *data)
{
	assert(data);

	free(data->layers[0].tiles);
	free(data->layers[1].tiles);

	memset(data, 0, sizeof (*data));
}

bool
map_init(struct map *map, struct map_data *data, struct texture *tileset)
{
	assert(map);
	assert(data);
	assert(tileset && texture_ok(tileset));

	if (!(texture_new(&map->picture, data->real_w, data->real_h)))
		return false;

	map->data = data;
	map->tileset = tileset;

	map_repaint(map);

	return true;
}

void
map_draw(struct map *map, int srcx, int srcy)
{
	texture_scale(&map->picture, srcx, srcy, window.w, window.h,
	    0, 0, window.w, window.h, 0.0);
}

void
map_repaint(struct map *map)
{
	PAINTER_BEGIN(&map->picture);
	draw_layer(map, &map->data->layers[0]);
	draw_layer(map, &map->data->layers[1]);
	PAINTER_END();
}

void
map_finish(struct map *map)
{
	assert(map);

	texture_finish(&map->picture);

	memset(map, 0, sizeof (*map));
}
