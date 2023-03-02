/*
 * notify.h -- in game notifications
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

#ifndef MLK_UI_NOTIFY_H
#define MLK_UI_NOTIFY_H

#include <stddef.h>

#include <mlk/core/core.h>

struct mlk_font;
struct mlk_texture;

struct mlk_notify {
	const struct mlk_texture *icon;
	const char *title;
	const char *body;
	unsigned int elapsed;
};

struct mlk_notify_style {
	unsigned long bg_color;
	unsigned long text_color;
	struct mlk_font *text_font;
	unsigned long border_color;
	unsigned int border_size;
	unsigned int delay;
	unsigned int padding;
};

struct mlk_notify_delegate {
	struct mlk_notify *stack;
	size_t stacksz;
	size_t length;
	void (*update)(unsigned int ticks);
	void (*draw)(const struct mlk_notify *notif, size_t index);
};

extern struct mlk_notify_style mlk_notify_style;
extern struct mlk_notify_delegate mlk_notify_delegate;

MLK_CORE_BEGIN_DECLS

void
mlk_notify(const struct mlk_texture *, const char *, const char *);

void
mlk_notify_update(unsigned int ticks);

void
mlk_notify_draw(void);

MLK_CORE_END_DECLS

#endif /*! MLK_UI_NOTIFY_H */
