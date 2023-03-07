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

#ifndef EXAMPLES_BATTLE_REGISTRY_H
#define EXAMPLES_BATTLE_REGISTRY_H

#include <mlk/core/music.h>
#include <mlk/core/sound.h>
#include <mlk/core/sprite.h>
#include <mlk/core/texture.h>

enum mlk_registry_texture {
	/* UI. */
	MLK_REGISTRY_TEXTURE_CURSOR,
	MLK_REGISTRY_TEXTURE_NUMBERS,

	/* Animations. */
	MLK_REGISTRY_TEXTURE_EXPLOSION,
	MLK_REGISTRY_TEXTURE_WATER,

	/* Characters. */
	MLK_REGISTRY_TEXTURE_JOHN,
	MLK_REGISTRY_TEXTURE_JOHN_WALK,
	MLK_REGISTRY_TEXTURE_JOHN_SWORD,
	MLK_REGISTRY_TEXTURE_PEOPLE,

	/* Enemies. */
	MLK_REGISTRY_TEXTURE_HAUNTED_WOOD,
	MLK_REGISTRY_TEXTURE_BLACK_CAT,

	/* Objects. */
	MLK_REGISTRY_TEXTURE_CHEST,

	/* Sword by Icongeek26 (https://www.flaticon.com). */
	MLK_REGISTRY_TEXTURE_SWORD,

	/* Tileset textures. */
	MLK_REGISTRY_TEXTURE_WORLD,

	/* Unused.*/
	MLK_REGISTRY_TEXTURE_LAST
};

enum mlk_registry_image {
	MLK_REGISTRY_IMAGE_BATTLE_BACKGROUND,
	MLK_REGISTRY_IMAGE_LAST
};

enum mlk_registry_sound {
	MLK_REGISTRY_SOUND_FIRE,
	MLK_REGISTRY_SOUND_OPEN_CHEST,
	MLK_REGISTRY_SOUND_LAST
};

enum mlk_registry_music {
	MLK_REGISTRY_MUSIC_ROMANCE,
	MLK_REGISTRY_MUSIC_LAST
};

extern struct mlk_texture mlk_registry_images[MLK_REGISTRY_IMAGE_LAST];
extern struct mlk_texture mlk_registry_textures[MLK_REGISTRY_TEXTURE_LAST];
extern struct mlk_sprite mlk_registry_sprites[MLK_REGISTRY_TEXTURE_LAST];
extern struct mlk_sound mlk_registry_sounds[MLK_REGISTRY_SOUND_LAST];
extern struct mlk_music mlk_registry_music[MLK_REGISTRY_MUSIC_LAST];

void
mlk_registry_init(void);

void
mlk_registry_finish(void);

#endif /* !EXAMPLES_BATTLE_REGISTRY_H */
