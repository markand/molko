/*
 * message.h -- message dialog
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

#ifndef MOLKO_RPG_MESSAGE_H
#define MOLKO_RPG_MESSAGE_H

#include <core/core.h>
#include <core/texture.h>

struct action;
struct font;
struct theme;

union event;

#define MESSAGE_DELAY_DEFAULT   (150)
#define MESSAGE_TIMEOUT_DEFAULT (5000)
#define MESSAGE_LINES_MAX       (3)

enum message_flags {
	MESSAGE_FLAGS_AUTOMATIC         = (1 << 0),
	MESSAGE_FLAGS_QUESTION          = (1 << 1),
	MESSAGE_FLAGS_FADEIN            = (1 << 2),
	MESSAGE_FLAGS_FADEOUT           = (1 << 3)
};

enum message_state {
	MESSAGE_STATE_NONE,
	MESSAGE_STATE_OPENING,
	MESSAGE_STATE_SHOWING,
	MESSAGE_STATE_HIDING
};

struct message {
	int x;
	int y;
	unsigned int w;
	unsigned int h;
	unsigned int spacing;
	unsigned int delay;
	unsigned int timeout;
	const char *text[MESSAGE_LINES_MAX];
	unsigned int index;
	enum message_flags flags;
	enum message_state state;
	const struct theme *theme;
	unsigned int elapsed;
	double scale;
};

CORE_BEGIN_DECLS

void
message_start(struct message *msg);

void
message_query(const struct message *msg, unsigned int *w, unsigned int *h);

void
message_handle(struct message *msg, const union event *ev);

int
message_update(struct message *msg, unsigned int ticks);

void
message_draw(const struct message *msg);

void
message_hide(struct message *msg);

void
message_action(struct message *msg, struct action *act);

CORE_END_DECLS

#endif /* !MOLKO_RPG_MESSAGE_H */
