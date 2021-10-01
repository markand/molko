/*
 * battle-message.c -- automatic top center message in battle
 *
 * Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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

#include <core/window.h>

#include <ui/align.h>
#include <ui/frame.h>
#include <ui/label.h>

#include "battle-message.h"

#define DELAY 1500

int
battle_message_update(struct battle_message *msg, unsigned int ticks)
{
	assert(msg);

	msg->elapsed += ticks;

	return msg->elapsed >= DELAY;
}

void
battle_message_draw(const struct battle_message *msg)
{
	assert(msg);

	struct frame f = {0};
	struct label l = {0};
	unsigned int lw = 0, lh = 0;

	/* Prepare message frame. */
	f.w = window.w / 3;
	f.h = window.h / 15;
	f.theme = msg->theme;

	/* Center on top. */
	align(ALIGN_TOP, &f.x, &f.y, f.w, f.h, 0, 20, window.w, window.h);

	/* Prepare message label box. */
	l.text = msg->text;
	l.flags = LABEL_FLAGS_SHADOW;
	l.theme = msg->theme;
	label_query(&l, &lw, &lh);

	/* Align the text in the box. */
	align(ALIGN_CENTER, &l.x, &l.y, lw, lh, f.x, f.y, f.w, f.h);

	frame_draw(&f);
	label_draw(&l);
}
