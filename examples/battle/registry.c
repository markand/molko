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

#include <adventure/assets/sprites/john.h>

#include <assets/images/haunted-wood.h>
#include <assets/images/black-cat.h>

#include <assets/sprites/cursor.h>
#include <assets/sprites/explosion.h>

#include <assets/sounds/fire.h>

#include "registry.h"

struct texture registry_textures[REGISTRY_TEXTURE_NUM];
struct sprite registry_sprites[REGISTRY_TEXTURE_NUM];
struct sound registry_sounds[REGISTRY_SOUND_NUM];

#define REGISTRY_TEXTURE(s, ptr, cw, ch) \
	{ (s), (ptr), sizeof ((ptr)), (cw), (ch) }

static const struct {
	enum registry_texture index;
	const void *data;
	size_t datasz;
	unsigned int cellw;
	unsigned int cellh;
} textures[] = {
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_CURSOR, sprites_cursor, 24, 24),
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_EXPLOSION, sprites_explosion, 256, 256),
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_JOHN, sprites_john, 48, 48),
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_HAUNTED_WOOD, images_haunted_wood, 0, 0),
	REGISTRY_TEXTURE(REGISTRY_TEXTURE_BLACK_CAT, images_black_cat, 0, 0)
};

#define REGISTRY_SOUND(s, ptr) \
	{ (s), (ptr), sizeof ((ptr)) }

static const struct {
	enum registry_sound index;
	const void *data;
	size_t datasz;
} sounds[] = {
	REGISTRY_SOUND(REGISTRY_SOUND_FIRE, sounds_fire)
};

static void
load_textures_and_sprites(void)
{
	for (size_t i = 0; i < NELEM(textures); ++i) {
		struct texture *texture = &registry_textures[textures[i].index];
		struct sprite *sprite = &registry_sprites[textures[i].index];

		if (!image_openmem(texture, textures[i].data, textures[i].datasz))
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
	for (size_t i = 0; i < NELEM(sounds); ++i) {
		struct sound *sound = &registry_sounds[sounds[i].index];

		if (!sound_openmem(sound, sounds[i].data, sounds[i].datasz))
			panic();
	}
}

void
registry_init(void)
{
	load_textures_and_sprites();
	load_sounds();
}

void
registry_finish(void)
{
	for (size_t i = 0; i < NELEM(registry_textures); ++i)
		texture_finish(&registry_textures[i]);
	for (size_t i = 0; i < NELEM(registry_sounds); ++i)
		sound_finish(&registry_sounds[i]);
}
