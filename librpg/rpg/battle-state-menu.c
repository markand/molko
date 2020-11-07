/*
 * battle-state-menu.h -- battle state (menu)
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
#include <stdbool.h>

#include "battle.h"
#include "battle-bar.h"
#include "battle-state-menu.h"
#include "battle-state-sub.h"
#include "battle-state.h"
#include "character.h"
#include "spell.h"

static void
open_spells(struct battle *bt)
{
	battle_bar_open_spells(&bt->bar, bt, bt->order_cur->ch);
	battle_state_sub(bt);
}

static void
handle(struct battle_state *st, struct battle *bt, const union event *ev)
{
	(void)st;

	if (battle_bar_handle(&bt->bar, bt, ev)) {
		switch (bt->bar.menu) {
		case BATTLE_BAR_MENU_ATTACK:
			/* TODO: do selection for attacking. */
			break;
		case BATTLE_BAR_MENU_MAGIC:
			open_spells(bt);
			break;
		case BATTLE_BAR_MENU_OBJECTS:
			break;
		case BATTLE_BAR_MENU_SPECIAL:
			break;
		default:
			break;
		}
	}
}

void
battle_state_menu(struct battle *bt)
{
	assert(bt);

	static struct battle_state self = {
		.handle = handle,
	};

	battle_switch(bt, &self);
}
