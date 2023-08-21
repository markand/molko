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

#include <mlk/core/color.h>
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

	return mlk_maths_is_boxed(click->x, click->y,
	    button->x, button->y, button->w, button->h);
}

static void
draw_frame(struct mlk_button_style *self, struct mlk_button *button)
{
	unsigned long background = self->background;
	unsigned long border = self->border;

	if (button->pressed) {
		background = mlk_color_darken(background, 0.9);
		border = mlk_color_darken(border, 0.9);
	}

	mlk_painter_set_color(border);
	mlk_painter_draw_rectangle(button->x, button->y, button->w, button->h);
	mlk_painter_set_color(background);
	mlk_painter_draw_rectangle(
		button->x + self->border_size,
		button->y + self->border_size,
		button->w - (self->border_size * 2),
		button->h - (self->border_size * 2)
	);
}

static void
draw_text(struct mlk_button_style *style, struct mlk_button *button)
{
	mlk_ui_draw_text(
		MLK_ALIGN_CENTER,
		MLK__STYLE_FONT(style->font, MLK_UI_FONT_INTERFACE),
		style->color,
		button->text,
		button->x,
		button->y,
		button->w,
		button->h
	);
}

struct mlk_button_style mlk_button_style_dark = {
	.background = 0x222323ff,
	.border = 0x141414ff,
	.border_size = 2,
	.color = 0xffffffff,
	.draw_frame = draw_frame,
	.draw_text = draw_text
};

struct mlk_button_style mlk_button_style_light = {
	.background = 0xf5f7faff,
	.border = 0xcdd2daff,
	.border_size = 2,
	.color = 0x000000ff,
	.draw_frame = draw_frame,
	.draw_text = draw_text
};

struct mlk_button_style *mlk_button_style = &mlk_button_style_light;

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

	MLK__STYLE_CALL(button->style, mlk_button_style, update, button, ticks);
}

void
mlk_button_draw(struct mlk_button *button)
{
	assert(button);

	MLK__STYLE_CALL(button->style, mlk_button_style, draw_frame, button);

	if (button->text)
		MLK__STYLE_CALL(button->style, mlk_button_style, draw_text, button);
}
