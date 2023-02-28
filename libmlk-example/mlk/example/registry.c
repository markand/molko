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
#include <assets/images/sword.h>

#include <assets/sounds/fire.h>
#include <assets/sounds/open-chest.h>

#include <assets/sprites/chest.h>
#include <assets/sprites/explosion.h>
#include <assets/sprites/john-sword.h>
#include <assets/sprites/john-walk.h>
#include <assets/sprites/numbers.h>
#include <assets/sprites/people.h>
#include <assets/sprites/ui-cursor.h>

#include <assets/music/vabsounds-romance.h>

#include "registry.h"

struct mlk_texture registry_images[REGISTRY_IMAGE_LAST] = {0};
struct mlk_texture registry_textures[REGISTRY_TEXTURE_LAST] = {0};
struct mlk_sprite registry_sprites[REGISTRY_TEXTURE_LAST] = {0};
struct mlk_sound registry_sounds[REGISTRY_SOUND_LAST] = {0};
struct mlk_music registry_music[REGISTRY_MUSIC_LAST] = {0};

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
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_BLACK_CAT, assets_images_black_cat, 0, 0),
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_CHEST, assets_sprites_chest, 32, 32),
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_NUMBERS, assets_sprites_numbers, 48, 48),
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_SWORD, assets_images_sword, 0, 0),
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_PEOPLE, assets_sprites_people, 48, 48)
};

#define REGISTRY_SOUND(s, data) \
	{ (s), (data), sizeof (data) }

static const struct {
	enum registry_sound index;
	const unsigned char *data;
	size_t datasz;
} sounds[] = {
	REGISTRY_SOUND(REGISTRY_SOUND_FIRE, assets_sounds_fire),
	REGISTRY_SOUND(REGISTRY_SOUND_OPEN_CHEST, assets_sounds_open_chest)
};

#define REGISTRY_MUSIC(m, data) \
	{ (m), (data), sizeof (data) }

static const struct {
	enum registry_music index;
	const unsigned char *data;
	size_t datasz;
} musics[] = {
	REGISTRY_MUSIC(REGISTRY_MUSIC_ROMANCE, assets_music_vabsounds_romance)
};

static void
load_images(void)
{
	int err;
	struct mlk_texture *texture;

	for (size_t i = 0; i < MLK_UTIL_SIZE(images); ++i) {
		texture = &registry_images[images[i].index];

		if ((err = mlk_image_openmem(texture, images[i].data, images[i].datasz)) < 0)
			mlk_panic(err);
	}
}

static void
load_textures_and_sprites(void)
{
	struct mlk_texture *texture;
	struct mlk_sprite *sprite;
	int err;

	for (size_t i = 0; i < MLK_UTIL_SIZE(textures); ++i) {
		texture = &registry_textures[textures[i].index];
		sprite = &registry_sprites[textures[i].index];

		if ((err = mlk_image_openmem(texture, textures[i].data, textures[i].datasz)) < 0)
			mlk_panic(err);

		if (textures[i].cellw == 0 || textures[i].cellh == 0)
			mlk_sprite_init(sprite, texture, texture->w, texture->h);
		else
			mlk_sprite_init(sprite, texture, textures[i].cellw, textures[i].cellh);
	}
}

static void
load_sounds(void)
{
	int err;
	struct mlk_sound *sound;

	for (size_t i = 0; i < MLK_UTIL_SIZE(sounds); ++i) {
		sound = &registry_sounds[sounds[i].index];

		if ((err = mlk_sound_openmem(sound, sounds[i].data, sounds[i].datasz)) < 0)
			mlk_panic(err);
	}
}

static void
load_music(void)
{
	int err;
	struct mlk_music *music;

	for (size_t i = 0; i < MLK_UTIL_SIZE(musics); ++i) {
		music = &registry_music[musics[i].index];

		if ((err = mlk_music_openmem(music, musics[i].data, musics[i].datasz)) < 0)
			mlk_panic(err);
	}
}

void
registry_init(void)
{
	load_images();
	load_textures_and_sprites();
	load_sounds();
	load_music();
}

void
registry_finish(void)
{
	for (size_t i = 0; i < MLK_UTIL_SIZE(registry_images); ++i)
		mlk_texture_finish(&registry_images[i]);
	for (size_t i = 0; i < MLK_UTIL_SIZE(registry_textures); ++i)
		mlk_texture_finish(&registry_textures[i]);
	for (size_t i = 0; i < MLK_UTIL_SIZE(registry_sounds); ++i)
		mlk_sound_finish(&registry_sounds[i]);
	for (size_t i = 0; i < MLK_UTIL_SIZE(registry_music); ++i)
		mlk_music_finish(&registry_music[i]);
}
