/*
 * button.c -- GUI button
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

#include <assert.h>
#include <string.h>

#include <mlk/core/event.h>
#include <mlk/core/maths.h>
#include <mlk/core/painter.h>
#include <mlk/core/trace.h>

#include "align.h"
#include "button.h"
#include "ui.h"
#include "ui_p.h"

static inline int
is_boxed(const struct mlk_button *button, const struct mlk_event_click *click)
{
	assert(button);
	assert(click);
	assert(click->type == MLK_EVENT_CLICKDOWN || click->type == MLK_EVENT_CLICKUP);

	return mlk_maths_is_boxed(button->x, button->y, button->w, button->h,
	    click->x, click->y);
}

static inline struct mlk_font *
style_font(const struct mlk_button *button)
{
	const struct mlk_button_style *style = MLK__STYLE(button, mlk_button_style);

	if (style->text_font)
		return style->text_font;

	return mlk_ui_fonts[MLK_UI_FONT_INTERFACE];
}

static void
delegate_draw_frame(struct mlk_button_delegate *delegate, const struct mlk_button *button)
{
	(void)delegate;

	const struct mlk_button_style *style = MLK__STYLE(button, mlk_button_style);

	mlk_painter_set_color(style->border_color);
	mlk_painter_draw_rectangle(button->x, button->y, button->w, button->h);
	mlk_painter_set_color(style->bg_color);
	mlk_painter_draw_rectangle(
		button->x + style->border_size,
		button->y + style->border_size,
		button->w - (style->border_size * 2),
		button->h - (style->border_size * 2)
	);
}

static void
delegate_draw_text(struct mlk_button_delegate *delegate, const struct mlk_button *button)
{
	(void)delegate;

	const struct mlk_button_style *style = MLK__STYLE(button, mlk_button_style);

	mlk_ui_draw_text(
		MLK_ALIGN_CENTER,
		style_font(button),
		style->text_color,
		button->text,
		button->x,
		button->y,
		button->w,
		button->h
	);
}

struct mlk_button_style mlk_button_style = {
	.bg_color       = MLK_UI_COLOR_BG,
	.text_color     = MLK_UI_COLOR_TEXT,
	.border_color   = MLK_UI_COLOR_BORDER,
	.border_size    = 1
};

struct mlk_button_delegate mlk_button_delegate = {
	.draw_frame     = delegate_draw_frame,
	.draw_text      = delegate_draw_text
};

int
mlk_button_ok(const struct mlk_button *button)
{
	if (!button)
		return 0;
	if (button->text && strlen(button->text) == 0)
		return 0;

	return 1;
}

int
mlk_button_handle(struct mlk_button *button, const union mlk_event *ev)
{
	assert(button);
	assert(ev);

	int active = 0;

	switch (ev->type) {
	case MLK_EVENT_CLICKDOWN:
		if (is_boxed(button, &ev->click))
			button->pressed = 1;
		break;
	case MLK_EVENT_CLICKUP:
		/*
		 * If the button was pressed, indicate that the button is
		 * finally activated. This let the user to move the cursor
		 * outside the button to "forget" the press.
		 */
		button->pressed = 0;

		if (is_boxed(button, &ev->click))
			active = 1;
		break;
	default:
		break;
	}

	return active;
}

void
mlk_button_update(struct mlk_button *button, unsigned int ticks)
{
	assert(button);

	MLK__DELEGATE_INVOKE(button->delegate, mlk_button_delegate, update, button, ticks);
}

void
mlk_button_draw(const struct mlk_button *button)
{
	assert(button);

	MLK__DELEGATE_INVOKE(button->delegate, mlk_button_delegate, draw_frame, button);

	if (button->text)
		MLK__DELEGATE_INVOKE(button->delegate, mlk_button_delegate, draw_text, button);
}
