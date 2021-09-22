/*
 * assets.h -- global atlas for every resources
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

#ifndef MOLKO_ADVENTURE_ASSETS_H
#define MOLKO_ADVENTURE_ASSETS_H

#include <core/core.h>
#include <core/sound.h>
#include <core/sprite.h>

enum assets_sprite {
	/* UI elements. */
	ASSETS_SPRITE_UI_CURSOR,

	/* Actions. */
	ASSETS_SPRITE_CHEST,

	/* Characters enemies. */
	ASSETS_SPRITE_CHARACTER_BLACK_CAT,

	/* Team assets. */
	ASSETS_SPRITE_CHARACTER_NETH,
	ASSETS_SPRITE_CHARACTER_NETH_SWORD,
	ASSETS_SPRITE_FACES,

	/* Animations. */
	ASSETS_SPRITE_EXPLOSION,

	ASSETS_SPRITE_NUM
};

enum assets_sound {
	/* Items. */
	ASSETS_SOUND_ITEM_POTION,

	ASSETS_SOUND_NUM
};

CORE_BEGIN_DECLS

extern struct sprite assets_sprites[ASSETS_SPRITE_NUM];
extern struct sound assets_sounds[ASSETS_SOUND_NUM];

void
assets_init(void);

void
assets_finish(void);

CORE_END_DECLS

#endif /* !MOLKO_ADVENTURE_ASSETS_H */
