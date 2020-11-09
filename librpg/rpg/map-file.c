/*
 * map-file.c -- map file loader
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
#include <string.h>

#include <core/alloc.h>
#include <core/error.h>
#include <core/image.h>

#include "map-file.h"

/* Create %<v>c string literal for scanf */
#define MAX_F(v) MAX_F_(v)
#define MAX_F_(v) "%" #v "c"

struct parser {
	struct map_file *mf;            /* Map loader. */
	struct map *map;                /* Map object to fill. */
	FILE *fp;                       /* Map file pointer. */
	char basedir[PATH_MAX];         /* Parent map directory */
};

static bool
parse_layer(struct parser *ps, const char *line)
{
	char layer_name[32 + 1] = {0};
	enum map_layer_type layer_type;
	size_t amount, current;

	/* Check if weight/height has been specified. */
	if (ps->map->w == 0 || ps->map->h == 0)
		return errorf("missing map dimensions before layer");

	/* Determine layer type. */
	if (sscanf(line, "layer|%32s", layer_name) <= 0)
		return errorf("missing layer type definition");

	if (strcmp(layer_name, "background") == 0)
		layer_type = MAP_LAYER_TYPE_BACKGROUND;
	else if (strcmp(layer_name, "foreground") == 0)
		layer_type = MAP_LAYER_TYPE_FOREGROUND;
	else
		return errorf("invalid layer type: %s", layer_name);

	amount = ps->map->w * ps->map->h;
	current = 0;

	/*
	 * The next line after a layer declaration is a list of plain integer
	 * that fill the layer tiles.
	 */
	if (!(ps->mf->layers[layer_type].tiles = alloc_zero(amount, sizeof (unsigned short))))
		return false;

	for (int tile; fscanf(ps->fp, "%d", &tile) && current < amount; ++current)
		ps->mf->layers[layer_type].tiles[current] = tile;

	ps->map->layers[layer_type].tiles = ps->mf->layers[layer_type].tiles;

	return true;
}

static bool
parse_tileset(struct parser *ps, const char *line)
{
	char filename[FILENAME_MAX + 1] = {0};
	char filepath[PATH_MAX];
	int ret;

	if (ps->map->tile_w == 0 || ps->map->tile_h == 0)
		return errorf("missing map dimensions before tileset");

	if ((ret = sscanf(line, "tileset|" MAX_F(FILENAME_MAX), filename)) == 1) {
		snprintf(filepath, sizeof (filepath), "%s/%s", ps->basedir, filename);

		if (!image_open(&ps->mf->tileset, filepath))
			return false;
	}

	/* Initialize sprite. */
	sprite_init(&ps->mf->sprite, &ps->mf->tileset, ps->map->tile_w, ps->map->tile_h);
	ps->map->tileset = &ps->mf->sprite;

	return true;
}

static bool
parse_title(struct parser *ps, const char *line)
{
	if (sscanf(line, "title|" MAX_F(MAP_FILE_TITLE_MAX), ps->mf->title) != 1 || strlen(ps->mf->title) == 0)
		return errorf("null map title");

	ps->map->title = ps->mf->title;

	return true;
}

static bool
parse_width(struct parser *ps, const char *line)
{
	if (sscanf(line, "width|%u", &ps->map->w) != 1 || ps->map->w == 0)
		return errorf("null map width");

	return true;
}

static bool
parse_height(struct parser *ps, const char *line)
{
	if (sscanf(line, "height|%u", &ps->map->h) != 1 || ps->map->h == 0)
		return errorf("null map height");

	return true;
}

static bool
parse_tilewidth(struct parser *ps, const char *line)
{
	if (sscanf(line, "tilewidth|%hu", &ps->map->tile_w) != 1 || ps->map->tile_w == 0)
		return errorf("null map tile width");
	if (ps->map->w == 0)
		return errorf("missing map width before tilewidth");

	ps->map->real_w = ps->map->w * ps->map->tile_w;

	return true;
}

static bool
parse_tileheight(struct parser *ps, const char *line)
{
	if (sscanf(line, "tileheight|%hu", &ps->map->tile_h) != 1 || ps->map->tile_h == 0)
		return errorf("null map tile height");
	if (ps->map->h == 0)
		return errorf("missing map height before tileheight");

	ps->map->real_h = ps->map->h * ps->map->tile_h;

	return true;
}

static bool
parse_origin(struct parser *ps, const char *line)
{
	if (sscanf(line, "origin|%d|%d", &ps->map->origin_x, &ps->map->origin_y) != 2)
		return errorf("invalid origin");

	/*
	 * We adjust the player position here because it should not be done in
	 * the map_init function. This is because the player should not move
	 * magically each time we re-use the map (saving position).
	 */
	ps->map->player_x = ps->map->origin_x;
	ps->map->player_y = ps->map->origin_y;

	return true;
}

static bool
parse_line(struct parser *ps, const char *line)
{
	static const struct {
		const char *property;
		bool (*read)(struct parser *, const char *);
	} props[] = {
		{ "title",      parse_title             },
		{ "width",      parse_width             },
		{ "height",     parse_height            },
		{ "tilewidth",  parse_tilewidth         },
		{ "tileheight", parse_tileheight        },
		{ "tileset",    parse_tileset           },
		{ "origin",     parse_origin            },
		{ "layer",      parse_layer             }
	};

	for (size_t i = 0; i < NELEM(props); ++i)
		if (strncmp(line, props[i].property, strlen(props[i].property)) == 0)
			return props[i].read(ps, line);

	return true;
}

static bool
parse(struct map_file *loader, const char *path, struct map *map, FILE *fp)
{
	char line[1024];
	struct parser ps = {
		.mf = loader,
		.map = map,
		.fp = fp
	};

	/*
	 * Even though dirname(3) usually not modify the path as argument it may
	 * do according to POSIX specification, as such we still need a
	 * temporary buffer.
	 */
	snprintf(ps.basedir, sizeof (ps.basedir), "%s", path);
	snprintf(ps.basedir, sizeof (ps.basedir), "%s", dirname(ps.basedir));

	while (fgets(line, sizeof (line), fp)) {
		/* Remove \n if any */
		line[strcspn(line, "\n")] = '\0';

		if (!parse_line(&ps, line))
			return false;
	}

	return true;
}

static bool
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
	if (!sprite_ok(map->tileset))
		return errorf("missing tileset");

	return true;
}

bool
map_file_open(struct map_file *file, const char *path, struct map *map)
{
	assert(file);
	assert(path);
	assert(map);

	FILE *fp;
	bool ret = true;

	memset(file, 0, sizeof (*file));
	memset(map, 0, sizeof (*map));

	if (!(fp = fopen(path, "r")))
		return errorf("%s", strerror(errno));

	if (!(ret = parse(file, path, map, fp)) || !(ret = check(map))) {
		map_finish(map);
		map_file_finish(file);
	}

	fclose(fp);

	return ret;
}

void
map_file_finish(struct map_file *file)
{
	assert(file);

	texture_finish(&file->tileset);
	memset(file, 0, sizeof (*file));
}
