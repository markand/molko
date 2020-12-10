/*
 * assets.c -- global atlas for every resources
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

#include <core/image.h>
#include <core/panic.h>
#include <core/sprite.h>
#include <core/texture.h>
#include <core/util.h>

#include <adventure/molko.h>

#include "assets.h"

#define SPRITE(which, file, w, h) { which, file, w, h }

static struct {
	enum assets_sprite index;
	const char *path;
	unsigned int cellw;
	unsigned int cellh;
	struct texture texture;
	struct sprite sprite;
} table_sprites[] = {
	SPRITE(ASSETS_SPRITE_UI_CURSOR, "sprites/ui-cursor.png", 24, 24)
};

struct sprite *assets_sprites[ASSETS_SPRITE_NUM] = {0};

static void
init_sprites(void)
{
	for (size_t i = 0; i < UTIL_SIZE(table_sprites); ++i) {
		if (!image_open(&table_sprites[i].texture, molko_path(table_sprites[i].path)))
			panic();

		sprite_init(&table_sprites[i].sprite, &table_sprites[i].texture,
		    table_sprites[i].cellw, table_sprites[i].cellh);

		assets_sprites[table_sprites[i].index] = &table_sprites[i].sprite;
	}
}

void
assets_init(void)
{
	init_sprites();
}

void
assets_finish(void)
{
	for (size_t i = 0; i < UTIL_SIZE(table_sprites); ++i)
		texture_finish(&table_sprites[i].texture);
}
