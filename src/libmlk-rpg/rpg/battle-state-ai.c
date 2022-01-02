/*
 * battle-state-ai.c -- battle state (enemy is playing)
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

#include <assert.h>

#include "battle.h"
#include "battle-state.h"
#include "character.h"

static int
update(struct battle_state *st, struct battle *bt, unsigned int ticks)
{
	(void)st;
	(void)ticks;

	struct character *ch = bt->order_cur->ch;

	/*
	 * Immediately invoke the enemy exec strategy and put the battle state
	 * to check.
	 */
	character_exec(ch, bt);

	return 0;
}

void
battle_state_ai(struct battle *bt)
{
	assert(bt);

	static struct battle_state self = {
		.update = update
	};

	battle_switch(bt, &self);
}
