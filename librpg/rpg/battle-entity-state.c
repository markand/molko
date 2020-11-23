/*
 * battle-entity-state.c -- abstract battle entity state
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

#include "battle-entity.h"
#include "battle-entity-state.h"

bool
battle_entity_state_update(struct battle_entity_state *st, struct battle_entity *et, unsigned int ticks)
{
	assert(st);
	assert(et);

	if (st->update)
		return st->update(st, et, ticks);

	return true;
}

void
battle_entity_state_draw(const struct battle_entity_state *st, const struct battle_entity *et)
{
	assert(st);

	if (st->draw)
		st->draw(st, et);
	else
		battle_entity_draw_sprite(et);
}

void
battle_entity_state_finish(struct battle_entity_state *st, struct battle_entity *et)
{
	assert(et);

	if (st->finish)
		st->finish(st, et);
}
