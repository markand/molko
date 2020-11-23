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

bool
character_ok(struct character *ch)
{
	return ch && ch->name && ch->type && ch->reset && sprite_ok(ch->sprites[CHARACTER_SPRITE_WALK]);
}

const char *
character_status_string(enum character_status status)
{
	/* We have to use a switch-case as it is a bitmask. */
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
	assert(ch);

	ch->reset(ch);
}

void
character_exec(struct character *ch, struct battle *bt)
{
	assert(character_ok(ch));

	if (ch->exec)
		ch->exec(ch, bt);
}
