/*
 * battle-state-menu.h -- battle state (menu)
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

#include "battle-bar.h"
#include "battle-state-menu.h"
#include "battle-state.h"
#include "battle.h"

static void
handle(struct battle_state *st, struct battle *bt, const union mlk_event *ev)
{
	(void)st;

	battle_state_menu_handle(bt, ev);
}

static int
update(struct battle_state *st, struct battle *bt, unsigned int ticks)
{
	(void)st;

	battle_state_menu_update(bt, ticks);

	return 0;
}

static void
draw(const struct battle_state *st, const struct battle *bt)
{
	(void)st;

	battle_state_menu_draw(bt);
}

static void
finish(struct battle_state *st, struct battle *bt)
{
	(void)bt;

	mlk_alloc_free(st);
}

void
battle_state_menu_handle(struct battle *bt, const union mlk_event *ev)
{
	assert(bt);
	assert(ev);

	battle_bar_handle(bt->bar, bt, ev);
}

void
battle_state_menu_update(struct battle *bt, unsigned int ticks)
{
	assert(battle_ok(bt));

	battle_update_component(bt, ticks, BATTLE_COMPONENT_ALL);
}

void
battle_state_menu_draw(const struct battle *bt)
{
	assert(battle_ok(bt));

	battle_draw_component(bt, BATTLE_COMPONENT_ALL);
}

void
battle_state_menu(struct battle *bt)
{
	assert(bt);

	struct battle_state *state;

	state = mlk_alloc_new0(1, sizeof (*state));
	state->data = bt;
	state->handle = handle;
	state->update = update;
	state->draw = draw;
	state->finish = finish;

	battle_bar_start(bt->bar, bt);
	battle_switch(bt, state);
}
