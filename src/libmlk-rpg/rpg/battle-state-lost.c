/*
 * battle-state-lost.c -- battle state (lost)
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

#include <core/alloc.h>
#include <core/music.h>
#include <core/panic.h>
#include <core/window.h>

#include "battle-state-closing.h"
#include "battle-state-lost.h"
#include "battle-state.h"
#include "battle.h"
#include "rpg_p.h"

struct self {
	struct battle_state_lost data;
	struct battle_state state;
};

static void
handle(struct battle_state *st, struct battle *bt, const union event *ev)
{
	(void)bt;

	battle_state_lost_handle(st->data, ev);
}

static int
update(struct battle_state *st, struct battle *bt, unsigned int ticks)
{
	return battle_state_lost_update(st->data, bt, ticks);
}

static void
draw(const struct battle_state *st, const struct battle *bt)
{
	(void)bt;

	battle_state_lost_draw(st->data, bt);
}

void
battle_state_lost_init(struct battle_state_lost *lost, struct battle *bt)
{
	assert(lost);
	assert(bt);

	lost->text = _("You have been defeated...");

	lost->msg.lines = &lost->text;
	lost->msg.linesz = 1;
	lost->msg.theme = bt->theme;
	lost->msg.flags = MESSAGE_FLAGS_AUTOMATIC |
	                  MESSAGE_FLAGS_FADEIN |
	                  MESSAGE_FLAGS_FADEOUT;
	lost->msg.timeout = MESSAGE_TIMEOUT_DEFAULT;
	lost->msg.delay = MESSAGE_DELAY_DEFAULT;

	message_start(&lost->msg);
	message_query(&lost->msg, NULL, &lost->msg.h);

	lost->msg.w = window.w * 0.6;
	lost->msg.y = window.h * 0.1;
	lost->msg.x = (window.w - lost->msg.w) / 2;

	bt->status = BATTLE_STATUS_LOST;

	if (bt->music[2])
		music_play(bt->music[2], MUSIC_NONE);
}

void
battle_state_lost_handle(struct battle_state_lost *lost, const union event *ev)
{
	assert(lost);
	assert(ev);

	message_handle(&lost->msg, ev);
}

int
battle_state_lost_update(struct battle_state_lost *lost, struct battle *bt, unsigned int ticks)
{
	assert(lost);
	assert(bt);

	if (message_update(&lost->msg, ticks))
		battle_state_closing(bt);

	return 0;
}

void
battle_state_lost_draw(struct battle_state_lost *lost, const struct battle *bt)
{
	assert(lost);
	assert(battle_ok(bt));

	battle_draw_component(bt, BATTLE_COMPONENT_ALL);
	message_draw(&lost->msg);
}

void
battle_state_lost(struct battle *bt)
{
	assert(bt);

	struct self *self;

	self = alloc_new0(sizeof (*self));
	self->state.data = self;
	self->state.handle = handle;
	self->state.update = update;
	self->state.draw = draw;

	battle_state_lost_init(&self->data, bt);
	battle_switch(bt, &self->state);
}
