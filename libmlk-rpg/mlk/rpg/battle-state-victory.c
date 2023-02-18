/*
 * battle-state-victory.c -- battle state (victory)
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

#include <mlk/core/alloc.h>
#include <mlk/core/music.h>
#include <mlk/core/panic.h>
#include <mlk/core/window.h>

#include "battle-state-closing.h"
#include "battle-state-victory.h"
#include "battle-state.h"
#include "battle.h"

struct self {
	struct battle_state_victory data;
	struct battle_state state;
};

static void
handle(struct battle_state *st, struct battle *bt, const union mlk_event *ev)
{
	(void)bt;

	battle_state_victory_handle(st->data, ev);
}

static int
update(struct battle_state *st, struct battle *bt, unsigned int ticks)
{
	(void)bt;

	return battle_state_victory_update(st->data, bt, ticks);
}

static void
draw(const struct battle_state *st, const struct battle *bt)
{
	(void)bt;

	battle_state_victory_draw(st->data, bt);
}

void
battle_state_victory_init(struct battle_state_victory *vic, struct battle *bt)
{
	assert(bt);

	vic->text = "Victory!";

	vic->msg.lines = &vic->text;
	vic->msg.linesz = 1;
	vic->msg.theme = bt->theme;
	vic->msg.flags = MESSAGE_FLAGS_AUTOMATIC |
	                 MESSAGE_FLAGS_FADEIN |
	                 MESSAGE_FLAGS_FADEOUT;
	vic->msg.timeout = MESSAGE_TIMEOUT_DEFAULT;
	vic->msg.delay = MESSAGE_DELAY_DEFAULT;

	message_start(&vic->msg);
	message_query(&vic->msg, NULL, &vic->msg.h);

	vic->msg.w = window.w * 0.6;
	vic->msg.y = window.h * 0.1;
	vic->msg.x = (window.w - vic->msg.w) / 2;

	bt->status = BATTLE_STATUS_WON;

	if (bt->music[1])
		music_play(bt->music[1], MUSIC_NONE);
}

void
battle_state_victory_handle(struct battle_state_victory *vic, const union mlk_event *ev)
{
	assert(vic);

	message_handle(&vic->msg, ev);
}

int
battle_state_victory_update(struct battle_state_victory *vic, struct battle *bt, unsigned int ticks)
{
	assert(vic);
	assert(bt);

	battle_update_component(bt, BATTLE_COMPONENT_ALL, ticks);

	if (message_update(&vic->msg, ticks))
		battle_state_closing(bt);

	return 0;
}

void
battle_state_victory_draw(const struct battle_state_victory *vic, const struct battle *bt)
{
	assert(vic);

	battle_draw_component(bt, BATTLE_COMPONENT_ALL);
	message_draw(&vic->msg);
}

void
battle_state_victory(struct battle *bt)
{
	assert(bt);

	struct self *self;

	/* TODO: compute money, xp and drop. */
	self = mlk_alloc_new0(1, sizeof (*self));
	self->state.data = self;
	self->state.handle = handle;
	self->state.update = update;
	self->state.draw = draw;

	battle_state_victory_init(&self->data, bt);
	battle_switch(bt, &self->state);
}
