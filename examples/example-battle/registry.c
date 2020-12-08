/*
 * registry.h -- registry of resources
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

#include <stddef.h>

#include <core/image.h>
#include <core/panic.h>
#include <core/util.h>
#include <core/sys.h>

#include "registry.h"

#define PATH(r) util_pathf("%s/mlk-adventure/%s", sys_dir(SYS_DIR_DATA), r)

struct texture registry_images[REGISTRY_IMAGE_NUM];
struct texture registry_textures[REGISTRY_TEXTURE_NUM];
struct sprite registry_sprites[REGISTRY_TEXTURE_NUM];
struct sound registry_sounds[REGISTRY_SOUND_NUM];

#define REGISTRY_IMAGE(i, path) \
	{ (i), (path) }

static const struct {
	enum registry_image index;
	const char *path;
} images[] = {
	REGISTRY_IMAGE(REGISTRY_IMAGE_BATTLE_BACKGROUND, "images/battle-background.png")
};

#define REGISTRY_TEXTURE(s, path, cw, ch) \
	{ (s), (path), (cw), (ch) }

static const struct {
	enum registry_texture index;
	const char *path;
	unsigned int cellw;
	unsigned int cellh;
} textures[] = {
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_CURSOR, "sprites/cursor.png", 24, 24),
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_EXPLOSION, "sprites/explosion.png", 256, 256),
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_JOHN_SWORD, "sprites/john-sword.png", 256, 256),
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_JOHN_WALK, "sprites/john-walk.png", 256, 256),
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_HAUNTED_WOOD, "images/haunted-wood.png", 0, 0),
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_BLACK_CAT, "images/black-cat.png", 0, 0)
};

#define REGISTRY_SOUND(s, path) \
	{ (s), (path) }

static const struct {
	enum registry_sound index;
	const char *path;
} sounds[] = {
	REGISTRY_SOUND(REGISTRY_SOUND_FIRE, "sounds/fire.wav")
};

static void
load_images(void)
{
	for (size_t i = 0; i < UTIL_SIZE(images); ++i) {
		struct texture *texture = &registry_images[images[i].index];

		if (!image_open(texture, PATH(images[i].path)))
			panic();
	}
}

static void
load_textures_and_sprites(void)
{
	for (size_t i = 0; i < UTIL_SIZE(textures); ++i) {
		struct texture *texture = &registry_textures[textures[i].index];
		struct sprite *sprite = &registry_sprites[textures[i].index];

		if (!image_open(texture, PATH(textures[i].path)))
			panic();

		if (textures[i].cellw == 0 || textures[i].cellh == 0)
			sprite_init(sprite, texture, texture->w, texture->h);
		else
			sprite_init(sprite, texture, textures[i].cellw, textures[i].cellh);
	}
}

static void
load_sounds(void)
{
	for (size_t i = 0; i < UTIL_SIZE(sounds); ++i) {
		struct sound *sound = &registry_sounds[sounds[i].index];

		if (!sound_open(sound, PATH(sounds[i].path)))
			panic();
	}
}

void
registry_init(void)
{
	load_images();
	load_textures_and_sprites();
	load_sounds();
}

void
registry_finish(void)
{
	for (size_t i = 0; i < UTIL_SIZE(registry_images); ++i)
		texture_finish(&registry_images[i]);
	for (size_t i = 0; i < UTIL_SIZE(registry_textures); ++i)
		texture_finish(&registry_textures[i]);
	for (size_t i = 0; i < UTIL_SIZE(registry_sounds); ++i)
		sound_finish(&registry_sounds[i]);
}
