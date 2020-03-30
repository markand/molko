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
#include "message.h"
#include "painter.h"
#include "sprite.h"
#include "texture.h"
#include "util.h"
#include "window.h"

#define MESSAGE_SPEED   200     /* Time delay for animations */
#define MESSAGE_TIMEOUT 5000    /* Time for auto-closing */

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

	message_finish(action->data);
	free(action->data);
}

static unsigned int
average(const struct message *msg)
{
	unsigned int n = 0;
	unsigned int total = 0;

	for (int i = 0; i < 6; ++i) {
		if (msg->textures[i].w) {
			n += 1;
			total += msg->textures[i].h;
		}
	}

	return n > 0 ? total / n : 0;
}

static void
clear(struct message *msg)
{
	for (unsigned int i = 0; i < 12; ++i)
		texture_finish(&msg->textures[i]);
}

static void
redraw(struct message *msg)
{
	clear(msg);

	/* Generate textures if not already done. */
	for (unsigned int i = 0; i < 6; ++i) {
		if (!msg->text[i])
			continue;

		/* Normal lines of text. */
		msg->font->color = msg->colors[0];

		if (msg->flags & MESSAGE_QUESTION && msg->index == i)
			msg->font->color = msg->colors[1];

		if (!texture_ok(&msg->textures[i]))
			font_render(
			    msg->font,
			    &msg->textures[i],
			    msg->text[i]
			);
		if (!texture_ok(&msg->textures[i + 6])) {
			msg->font->color = 0x000000ff;

			font_render(
			    msg->font,
			    &msg->textures[i + 6],
			    msg->text[i]
			);
		}
	}
}

void
message_start(struct message *msg)
{
	assert(msg);

	msg->elapsed = 0;
	msg->state = msg->flags & MESSAGE_QUICK ? MESSAGE_SHOWING : MESSAGE_OPENING;
	msg->height[0] = msg->frame->h;
	msg->height[1] = 0;

	redraw(msg);
}

void
message_handle(struct message *msg, const union event *ev)
{
	assert(msg);
	assert(ev);

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

	redraw(msg);
}

bool
message_update(struct message *msg, unsigned int ticks)
{
	assert(msg);

	msg->elapsed += ticks;

	switch (msg->state) {
	case MESSAGE_OPENING:
		msg->height[1] += msg->frame->h * ticks / MESSAGE_SPEED;

		if (msg->height[1] > msg->height[0])
			msg->height[1] = msg->height[0];
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
		msg->height[1] -= msg->frame->h * ticks / MESSAGE_SPEED;

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

void
message_draw(struct message *msg)
{
	assert(msg);
	assert(msg->frame);

	const unsigned int w = msg->frame->w;
	const unsigned int h = msg->frame->h;
	const unsigned int x = (window_width() / 2) - (w / 2);
	const unsigned int y = 80;
	const unsigned int avgh = average(msg);
	const unsigned int gapy = (h - (avgh * 6)) / 7;

	switch (msg->state) {
	case MESSAGE_OPENING:
	case MESSAGE_HIDING:
		texture_scale(msg->frame, 0, 0, w, msg->height[1], x, y, w, msg->height[1], 0);
		break;
	case MESSAGE_SHOWING:
		texture_draw(msg->frame, x, y);

		for (int i = 0; i < 6; ++i) {
			/* TODO: avatar handling */
			const int real_x = x + 20;
			const int real_y = y + ((i + 1) * gapy) + (i * avgh);

			if (!texture_ok(&msg->textures[i]))
				continue;

			texture_draw(&msg->textures[i + 6], real_x + 2, real_y + 2);
			texture_draw(&msg->textures[i], real_x, real_y);
		}
		break;
	default:
		break;
	}
}

void
message_hide(struct message *msg)
{
	assert(msg);

	msg->state = MESSAGE_HIDING;
	msg->elapsed = 0;
}

void
message_finish(struct message *msg)
{
	assert(msg);

	clear(msg);
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
