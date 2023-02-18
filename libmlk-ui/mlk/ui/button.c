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

static int
is_boxed(const struct button *button, const struct mlk_event_click *click)
{
	assert(button);
	assert(click);
	assert(click->type == MLK_EVENT_CLICKDOWN || click->type == MLK_EVENT_CLICKUP);

	return mlk_maths_is_boxed(button->x, button->y, button->w, button->h,
	    click->x, click->y);
}

void
button_draw_default(const struct theme *t, const struct button *button)
{
	assert(t);
	assert(button);

	(void)t;

	struct label label = {
		.text = button->text,
	};
	unsigned int lw, lh;

	label_query(&label, &lw, &lh);

	if (lw > button->w)
		tracef("button width is too small for text: %u < %u", button->w, lw);
	if (lh > button->h)
		tracef("button height is too small for text: %u < %u", button->h, lh);

	align(ALIGN_CENTER, &label.x, &label.y, lw, lh,
	    button->x, button->y, button->w, button->h);

	painter_set_color(0x577277ff);
	painter_draw_rectangle(button->x, button->y, button->w, button->h);

	label_draw(&label);
}

int
button_handle(struct button *button, const union mlk_event *ev)
{
	assert(button);
	assert(ev);

	switch (ev->type) {
	case MLK_EVENT_CLICKDOWN:
		if (is_boxed(button, &ev->click))
			button->state = BUTTON_STATE_PRESSED;
		break;
	case MLK_EVENT_CLICKUP:
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

	return button->state == BUTTON_STATE_ACTIVATED;
}

void
button_reset(struct button *button)
{
	assert(button);

	button->state = BUTTON_STATE_NONE;
}

void
button_draw(const struct button *button)
{
	assert(button);

	theme_draw_button(button->theme, button);
}
