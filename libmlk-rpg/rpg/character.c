/*
 * character.c -- character definition
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

#include <assert.h>

#include <core/sprite.h>

#include "character.h"
#include "equipment.h"

bool
character_ok(struct character *ch)
{
	return ch && ch->name && ch->type && ch->reset && sprite_ok(ch->sprites[CHARACTER_SPRITE_NORMAL]);
}

const char *
character_status_string(enum character_status status)
{
	/*
	 * We expect the user to only specify one status as character_status
	 * is a bitmask.
	 */
	switch (status) {
	case CHARACTER_STATUS_POISON:
		return "poison";
	default:
		return "normal";
	}
}

void
character_reset(struct character *ch)
{
	assert(character_ok(ch));

	ch->reset(ch);

	/* For all equipments, apply its equip function. */
	for (int i = 0; i < CHARACTER_EQUIPMENT_NUM; ++i)
		if (ch->equipments[i])
			equipment_equip(ch->equipments[i], ch);
}

void
character_exec(struct character *ch, struct battle *bt)
{
	assert(character_ok(ch));

	if (ch->exec)
		ch->exec(ch, bt);
}