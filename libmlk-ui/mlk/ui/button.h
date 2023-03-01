/*
 * button.h -- GUI button
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

#ifndef MLK_UI_BUTTON_H
#define MLK_UI_BUTTON_H

#include <mlk/core/core.h>

union mlk_event;

struct mlk_button;
struct mlk_label_style;
struct mlk_theme;

struct mlk_button_style {
	void *data;
	unsigned long bg_color;
	unsigned long border_color;
	void (*init)(struct mlk_button_style *, struct mlk_button *);
	void (*update)(struct mlk_button_style *, struct mlk_button *, unsigned int);
	void (*draw)(struct mlk_button_style *, const struct mlk_button *);
	void (*finish)(struct mlk_button_style *, struct mlk_button *);
};

struct mlk_button {
	int x, y;
	unsigned int w, h;
	const char *text;
	struct mlk_button_style *style;
	struct mlk_label_style *text_style;
	int pressed;
};

extern struct mlk_button_style mlk_button_style;

MLK_CORE_BEGIN_DECLS

void
mlk_button_init(struct mlk_button *);

int
mlk_button_handle(struct mlk_button *, const union mlk_event *);

void
mlk_button_update(struct mlk_button *, unsigned int);

void
mlk_button_draw(const struct mlk_button *);

void
mlk_button_finish(struct mlk_button *);

MLK_CORE_END_DECLS

#endif /* !MLK_UI_BUTTON_H */
