/*
 * battle-state-lost.c -- battle state (lost)
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
#include "battle-state-closing.h"
#include "battle-state-victory.h"

struct lost {
	struct battle_state self;
	struct message msg;
};

static void
handle(struct battle_state *st, struct battle *bt, const union event *ev)
{
	(void)bt;

	struct lost *lost = st->data;

	message_handle(&lost->msg, ev);
}

static bool
update(struct battle_state *st, struct battle *bt, unsigned int ticks)
{
	(void)bt;

	struct lost *lost = st->data;

	if (message_update(&lost->msg, ticks))
		battle_state_closing(bt);

	return false;
}

static void
draw(const struct battle_state *st, const struct battle *bt)
{
	(void)bt;

	const struct lost *lost = st->data;

	message_draw(&lost->msg);
}

void
battle_state_lost(struct battle *bt)
{
	assert(bt);

	struct lost *lost;

	if (!(lost = alloc_new0(sizeof (*lost))))
		panic();

	lost->self.data = lost;
	lost->self.handle = handle;
	lost->self.update = update;
	lost->self.draw = draw;

	lost->msg.text[0] = "You have been defeated...";
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
	battle_switch(bt, &lost->self);

	if (bt->music[2])
		music_play(bt->music[2], MUSIC_NONE, 0);
}
