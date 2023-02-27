/*
 * battle-state-ai.c -- battle state (enemy is playing)
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

#include <assert.h>
#include <stdlib.h>

#include <mlk/core/alloc.h>

#include "battle-state-ai.h"
#include "battle-state.h"
#include "battle.h"
#include "character.h"

static int
update(struct battle_state *st, struct battle *bt, unsigned int ticks)
{
	(void)st;
	(void)ticks;

	battle_state_ai_update(bt);

	return 0;
}

static void
draw(const struct battle_state *st, const struct battle *bt)
{
	(void)st;

	battle_state_ai_draw(bt);
}

static void
finish(struct battle_state *st, struct battle *bt)
{
	(void)bt;

	mlk_alloc_free(st);
}

void
battle_state_ai_update(struct battle *bt)
{
	assert(battle_ok(bt));

	struct character *ch = battle_current(bt)->ch;

	/*
	 * Immediately invoke the enemy exec strategy and put the battle state
	 * to check.
	 */
	character_exec(ch, bt);
}

void
battle_state_ai_draw(const struct battle *bt)
{
	assert(battle_ok(bt));

	battle_draw_component(bt, BATTLE_COMPONENT_ALL);
}

void
battle_state_ai(struct battle *bt)
{
	assert(bt);

	struct battle_state *self;

	self = mlk_alloc_new0(1, sizeof (*self));
	self->data = bt;
	self->update = update;
	self->draw = draw;
	self->finish = finish;

	battle_switch(bt, self);
}
