/*
 * neth.c -- Neth
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

#include <rpg/character.h>

#include <adventure/adventure_p.h>
#include <adventure/assets.h>

#include "neth.h"

static void
reset(struct character *ch)
{
	/* TODO: compute hpmax given the level. */
	ch->hpmax = 570;
	ch->mpmax = 50;
	ch->atk = 22;
	ch->def = 19;
	ch->agt = 16;
	ch->luck = 3;
}

struct character character_neth = {
	.name = N_("Neth"),
	.level = 1,
	.sprites = {
		[CHARACTER_SPRITE_NORMAL] = &assets_sprites[ASSETS_SPRITE_CHARACTER_NETH],
		[CHARACTER_SPRITE_SWORD] = &assets_sprites[ASSETS_SPRITE_CHARACTER_NETH_SWORD]
	},
	.reset = reset
};
