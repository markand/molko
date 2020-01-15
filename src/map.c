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

#include "error.h"
#include "error_p.h"
#include "map.h"
#include "image.h"
#include "texture.h"
#include "sprite.h"
#include "sys.h"
#include "painter.h"

#include <SDL.h>

/* Create %<v>c string literal for scanf */
#define MAX_F(v) MAX_F_(v)
#define MAX_F_(v) "%" #v "c"

static void
parse_layer(struct map *map, const char *line, FILE *fp)
{
	char layer_name[32 + 1] = { 0 };
	struct map_layer *layer;
	size_t amount, current;

	/* Determine layer. */
	if (sscanf(line, "layer|%32s", layer_name) <= 0)
		return;
	if (strcmp(layer_name, "background") == 0)
		layer = &map->layers[0];
	else if (strcmp(layer_name, "foreground") == 0)
		layer = &map->layers[1];
	else
		return;

	/* Check if weight/height has been specified. */
	if (map->width == 0 || map->height == 0)
		return;

	amount = map->width * map->height;
	current = 0;

	if (!(layer->tiles = calloc(amount, sizeof (uint16_t))))
		return;

	for (int tile; fscanf(fp, "%d", &tile) && current < amount; ++current)
		layer->tiles[current] = tile;
}

static void
parse_tileset(struct map *map, const char *line)
{
	char filename[128 + 1] = { 0 };

	sscanf(line, "tileset|%128s", filename);

	if (map->tilewidth == 0 || map->tileheight == 0)
		return;
	if (!(map->tileset = image_openf(sys_datapath("tilesets/%s", filename))))
		return;

	sprite_init(&map->sprite, map->tileset, map->tilewidth, map->tileheight);
}

static void
parse(struct map *map, const char *line, FILE *fp)
{
	if (strncmp(line, "title", 5) == 0)
		sscanf(line, "title|" MAX_F(MAP_TITLE_MAX), map->title);
	else if (strncmp(line, "width", 5) == 0)
		sscanf(line, "width|%hu", &map->width);
	else if (strncmp(line, "height", 6) == 0)
		sscanf(line, "height|%hu", &map->height);
	else if (strncmp(line, "tilewidth", 9) == 0)
		sscanf(line, "tilewidth|%hhu", &map->tilewidth);
	else if (strncmp(line, "tileheight", 10) == 0)
		sscanf(line, "tileheight|%hhu", &map->tileheight);
	else if (strncmp(line, "tileset", 7) == 0)
		parse_tileset(map, line);
	else if (strncmp(line, "layer", 5) == 0)
		parse_layer(map, line, fp);
}

static bool
check(struct map *map)
{
	if (strlen(map->title) == 0)
		return error_printf("map has no title");
	if (!map->tileset)
		return error_printf("unable to open tileset");
	if (map->width == 0 || map->height == 0)
		return error_printf("map has null sizes");
	if (map->tilewidth == 0 || map->tileheight == 0)
		return error_printf("map has null tile sizes");
	if (!map->layers[0].tiles || !map->layers[1].tiles)
		return error_printf("could not allocate data");

	return true;
}

static void
draw_layer(struct map *map, const struct map_layer *layer)
{
	assert(map);
	assert(layer);

	int16_t x = 0, y = 0;

	for (uint16_t r = 0; r < map->width; ++r) {
		for (uint16_t c = 0; c < map->height; ++c) {
			size_t si = r * map->width + c;
			size_t sr = (layer->tiles[si] - 1) / map->sprite.ncols;
			size_t sc = (layer->tiles[si] - 1) % map->sprite.nrows;

			if (layer->tiles[si] != 0)
				sprite_draw(&map->sprite, sr, sc, x, y);

			x += map->tilewidth;
		}

		x = 0;
		y += map->tileheight;
	}
}

bool
map_open(struct map *map, const char *path)
{
	assert(map);
	assert(path);

	memset(map, 0, sizeof (struct map));

	FILE *fp = fopen(path, "r");
	char line[BUFSIZ];

	if (!fp)
		return false;

	while (fgets(line, sizeof (line), fp)) {
		/* Remove \n if any */
		line[strcspn(line, "\n")] = '\0';
		parse(map, line, fp);
	}

	fclose(fp);

	if (!check(map)) {
		map_close(map);
		return false;
	}

	size_t pw = map->width * map->tilewidth;
	size_t ph = map->height * map->tileheight;

	if (!(map->picture = texture_new(pw, ph)))
		return error_sdl();

	return true;
}

void
map_draw(struct map *map, int srcx, int srcy)
{
	/* TODO: remove window size here */
	texture_draw_ex(
		map->picture,
		srcx,
		srcy,
		1024,
		576,
		0,
		0,
		1024,
		576,
		0
	);
}

void
map_repaint(struct map *map)
{
	struct texture *old;

	old = painter_get_target();
	painter_set_target(map->picture);
	draw_layer(map, &map->layers[0]);
	draw_layer(map, &map->layers[1]);
	painter_set_target(old);
}

void
map_close(struct map *map)
{
	assert(map);

	if (map->tileset)
		texture_close(map->tileset);
	if (map->picture)
		texture_close(map->picture);

	free(map->layers[0].tiles);
	free(map->layers[1].tiles);

	memset(&map, 0, sizeof (struct map));
}
