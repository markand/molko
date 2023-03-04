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

union mlk_event;

struct mlk_button;
struct mlk_font;

struct mlk_button_style {
	unsigned long bg_color;
	unsigned long pressed_bg_color;
	unsigned long border_color;
	unsigned long pressed_border_color;
	unsigned long border_size;
	unsigned long text_color;
	unsigned long pressed_text_color;
	struct mlk_font *text_font;
};

struct mlk_button_delegate {
	void *data;
	void (*update)(struct mlk_button_delegate *delegate, struct mlk_button *button, unsigned int ticks);
	void (*draw_frame)(struct mlk_button_delegate *delegate, const struct mlk_button *button);
	void (*draw_text)(struct mlk_button_delegate *delegate, const struct mlk_button *button);
};

struct mlk_button {
	int x, y;
	unsigned int w, h;
	const char *text;
	int pressed;
	struct mlk_button_style *style;
	struct mlk_button_delegate *delegate;
};

extern struct mlk_button_style mlk_button_style;
extern struct mlk_button_delegate mlk_button_delegate;

#if defined(__cplusplus)
extern "C" {
#endif

int
mlk_button_ok(const struct mlk_button *);

int
mlk_button_handle(struct mlk_button *, const union mlk_event *);

void
mlk_button_update(struct mlk_button *, unsigned int);

void
mlk_button_draw(const struct mlk_button *);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_UI_BUTTON_H */
