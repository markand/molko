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

#include "font.h"
#include "message.h"
#include "painter.h"
#include "sprite.h"
#include "texture.h"
#include "window.h"

#define MESSAGE_SPEED   1000    /* Time delay for animations */
#define MESSAGE_TIMEOUT 5000    /* Time for auto-closing */

static unsigned int
average_height(const struct message *msg)
{
	unsigned int n = 0;
	unsigned int total = 0;

	for (int i = 0; i < 6; ++i) {
		if (msg->ttext[i]) {
			n += 1;
			total += texture_height(msg->ttext[i]);
		}
	}

	return n > 0 ? total / n : 0;
}

static void
redraw(struct message *msg)
{
	/* Generate textures if not already done. */
	for (int i = 0; i < 6; ++i) {
		if (!msg->text[i] || msg->ttext[i] || msg->stext[i])
			continue;

		msg->stext[i] = font_render(msg->font, msg->text[i], 0x000000ff);
		msg->ttext[i] = font_render(msg->font, msg->text[i], msg->color);
	}
}

void
message_start(struct message *msg)
{
	assert(msg);

	msg->elapsed = 0;
	msg->alpha = 0;
	msg->state = MESSAGE_OPENING;
}

bool
message_update(struct message *msg, unsigned int ticks)
{
	assert(msg);

	msg->elapsed += ticks;

	switch (msg->state) {
	case MESSAGE_OPENING:
		msg->alpha += 255 * ticks / MESSAGE_SPEED;

		if (msg->alpha > 255)
			msg->alpha = 255;
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
	const unsigned int avgh = average_height(msg);
	const unsigned int gapy = (h - (avgh * 6)) / 7;

	/* TODO: handle state */
	redraw(msg);
	texture_draw(msg->frame, x, y);

	for (int i = 0; i < 6; ++i) {
		/* TODO: avatar handling */
		const int real_x = x + 20;
		const int real_y = y + ((i + 1) * gapy) + (i * avgh);

		if (!msg->ttext[i])
			continue;

		texture_draw(msg->stext[i], real_x + 2, real_y + 2);
		texture_draw(msg->ttext[i], real_x, real_y);
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

	for (int i = 0; i < 6; ++i) {
		if (msg->ttext[i]) {
			texture_close(msg->ttext[i]);
			msg->ttext[i] = NULL;
		}
		if (msg->stext[i]) {
			texture_close(msg->stext[i]);
			msg->stext[i] = NULL;
		}
	}
}
