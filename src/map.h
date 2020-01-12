/*
 * map.h -- game map
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

#ifndef MOLKO_MAP_H
#define MOLKO_MAP_H

/**
 * \file map.h
 * \brief Game map.
 */

#include <stdbool.h>
#include <stdint.h>

#include "sprite.h"

#define MAP_TITLE_MAX   32

struct texture;

struct map_layer {
	uint16_t *tiles;
};

struct map {
	char title[MAP_TITLE_MAX];
	struct texture *tileset;
	struct sprite sprite;
	uint16_t width;
	uint16_t height;
	uint8_t tilewidth;
	uint8_t tileheight;
	struct map_layer layers[2];
};

bool
map_open(struct map *map, const char *path);

void
map_close(struct map *map);

#endif /* !MOLKO_MAP_H */
