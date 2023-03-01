/*
 * message.c -- message dialog
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
#include <string.h>

#include <mlk/core/event.h>
#include <mlk/core/font.h>
#include <mlk/core/maths.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/sprite.h>
#include <mlk/core/trace.h>
#include <mlk/core/util.h>

#include <mlk/ui/align.h>
#include <mlk/ui/frame.h>
#include <mlk/ui/label.h>

#include "message.h"

static void
draw_frame(const struct message *msg)
{
	assert(msg);

	struct mlk_frame frame = {
		.w = msg->w,
		.h = msg->h
	};

	mlk_frame_draw(&frame);
}

static inline unsigned int
min_width(const struct message *msg)
{
#if 0
	assert(msg);

	unsigned int maxw = 0, w = 0;
	int err;

	for (size_t i = 0; i < msg->linesz; ++i) {
		if (!msg->lines[i])
			continue;
		if ((err = mlk_font_query(THEME(msg)->fonts[MLK_THEME_FONT_INTERFACE], msg->lines[i], &w, NULL)) < 0)
			mlk_panic(err);
		if (w > maxw)
			maxw = w;
	}

	return (THEME(msg)->padding * 2) + maxw;
#endif
	return 0;
}

static inline unsigned int
min_height(const struct message *msg)
{
	assert(msg);

#if 0
	const struct mlk_theme *th = THEME(msg);
	const unsigned int lh  = mlk_font_height(th->fonts[MLK_THEME_FONT_INTERFACE]);

	return (th->padding * 2) + (msg->linesz * lh) + ((msg->linesz - 1) * msg->spacing);
#endif
	return 0;
}

static void
draw_lines(const struct message *msg)
{
#if 0
	const struct mlk_theme *theme = THEME(msg);
	struct mlk_label label;
	unsigned int lw, lh;
	int err;

	for (size_t i = 0; i < msg->linesz; ++i) {
		if (!msg->lines[i])
			continue;
		if ((err = mlk_font_query(theme->fonts[MLK_THEME_FONT_INTERFACE], msg->lines[i], &lw, &lh)) < 0)
			mlk_panic(err);

		label.x = theme->padding;
		label.y = theme->padding + (i * (lh + msg->spacing));
		label.text = msg->lines[i];
		label.flags = MLK_LABEL_FLAGS_SHADOW;

		if (label.x + lw > msg->w)
			mlk_tracef("message width too small: %u < %u", msg->w, min_width(msg));
		if (label.y + lh > msg->h)
			mlk_tracef("message height too small: %u < %u", msg->h, min_height(msg));

		/*
		 * The function label_draw will use THEME_COLOR_NORMAL to draw
		 * text and THEME_COLOR_SHADOW so if we have selected a line
		 * we need to cheat the normal color.
		 */
#if 0
		if ((msg->flags & MESSAGE_FLAGS_QUESTION) && msg->index == (unsigned int)i)
			label.flags |= MLK_LABEL_FLAGS_SELECTED;
		else
			label.flags &= ~(MLK_LABEL_FLAGS_SELECTED);
#endif

		mlk_label_draw(&label);
	}
#endif
}

void
message_start(struct message *msg)
{
	assert(msg);

	if (msg->flags & (MESSAGE_FLAGS_FADEIN|MESSAGE_FLAGS_FADEOUT))
		assert(msg->delay > 0);

	msg->elapsed = 0;
	msg->scale = msg->flags & MESSAGE_FLAGS_FADEIN ? 0.0 : 1.0;
	msg->state = msg->flags & MESSAGE_FLAGS_FADEIN
	    ? MESSAGE_STATE_OPENING
	    : MESSAGE_STATE_SHOWING;

	if (msg->flags & MESSAGE_FLAGS_AUTOMATIC && msg->timeout == 0)
		mlk_tracef("message is automatic but has zero timeout");
}

void
message_query(const struct message *msg, unsigned int *w, unsigned int *h)
{
	assert(msg);

	if (w)
		*w = min_width(msg);
	if (h)
		*h = min_height(msg);
}

void
message_handle(struct message *msg, const union mlk_event *ev)
{
	assert(msg);
	assert(ev);

	/* Skip if the message animation hasn't complete. */
	if (msg->state != MESSAGE_STATE_SHOWING)
		return;

	/* Only keyboard event are valid. */
	if (ev->type != MLK_EVENT_KEYDOWN || msg->state == MESSAGE_STATE_NONE)
		return;

	switch (ev->key.key) {
	case MLK_KEY_UP:
		if (msg->index > 0)
			msg->index--;
		break;
	case MLK_KEY_DOWN:
		if (msg->index + 1 < msg->linesz && msg->lines[msg->index + 1])
			msg->index++;
		break;
	case MLK_KEY_ENTER:
		msg->state = msg->flags & MESSAGE_FLAGS_FADEOUT
		    ? MESSAGE_STATE_HIDING
		    : MESSAGE_STATE_NONE;
		msg->elapsed = 0;
		break;
	default:
		break;
	}
}

int
message_update(struct message *msg, unsigned int ticks)
{
	assert(msg);

	msg->elapsed += ticks;

	switch (msg->state) {
	case MESSAGE_STATE_OPENING:
		msg->scale = (double)msg->elapsed / (double)msg->delay;

		if (msg->scale > 1)
			msg->scale = 1;

		if (msg->elapsed >= msg->delay) {
			msg->state = MESSAGE_STATE_SHOWING;
			msg->elapsed = 0;
		}

		break;
	case MESSAGE_STATE_SHOWING:
		/* Do automatically switch state if requested by the user. */
		if (msg->flags & MESSAGE_FLAGS_AUTOMATIC && msg->elapsed >= msg->timeout) {
			msg->state = msg->flags & MESSAGE_FLAGS_FADEOUT
			    ? MESSAGE_STATE_HIDING
			    : MESSAGE_STATE_NONE;
			msg->elapsed = 0;
		}

		break;
	case MESSAGE_STATE_HIDING:
		msg->scale = 1 - (double)msg->elapsed / (double)msg->delay;

		if (msg->scale < 0)
			msg->scale = 0;
		if (msg->elapsed >= msg->delay) {
			msg->state = MESSAGE_STATE_NONE;
			msg->elapsed = 0;
		}

		break;
	default:
		break;
	}

	return msg->state == MESSAGE_STATE_NONE;
}

void
message_draw(const struct message *msg)
{
	assert(msg);

	struct mlk_texture tex;
	int x, y, err;
	unsigned int w, h;

	if (msg->w == 0 || msg->h == 0) {
		mlk_tracef("message has null dimensions");
		return;
	}

	if ((err = mlk_texture_new(&tex, msg->w, msg->h)) < 0)
		mlk_panic(err);

	MLK_PAINTER_BEGIN(&tex);
	draw_frame(msg);
	draw_lines(msg);
	MLK_PAINTER_END();

	/* Compute scaling. */
	w = msg->w * msg->scale;
	h = msg->h * msg->scale;

	/* Centerize within its drawing area. */
	mlk_align(MLK_ALIGN_CENTER, &x, &y, w, h, msg->x, msg->y, msg->w, msg->h);

	/* Draw and clear. */
	mlk_texture_scale(&tex, 0, 0, msg->w, msg->h, x, y, w, h, 0);
	mlk_texture_finish(&tex);
}

void
message_hide(struct message *msg)
{
	assert(msg);

	msg->state = MESSAGE_STATE_HIDING;
	msg->elapsed = 0;
}
