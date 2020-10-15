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

#include <core/action.h>
#include <core/event.h>
#include <core/font.h>
#include <core/maths.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/sprite.h>
#include <core/trace.h>
#include <core/util.h>

#include <ui/align.h>
#include <ui/frame.h>
#include <ui/label.h>
#include <ui/theme.h>

#include "message.h"

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
	struct theme theme;
	unsigned int lineh;

	/* Shallow copy theme to modify colors. */
	theme_shallow(&theme, msg->theme);

	/* Compute text size for list alignment. */
	lineh = font_height(theme.fonts[THEME_FONT_INTERFACE]);

	for (int i = 0; i < 6; ++i) {
		if (!msg->text[i])
			continue;

		struct label label = {
			.y = i * lineh,
			.w = msg->w,
			.h = msg->h,
			.theme = &theme,
			.text = msg->text[i],
			.align = ALIGN_TOP_LEFT,
			.flags = LABEL_FLAGS_SHADOW
		};

		/*
		 * The function label_draw will use THEME_COLOR_NORMAL to draw
		 * text and THEME_COLOR_SHADOW so if we have selected a line
		 * we need to cheat the normal color.
		 */
		if (msg->flags & MESSAGE_FLAGS_QUESTION && msg->index == (unsigned int)i)
			theme.colors[THEME_COLOR_NORMAL] = THEME(msg)->colors[THEME_COLOR_SELECTED];
		else
			theme.colors[THEME_COLOR_NORMAL] = THEME(msg)->colors[THEME_COLOR_NORMAL];

		label_draw(&label);
	}
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
		trace("message is automatic but has zero timeout");
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
	align(ALIGN_CENTER, &x, &y, w, h, msg->x, msg->y, msg->w, msg->h);

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
