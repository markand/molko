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
#include "message.h"
#include "painter.h"
#include "panic.h"
#include "sprite.h"
#include "texture.h"
#include "theme.h"
#include "util.h"
#include "window.h"

#define MESSAGE_SPEED   100     /* Time delay for animations */
#define MESSAGE_TIMEOUT 5000    /* Time for auto-closing */

#define WIDTH   (window.w * 0.75)
#define HEIGHT  (window.h * 0.125)

#define THEME(msg) (msg->theme ? msg->theme : theme_default())

static void
action_handle(struct action *action, const union event *ev)
{
	assert(action);
	assert(ev);

	message_handle(action->data, ev);
}

static bool
action_update(struct action *action, unsigned int ticks)
{
	assert(action);

	return message_update(action->data, ticks);
}

static void
action_draw(struct action *action)
{
	assert(action);

	message_draw(action->data);
}

static void
action_finish(struct action *action)
{
	assert(action);

	free(action->data);
}

void
message_start(struct message *msg)
{
	assert(msg);

	msg->elapsed = 0;
	msg->scale = 0.0;
	msg->state = msg->flags & MESSAGE_QUICK ? MESSAGE_SHOWING : MESSAGE_OPENING;
}

void
message_handle(struct message *msg, const union event *ev)
{
	assert(msg);
	assert(ev);

	/* Skip if the message animation hasn't complete. */
	if (msg->state != MESSAGE_SHOWING)
		return;

	/* Only keyboard event are valid. */
	if (ev->type != EVENT_KEYDOWN || msg->state == MESSAGE_NONE)
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
		msg->state = msg->flags & MESSAGE_QUICK ? MESSAGE_NONE : MESSAGE_HIDING;
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
	case MESSAGE_OPENING:
		msg->scale = (double)msg->elapsed / (double)MESSAGE_SPEED;

		if (msg->scale > 1)
			msg->scale = 1;

		if (msg->elapsed >= MESSAGE_SPEED) {
			msg->state = MESSAGE_SHOWING;
			msg->elapsed = 0;
		}

		break;
	case MESSAGE_SHOWING:
		/* Do automatically switch state if requested by the user. */
		if (msg->flags & MESSAGE_AUTOMATIC && msg->elapsed >= MESSAGE_TIMEOUT) {
			msg->state = msg->flags & MESSAGE_QUICK ? MESSAGE_NONE : MESSAGE_HIDING;
			msg->elapsed = 0;
		}

		break;
	case MESSAGE_HIDING:
		msg->scale = 1 - (double)msg->elapsed / (double)MESSAGE_SPEED;

		if (msg->scale < 0)
			msg->scale = 0;
		if (msg->elapsed >= MESSAGE_SPEED) {
			msg->state = MESSAGE_NONE;
			msg->elapsed = 0;
		}

		break;
	default:
		break;
	}

	return msg->state == MESSAGE_NONE;
}

static void
draw_frame(const struct message *msg)
{
	assert(msg);

	struct frame frame = {
		.w = WIDTH,
		.h = HEIGHT
	};

	frame_draw(&frame);
}

static void
draw_lines(const struct message *msg)
{
	struct font *font;
	unsigned int lineh;

	font = THEME(msg)->fonts[THEME_FONT_INTERFACE];
	lineh = font_height(font);

	for (int i = 0; i < 6; ++i) {
		if (!msg->text[i])
			continue;

		struct label label = {
			.x = 10,
			.y = 10 + (i * lineh),
			.h = lineh,
			.theme = msg->theme,
			.text = msg->text[i],
			.flags = LABEL_NO_HCENTER
		};

		/*
		 * The function label_draw will normally use
		 * THEME_FONT_INTERFACE so update its color if needed.
		 */
		if (msg->flags & MESSAGE_QUESTION && msg->index == i)
			label.color = THEME(msg)->colors[THEME_COLOR_SELECTED];

		label_draw(&label);
	}
}

void
message_draw(struct message *msg)
{
	assert(msg);

	struct texture tex;
	int x, y, w, h;

	if (!texture_new(&tex, WIDTH, HEIGHT))
		panic();

	PAINTER_BEGIN(&tex);
	draw_frame(msg);
	draw_lines(msg);
	PAINTER_END();

	/* Compute scaling. */
	w = WIDTH * msg->scale;
	h = HEIGHT * msg->scale;

	/* Compute position. */
	x = (window.w / 2) - (w / 2);
	y = HEIGHT;

	texture_scale(&tex, 0, 0, WIDTH, HEIGHT, x, y, w, h, 0.0);
	texture_finish(&tex);
}

void
message_hide(struct message *msg)
{
	assert(msg);

	msg->state = MESSAGE_HIDING;
	msg->elapsed = 0;
}

void
message_action(const struct message *msg, struct action *action)
{
	assert(msg);
	assert(action);

	memset(action, 0, sizeof (struct action));
	action->data = ememdup(msg, sizeof (struct message));
	action->handle = action_handle;
	action->update = action_update;
	action->draw = action_draw;
	action->finish = action_finish;
}
