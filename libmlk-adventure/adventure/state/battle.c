/*
 * battle.c -- manage a battle
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
#include <stdlib.h>

#include <core/alloc.h>
#include <core/game.h>
#include <core/painter.h>
#include <core/state.h>

#include <rpg/battle.h>

#include "battle.h"

struct self {
	struct battle *battle;
	struct state state;
};

static void
start(struct state *state)
{
	struct self *self = state->data;

	battle_start(self->battle);
}

static void
handle(struct state *state, const union event *ev)
{
	struct self *self = state->data;

	battle_handle(self->battle, ev);
}

static void
update(struct state *state, unsigned int ticks)
{
	struct self *self = state->data;

	/* TODO: once we have stacked states, pop it. */
	if (battle_update(self->battle, ticks))
		game_pop();
}

static void
draw(struct state *state)
{
	struct self *self = state->data;

	painter_set_color(0xffffffff);
	painter_clear();
	battle_draw(self->battle);
	painter_present();
}

static void
finish(struct state *state)
{
	struct self *self = state->data;

	battle_finish(self->battle);

	free(self->battle);
	free(self);
}

struct state *
state_battle_new(struct battle *bt)
{
	assert(bt);

	struct self *self;

	self = alloc_new0(sizeof (*self));
	self->battle = bt;
	self->state.data = self;
	self->state.start = start;
	self->state.handle = handle;
	self->state.update = update;
	self->state.draw = draw;
	self->state.finish = finish;

	return &self->state;
}
