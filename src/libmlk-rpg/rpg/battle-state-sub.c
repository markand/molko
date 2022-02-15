/*
 * battle-state-sub.c -- battle state (sub)
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
#include <stdlib.h>

#include <core/alloc.h>
#include <core/event.h>
#include <core/sprite.h>
#include <core/trace.h>

#include <ui/theme.h>

#include <rpg/inventory.h>
#include <rpg/item.h>

#include "battle-bar.h"
#include "battle-state-menu.h"
#include "battle-state-selection.h"
#include "battle-state-sub.h"
#include "battle-state.h"
#include "battle.h"
#include "character.h"
#include "spell.h"


static void
handle(struct battle_state *st, struct battle *bt, const union event *ev)
{
	(void)st;

	battle_state_sub_handle(bt, ev);
}

static void
draw(const struct battle_state *st, const struct battle *bt)
{
	(void)st;

	battle_state_sub_draw(bt);
}

static void
finish(struct battle_state *st, struct battle *bt)
{
	(void)bt;

	free(st);
}

void
battle_state_sub_handle(struct battle *bt, const union event *ev)
{
	assert(bt);
	assert(ev);

	switch (ev->type) {
	case EVENT_KEYDOWN:
		switch (ev->key.key) {
		case KEY_ESCAPE:
			/* Escape go to the previous state. */
			battle_state_menu(bt);
			return;
		default:
			break;
		}
	default:
		break;
	}

	if (battle_bar_handle(&bt->bar, bt, ev)) {
		switch (bt->bar.menu) {
		case BATTLE_BAR_MENU_MAGIC:
			start_select_spell(bt);
			break;
		default:
			start_select_object(bt);
			break;
		}
	}
}

void
battle_state_sub_draw(const struct battle *bt)
{
	assert(bt);

	battle_bar_draw(&bt->bar, bt);

	if (bt->bar.menu == BATTLE_BAR_MENU_MAGIC)
		draw_spell_help(bt);
	else if (bt->bar.menu == BATTLE_BAR_MENU_OBJECTS)
		draw_object_help(bt);
}

void
battle_state_sub(struct battle *bt)
{
	assert(bt);

	struct battle_state *state;

	state = alloc_new0(sizeof (*state));
	state->data = bt;
	state->handle = handle;
	state->draw = draw;
	state->finish = finish;

	battle_switch(bt, state);
}
