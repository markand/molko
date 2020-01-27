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

#include "event.h"
#include "font.h"
#include "message.h"
#include "painter.h"
#include "sprite.h"
#include "texture.h"
#include "window.h"

#define MESSAGE_SPEED   200     /* Time delay for animations */
#define MESSAGE_TIMEOUT 5000    /* Time for auto-closing */

static unsigned int
average(const struct message *msg)
{
	unsigned int n = 0;
	unsigned int total = 0;

	for (int i = 0; i < 6; ++i) {
		if (msg->textures[i]) {
			n += 1;
			total += texture_height(msg->textures[i]);
		}
	}

	return n > 0 ? total / n : 0;
}

static void
clear(struct message *msg)
{
	for (unsigned int i = 0; i < 12; ++i) {
		if (msg->textures[i]) {
			texture_close(msg->textures[i]);
			msg->textures[i] = NULL;
		}
	}
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
		unsigned long color = msg->colors[0];

		if (msg->flags & MESSAGE_QUESTION && msg->index == i)
			color = msg->colors[1];

		if (!msg->textures[i])
			msg->textures[i] = font_render(
			    msg->font,
			    msg->text[i],
			    color
			);
		if (!msg->textures[i + 6])
			msg->textures[i + 6] = font_render(
			    msg->font,
			    msg->text[i],
			    0x000000ff
			);
	}
}

void
message_start(struct message *msg)
{
	assert(msg);

	msg->elapsed = 0;
	msg->state = MESSAGE_OPENING;
	msg->height[0] = texture_height(msg->frame);
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
		msg->state = MESSAGE_HIDING;
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
		msg->height[1] += texture_height(msg->frame) * ticks / MESSAGE_SPEED;

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
			msg->state = MESSAGE_HIDING;
			msg->elapsed = 0;
		}

		break;
	case MESSAGE_HIDING:
		msg->height[1] -= texture_height(msg->frame) * ticks / MESSAGE_SPEED;

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

	const unsigned int w = texture_width(msg->frame);
	const unsigned int h = texture_height(msg->frame);
	const unsigned int x = (window_width() / 2) - (w / 2);
	const unsigned int y = 80;
	const unsigned int avgh = average(msg);
	const unsigned int gapy = (h - (avgh * 6)) / 7;

	switch (msg->state) {
	case MESSAGE_OPENING:
	case MESSAGE_HIDING:
		texture_draw_ex(msg->frame, 0, 0, w, msg->height[1], x, y, w, msg->height[1], 0);
		break;
	case MESSAGE_SHOWING:
		texture_draw(msg->frame, x, y);

		for (int i = 0; i < 6; ++i) {
			/* TODO: avatar handling */
			const int real_x = x + 20;
			const int real_y = y + ((i + 1) * gapy) + (i * avgh);

			if (!msg->textures[i])
				continue;

			texture_draw(msg->textures[i + 6], real_x + 2, real_y + 2);
			texture_draw(msg->textures[i], real_x, real_y);
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
message_close(struct message *msg)
{
	assert(msg);

	clear(msg);
}
