/*
 * battle-message.c -- automatic top center message in battle
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

#include <mlk/core/window.h>

#include <mlk/ui/align.h>
#include <mlk/ui/frame.h>
#include <mlk/ui/label.h>

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

	struct mlk_frame f = {0};
	struct mlk_label l = {0};
	unsigned int lw = 0, lh = 0;

	/* Prepare message frame. */
	f.w = mlk_window.w / 3;
	f.h = mlk_window.h / 15;

	/* Center on top. */
	mlk_align(MLK_ALIGN_TOP, &f.x, &f.y, f.w, f.h, 0, 20, mlk_window.w, mlk_window.h);

	/* Prepare message label box. */
	l.text = msg->text;
	mlk_label_query(&l, &lw, &lh);

	/* Align the text in the box. */
	mlk_align(MLK_ALIGN_CENTER, &l.x, &l.y, lw, lh, f.x, f.y, f.w, f.h);

	mlk_frame_draw(&f);
	mlk_label_draw(&l);
}
