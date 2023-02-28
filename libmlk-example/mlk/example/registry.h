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

enum registry_texture {
	/* UI. */
	REGISTRY_TEXTURE_CURSOR,
	REGISTRY_TEXTURE_NUMBERS,

	/* Animations. */
	REGISTRY_TEXTURE_EXPLOSION,

	/* Characters. */
	REGISTRY_TEXTURE_JOHN_WALK,
	REGISTRY_TEXTURE_JOHN_SWORD,
	REGISTRY_TEXTURE_PEOPLE,

	/* Enemies. */
	REGISTRY_TEXTURE_HAUNTED_WOOD,
	REGISTRY_TEXTURE_BLACK_CAT,

	/* Objects. */
	REGISTRY_TEXTURE_CHEST,

	/* Sword by Icongeek26 (https://www.flaticon.com). */
	REGISTRY_TEXTURE_SWORD,

	/* Unused.*/
	REGISTRY_TEXTURE_LAST
};

enum registry_image {
	REGISTRY_IMAGE_BATTLE_BACKGROUND,
	REGISTRY_IMAGE_LAST
};

enum registry_sound {
	REGISTRY_SOUND_FIRE,
	REGISTRY_SOUND_OPEN_CHEST,
	REGISTRY_SOUND_LAST
};

enum registry_music {
	REGISTRY_MUSIC_ROMANCE,
	REGISTRY_MUSIC_LAST
};

extern struct mlk_texture registry_images[REGISTRY_IMAGE_LAST];
extern struct mlk_texture registry_textures[REGISTRY_TEXTURE_LAST];
extern struct mlk_sprite registry_sprites[REGISTRY_TEXTURE_LAST];
extern struct mlk_sound registry_sounds[REGISTRY_SOUND_LAST];
extern struct mlk_music registry_music[REGISTRY_MUSIC_LAST];

void
registry_init(void);

void
registry_finish(void);

#endif /* !EXAMPLES_BATTLE_REGISTRY_H */
