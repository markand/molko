/*
 * message.c -- message dialog
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
#include <string.h>

#include "action.h"
#include "event.h"
#include "font.h"
#include "frame.h"
#include "label.h"
#include "maths.h"
#include "message.h"
#include "painter.h"
#include "panic.h"
#include "sprite.h"
#include "texture.h"
#include "theme.h"
#include "util.h"

#define THEME(msg)      (msg->theme ? msg->theme : theme_default())

static void
handle(struct action *action, const union event *ev)
{
	assert(action);
	assert(ev);

	message_handle(action->data, ev);
}

static bool
update(struct action *action, unsigned int ticks)
{
	assert(action);

	return message_update(action->data, ticks);
}

static void
draw(struct action *action)
{
	assert(action);

	message_draw(action->data);
}

static void
draw_frame(const struct message *msg)
{
	assert(msg);

	struct frame frame = {
		.w = msg->w,
		.h = msg->h,
		.theme = msg->theme
	};

	frame_draw(&frame);
}

static void
draw_lines(const struct message *msg)
{
	struct theme *theme;
	struct font *font;
	unsigned int lineh;

	theme = THEME(msg);
	font = theme->fonts[THEME_FONT_INTERFACE];
	lineh = font_height(font);

	for (int i = 0; i < 6; ++i) {
		if (!msg->text[i])
			continue;

		struct label label = {
			.x = theme->padding,
			.y = theme->padding + (i * lineh),
			.h = lineh,
			.theme = msg->theme,
			.text = msg->text[i],
			.flags = LABEL_NO_HCENTER
		};

		/*
		 * The function label_draw will normally use
		 * THEME_FONT_INTERFACE so update its color if needed.
		 */
		if (msg->flags & MESSAGE_FLAGS_QUESTION && msg->index == (unsigned int)i)
			label.color = theme->colors[THEME_COLOR_SELECTED];

		label_draw(&label);
	}
}

void
message_start(struct message *msg)
{
	assert(msg);

	msg->elapsed = 0;
	msg->scale = msg->flags & MESSAGE_FLAGS_FADEIN ? 0.0 : 1.0;
	msg->state = msg->flags & MESSAGE_FLAGS_FADEIN
	    ? MESSAGE_STATE_OPENING
	    : MESSAGE_STATE_SHOWING;
}

void
message_handle(struct message *msg, const union event *ev)
{
	assert(msg);
	assert(ev);

	/* Skip if the message animation hasn't complete. */
	if (msg->state != MESSAGE_STATE_SHOWING)
		return;

	/* Only keyboard event are valid. */
	if (ev->type != EVENT_KEYDOWN || msg->state == MESSAGE_STATE_NONE)
		return;

	switch (ev->key.key) {
	case KEY_UP:
		if (msg->index > 0)
			msg->index--;
		break;
	case KEY_DOWN:
		if (msg->index < 5 && msg->text[msg->index + 1])
			msg->index++;
		break;
	case KEY_ENTER:
		msg->state = msg->flags & MESSAGE_FLAGS_FADEOUT
		    ? MESSAGE_STATE_HIDING
		    : MESSAGE_STATE_NONE;
		msg->elapsed = 0;
		break;
	default:
		break;
	}
}

bool
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
message_draw(struct message *msg)
{
	assert(msg);

	struct texture tex;
	int x, y;
	unsigned int w, h;

	if (!texture_new(&tex, msg->w, msg->h))
		panic();

	PAINTER_BEGIN(&tex);
	draw_frame(msg);
	draw_lines(msg);
	PAINTER_END();

	/* Compute scaling. */
	w = msg->w * msg->scale;
	h = msg->h * msg->scale;

	/* Centerize within its drawing area. */
	maths_centerize(&x, &y, w, h, msg->x, msg->y, msg->w, msg->h);

	/* Draw and clear. */
	texture_scale(&tex, 0, 0, msg->w, msg->h, x, y, w, h, 0);
	texture_finish(&tex);
}

void
message_hide(struct message *msg)
{
	assert(msg);

	msg->state = MESSAGE_STATE_HIDING;
	msg->elapsed = 0;
}

void
message_action(struct message *msg, struct action *action)
{
	assert(msg);
	assert(action);

	memset(action, 0, sizeof (struct action));
	action->data = msg;
	action->handle = handle;
	action->update = update;
	action->draw = draw;

	message_start(msg);
}
