/*
 * message.h -- message dialog
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

#ifndef MLK_RPG_MESSAGE_H
#define MLK_RPG_MESSAGE_H

#include <stddef.h>

#include <mlk/core/texture.h>

#define MLK_MESSAGE_DELAY_DEFAULT       (150)
#define MLK_MESSAGE_DURATION_DEFAULT    (5000)

struct mlk_font;
struct mlk_message;

union mlk_event;

enum mlk_message_flags {
	MLK_MESSAGE_FLAGS_AUTOMATIC     = (1 << 0),
	MLK_MESSAGE_FLAGS_QUESTION      = (1 << 1),
	MLK_MESSAGE_FLAGS_FADEIN        = (1 << 2),
	MLK_MESSAGE_FLAGS_FADEOUT       = (1 << 3)
};

enum mlk_message_state {
	MLK_MESSAGE_STATE_NONE,
	MLK_MESSAGE_STATE_OPENING,
	MLK_MESSAGE_STATE_SHOWING,
	MLK_MESSAGE_STATE_HIDING
};

struct mlk_message_style {
	unsigned int padding;
	unsigned int delay;
	unsigned int duration;
	unsigned long bg_color;
	unsigned long border_color;
	unsigned long border_size;
	unsigned long text_color;
	unsigned long selected_color;
	struct mlk_font *text_font;
};

struct mlk_message_delegate {
	void *data;

	int (*query)(struct mlk_message_delegate *self,
	             const struct mlk_message *message,
	             unsigned int *w,
	             unsigned int *h);

	void (*update)(struct mlk_message_delegate *self,
	               struct mlk_message *message,
	               unsigned int ticks);

	void (*draw)(struct mlk_message_delegate *self,
	             const struct mlk_message *message);
};

struct mlk_message {
	int x, y;
	unsigned int w, h;
	const char * const *lines;
	size_t linesz;
	unsigned int index;
	enum mlk_message_flags flags;
	enum mlk_message_state state;
	struct mlk_message_style *style;
	struct mlk_message_delegate *delegate;
	unsigned int elapsed;
	double scale;
};

extern struct mlk_message_style mlk_message_style;
extern struct mlk_message_delegate mlk_message_delegate;

#if defined(__cplusplus)
extern "C" {
#endif

void
mlk_message_start(struct mlk_message *msg);

int
mlk_message_query(const struct mlk_message *msg, unsigned int *w, unsigned int *h);

void
mlk_message_handle(struct mlk_message *msg, const union mlk_event *ev);

int
mlk_message_update(struct mlk_message *msg, unsigned int ticks);

void
mlk_message_draw(const struct mlk_message *msg);

void
mlk_message_hide(struct mlk_message *msg);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_RPG_MESSAGE_H */
