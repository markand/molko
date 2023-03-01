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
#include "label.h"
#include "theme.h"

#define STYLE_INVOKE(s, f, ...)                                                 \
do {                                                                            \
        if (s && s->f)                                                          \
                s->f(s, __VA_ARGS__);                                           \
        else if (mlk_button_style.f)                                            \
                mlk_button_style.f(s ? s : &mlk_button_style, __VA_ARGS__);     \
} while (0)

static void
draw(struct mlk_button_style *style, const struct mlk_button *button)
{
	struct mlk_label label = {
		.text = button->text,
	};
	unsigned int lw, lh;

	// TODO: once label has style, copy color.
	mlk_label_query(&label, &lw, &lh);

	if (lw > button->w)
		mlk_tracef("button width is too small for text: %u < %u", button->w, lw);
	if (lh > button->h)
		mlk_tracef("button height is too small for text: %u < %u", button->h, lh);

	mlk_align(MLK_ALIGN_CENTER, &label.x, &label.y, lw, lh,
	    button->x, button->y, button->w, button->h);

	mlk_painter_set_color(style->bg_color);
	mlk_painter_draw_rectangle(button->x, button->y, button->w, button->h);

	mlk_label_draw(&label);
}

static inline int
is_boxed(const struct mlk_button *button, const struct mlk_event_click *click)
{
	assert(button);
	assert(click);
	assert(click->type == MLK_EVENT_CLICKDOWN || click->type == MLK_EVENT_CLICKUP);

	return mlk_maths_is_boxed(button->x, button->y, button->w, button->h,
	    click->x, click->y);
}

struct mlk_button_style mlk_button_style = {
	.bg_color = 0x577277ff,
	.text_color = 0xffffffff,
	.draw = draw
};

void
mlk_button_init(struct mlk_button *button)
{
	assert(button);

	STYLE_INVOKE(button->style, init, button);
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

	STYLE_INVOKE(button->style, update, button, ticks);
}

void
mlk_button_draw(const struct mlk_button *button)
{
	assert(button);

	STYLE_INVOKE(button->style, draw, button);
}

void
mlk_button_finish(struct mlk_button *button)
{
	assert(button);

	STYLE_INVOKE(button->style, finish, button);
}
