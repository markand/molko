/*
 * registry.h -- registry of resources
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

#include <stddef.h>

#include <mlk/core/image.h>
#include <mlk/core/panic.h>
#include <mlk/core/util.h>
#include <mlk/core/sys.h>

#include <assets/images/battle-background.h>
#include <assets/images/black-cat.h>
#include <assets/images/haunted-wood.h>

#include <assets/sounds/fire.h>

#include <assets/sprites/explosion.h>
#include <assets/sprites/john-sword.h>
#include <assets/sprites/john-walk.h>
#include <assets/sprites/ui-cursor.h>

#include "registry.h"

struct texture registry_images[REGISTRY_IMAGE_NUM];
struct texture registry_textures[REGISTRY_TEXTURE_NUM];
struct sprite registry_sprites[REGISTRY_TEXTURE_NUM];
struct mlk_sound registry_sounds[REGISTRY_SOUND_NUM];

#define REGISTRY_IMAGE(i, data) \
	{ (i), (data), sizeof (data) }

static const struct {
	enum registry_image index;
	const unsigned char *data;
	size_t datasz;
} images[] = {
	REGISTRY_IMAGE(REGISTRY_IMAGE_BATTLE_BACKGROUND, assets_images_battle_background)
};

#define REGISTRY_TEXTURE(s, data, cw, ch) \
	{ (s), (data), sizeof (data), (cw), (ch) }

static const struct {
	enum registry_texture index;
	const unsigned char *data;
	size_t datasz;
	unsigned int cellw;
	unsigned int cellh;
} textures[] = {
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_CURSOR, assets_sprites_ui_cursor, 24, 24),
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_EXPLOSION, assets_sprites_explosion, 256, 256),
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_JOHN_SWORD, assets_sprites_john_sword, 256, 256),
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_JOHN_WALK, assets_sprites_john_walk, 256, 256),
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_HAUNTED_WOOD, assets_images_haunted_wood, 0, 0),
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_BLACK_CAT, assets_images_black_cat, 0, 0)
};

#define REGISTRY_SOUND(s, data) \
	{ (s), (data), sizeof (data) }

static const struct {
	enum registry_sound index;
	const unsigned char *data;
	size_t datasz;
} sounds[] = {
	REGISTRY_SOUND(REGISTRY_SOUND_FIRE, assets_sounds_fire)
};

static void
load_images(void)
{
	for (size_t i = 0; i < UTIL_SIZE(images); ++i) {
		struct texture *texture = &registry_images[images[i].index];

		if (mlk_image_openmem(texture, images[i].data, images[i].datasz) < 0)
			mlk_panic();
	}
}

static void
load_textures_and_sprites(void)
{
	for (size_t i = 0; i < UTIL_SIZE(textures); ++i) {
		struct texture *texture = &registry_textures[textures[i].index];
		struct sprite *sprite = &registry_sprites[textures[i].index];

		if (mlk_image_openmem(texture, textures[i].data, textures[i].datasz) < 0)
			mlk_panic();

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
		struct mlk_sound *sound = &registry_sounds[sounds[i].index];

		if (mlk_sound_openmem(sound, sounds[i].data, sounds[i].datasz) < 0)
			mlk_panic();
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
		mlk_sound_finish(&registry_sounds[i]);
}
