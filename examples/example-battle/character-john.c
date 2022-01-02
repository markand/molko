/*
 * character-john.c -- john character
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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

#include "character-john.h"
#include "spell-fire.h"
#include "registry.h"

static void
adventurer_reset(struct character *ch)
{
	/* TODO: this function should compute the attack thanks to the level. */
	ch->hpmax = 120;
	ch->mpmax = 50;
	ch->atk = 50;
	ch->def = 50;
	ch->agt = 50;
	ch->luck = 50;
}

const struct character character_john = {
	.name = "John ",
	.type = "Adventurer",
	.level = 1,
	.hp = 120,
	.mp = 50,
	.reset = adventurer_reset,
	.sprite = &registry_sprites[REGISTRY_TEXTURE_JOHN],
	.spells = {
		&spell_fire
	}
};
