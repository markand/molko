/*
 * button.c -- GUI button
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

#include <core/event.h>
#include <core/maths.h>
#include <core/painter.h>

#include "button.h"
#include "label.h"
#include "theme.h"

static bool
is_boxed(const struct button *button, const struct event_click *click)
{
	assert(button);
	assert(click);
	assert(click->type == EVENT_CLICKDOWN || click->type == EVENT_CLICKUP);

	return maths_is_boxed(button->x, button->y, button->w, button->h,
	    click->x, click->y);
}

void
button_draw_default(struct theme *t, const struct button *button)
{
	assert(t);
	assert(button);

	(void)t;

	const struct label label = {
		.text = button->text,
		.x = button->x,
		.y = button->y,
		.w = button->w,
		.h = button->h,
		.align = ALIGN_CENTER
	};

	painter_set_color(0x577277ff);
	painter_draw_rectangle(button->x, button->y, button->w, button->h);

	label_draw(&label);
}

void
button_handle(struct button *button, const union event *ev)
{
	assert(button);
	assert(ev);

	switch (ev->type) {
	case EVENT_CLICKDOWN:
		if (is_boxed(button, &ev->click))
			button->state = BUTTON_STATE_PRESSED;
		break;
	case EVENT_CLICKUP:
		/*
		 * If the button was pressed, indicate that the button is
		 * finally activated. This let the user to move the cursor
		 * outside the button to "forget" the press.
		 */
		if (!is_boxed(button, &ev->click))
			button->state = BUTTON_STATE_NONE;
		else if (button->state == BUTTON_STATE_PRESSED)
			button->state = BUTTON_STATE_ACTIVATED;
		break;
	default:
		break;
	}
}

void
button_reset(struct button *button)
{
	assert(button);

	button->state = BUTTON_STATE_NONE;
}

void
button_draw(struct button *button)
{
	assert(button);

	theme_draw_button(button->theme, button);
}
