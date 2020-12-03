/*
 * battle-state-victory.c -- battle state (victory)
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

#include <core/alloc.h>
#include <core/music.h>
#include <core/panic.h>
#include <core/window.h>

#include <rpg/message.h>

#include "battle.h"
#include "battle-state.h"
#include "rpg_p.h"

struct victory {
	struct battle_state self;
	struct message msg;
};

static void
handle(struct battle_state *st, struct battle *bt, const union event *ev)
{
	(void)bt;

	struct victory *vic = st->data;

	message_handle(&vic->msg, ev);
}

static bool
update(struct battle_state *st, struct battle *bt, unsigned int ticks)
{
	(void)bt;

	struct victory *vic = st->data;

	if (message_update(&vic->msg, ticks))
		battle_state_closing(bt);

	return false;
}

static void
draw(const struct battle_state *st, const struct battle *bt)
{
	(void)bt;

	const struct victory *vic = st->data;

	message_draw(&vic->msg);
}

void
battle_state_victory(struct battle *bt)
{
	assert(bt);

	struct victory *vic;

	if (!(vic = alloc_new0(sizeof (*vic))))
		panic();

	/* TODO: compute money, xp and drop. */
	vic->self.data = vic;
	vic->self.handle = handle;
	vic->self.update = update;
	vic->self.draw = draw;

	vic->msg.text[0] = _("Victory!");
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
	battle_switch(bt, &vic->self);

	if (bt->music[1])
		music_play(bt->music[1], MUSIC_NONE, 0);
}
