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
#include <core/sound.h>
#include <core/sprite.h>
#include <core/texture.h>
#include <core/util.h>

#include <ui/theme.h>

#include <adventure/molko.h>

#include "assets.h"

#define SPRITE(which, file, w, h)       { which, file, w, h }
#define SOUND(which, file)              { which, file }

static struct {
	enum assets_sprite index;
	const char *path;
	unsigned int cellw;
	unsigned int cellh;
	struct texture texture;
} table_sprites[] = {
	SPRITE(ASSETS_SPRITE_UI_CURSOR, "sprites/ui-cursor.png", 24, 24),
	SPRITE(ASSETS_SPRITE_CHEST, "sprites/chest.png", 32, 32),
	SPRITE(ASSETS_SPRITE_CHARACTER_BLACK_CAT, "images/black-cat.png", 123, 161),
	SPRITE(ASSETS_SPRITE_CHARACTER_NETH, "sprites/john-walk.png", 256, 256),
	SPRITE(ASSETS_SPRITE_CHARACTER_NETH_SWORD, "sprites/john-sword.png", 256, 256),
	SPRITE(ASSETS_SPRITE_FACES, "sprites/faces.png", 144, 144),
	SPRITE(ASSETS_SPRITE_EXPLOSION, "sprites/explosion.png", 256, 256)
};

static struct {
	enum assets_sound index;
	const char *path;
} table_sounds[] = {
	SOUND(ASSETS_SOUND_ITEM_POTION, "sounds/potion.wav")
};

struct sprite assets_sprites[ASSETS_SPRITE_NUM];
struct sound assets_sounds[ASSETS_SOUND_NUM];

static void
init_sprites(void)
{
	for (size_t i = 0; i < UTIL_SIZE(table_sprites); ++i) {
		if (image_open(&table_sprites[i].texture, molko_path(table_sprites[i].path)) < 0)
			panic();

		sprite_init(&assets_sprites[table_sprites[i].index],
		    &table_sprites[i].texture,
		    table_sprites[i].cellw, table_sprites[i].cellh);
	}
}

static void
init_sounds(void)
{
	for (size_t i = 0; i < UTIL_SIZE(assets_sounds); ++i) {
		if (sound_open(&assets_sounds[table_sounds[i].index], molko_path(table_sounds[i].path)) < 0)
			panic();
	}
}

void
assets_init(void)
{
	init_sprites();
	init_sounds();

	/* Prepare the theme. */
	theme_default()->sprites[THEME_SPRITE_CURSOR] = &assets_sprites[ASSETS_SPRITE_UI_CURSOR];
}

void
assets_finish(void)
{
	for (size_t i = 0; i < UTIL_SIZE(table_sprites); ++i)
		texture_finish(&table_sprites[i].texture);
	for (size_t i = 0; i < UTIL_SIZE(table_sounds); ++i)
		sound_finish(&assets_sounds[i]);
}
