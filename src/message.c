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

#define MESSAGE_SPEED   1000    /* Time delay for animations */
#define MESSAGE_TIMEOUT 5000    /* Time for auto-closing */

void
message_start(struct message *msg)
{
	assert(msg);

	msg->elapsed = 0;
	msg->state = MESSAGE_OPENING;
}

void
message_update(struct message *msg, unsigned int ticks)
{
	assert(msg);

	msg->elapsed += ticks;

	switch (msg->state) {
	case MESSAGE_OPENING:
		if (msg->elapsed >= MESSAGE_SPEED)
			msg->state = MESSAGE_SHOWING;
		break;
	case MESSAGE_SHOWING:
		/* Do automatically switch state if requested by the user. */
		if (msg->flags & MESSAGE_AUTOMATIC && msg->elapsed >= MESSAGE_TIMEOUT)
			msg->state = MESSAGE_HIDING;

		break;
	default:
		break;
	}
}

void
message_draw(struct message *msg)
{
	assert(msg);

	/* TODO: more constant variables. */
	struct texture *lines[3];
	int x = 160;
	int y = 80;

	painter_set_color(0xff0000ff);
	painter_draw_rectangle(true, x, y, 960, 160);

	for (int i = 0; msg->text[i]; ++i) {
		lines[i] = font_render(msg->font, msg->text[i], 0xffffffff);

		if (!lines[i])
			continue;

		texture_draw(lines[i], x, y);
		texture_close(lines[i]);
		y += 53;
	}
}

void
message_hide(struct message *msg)
{
	assert(msg);

	msg->elapsed = 0;
}

bool
message_is_complete(struct message *msg)
{
	assert(msg);

	return msg->state == MESSAGE_HIDING && msg->elapsed >= MESSAGE_SPEED;
}
