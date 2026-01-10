/*
 * character.c -- character definition
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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

#include <mlk/core/sprite.h>

#include <assets/sql/character-save.h>
#include <assets/sql/character-load.h>

#include "character.h"
#include "equipment.h"
#include "save.h"

int
character_ok(const struct character *ch)
{
	return ch && ch->name && ch->reset && mlk_sprite_ok(ch->sprites[CHARACTER_SPRITE_NORMAL]);
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

int
character_save(const struct character *ch, struct save *s)
{
	assert(ch);
	assert(save_ok(s));

	return save_exec(s, (const char *)assets_sql_character_save, "s iii i iiiiii",
	    ch->name,
	    ch->hp,
	    ch->mp,
	    ch->level,
	    ch->team_order,
	    ch->hpbonus,
	    ch->mpbonus,
	    ch->atkbonus,
	    ch->defbonus,
	    ch->agtbonus,
	    ch->luckbonus
	);
}

int
character_load(struct character *ch, struct save *s)
{
	assert(ch);
	assert(save_ok(s));

	struct save_stmt stmt;
	enum save_stmt_errno ret;

	if (save_stmt_init(&stmt, s, (const char *)assets_sql_character_load, "s", ch->name) < 0)
		return -1;

	ret = save_stmt_next(&stmt, "iii i iiiiii",
	    &ch->hp,
	    &ch->mp,
	    &ch->level,
	    &ch->team_order,
	    &ch->hpbonus,
	    &ch->mpbonus,
	    &ch->atkbonus,
	    &ch->defbonus,
	    &ch->agtbonus,
	    &ch->luckbonus
	) == SAVE_STMT_ROW;

	save_stmt_finish(&stmt);

	return ret ? 0 : -1;
}
