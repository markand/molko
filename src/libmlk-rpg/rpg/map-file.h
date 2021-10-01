/*
 * map-file.h -- map file loader
 *
 * Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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

#ifndef MOLKO_RPG_MAP_FILE_H
#define MOLKO_RPG_MAP_FILE_H

#include <core/alloc.h>
#include <core/core.h>
#include <core/sprite.h>
#include <core/texture.h>

#include "map.h"
#include "tileset.h"
#include "tileset-file.h"

#define MAP_FILE_TITLE_MAX 64

struct map_file {
	void (*load_action)(struct map *, int, int, int, int, const char *);

	char title[MAP_FILE_TITLE_MAX];
	struct map_layer layers[MAP_LAYER_TYPE_NUM];
	struct tileset_file tileset_file;
	struct tileset tileset;
	struct alloc_pool blocks;
};

CORE_BEGIN_DECLS

int
map_file_open(struct map_file *file, struct map *map, const char *path);

void
map_file_finish(struct map_file *file);

CORE_END_DECLS

#endif /* !MOLKO_RPG_MAP_FILE_H */
