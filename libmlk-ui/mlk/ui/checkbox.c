/*
 * checkbox.c -- GUI checkbox
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

#include "label.h"
#include "checkbox.h"
#include "theme.h"

static int
is_boxed(const struct mlk_checkbox *cb, const struct mlk_event_click *click)
{
	assert(cb);
	assert(click && click->type == MLK_EVENT_CLICKDOWN);

	return mlk_maths_is_boxed(cb->x, cb->y, cb->w, cb->h, click->x, click->y);
}

void
mlk_checkbox_draw_default(const struct theme *t, const struct mlk_checkbox *cb)
{
	(void)t;

	assert(cb);

	mlk_painter_set_color(0x151d28ff);
	mlk_painter_draw_rectangle(cb->x, cb->y, cb->w, cb->h);
	mlk_painter_set_color(0xd7b594ff);
	mlk_painter_draw_rectangle(cb->x + 1, cb->y + 1, cb->w - 2, cb->h - 2);

	if (cb->checked) {
		mlk_painter_set_color(0x341c27ff);
		mlk_painter_draw_rectangle(cb->x + 5, cb->y + 5, cb->w - 10, cb->h - 10);
	}
}

int
mlk_checkbox_handle(struct mlk_checkbox *cb, const union mlk_event *ev)
{
	assert(cb);
	assert(ev);

	switch (ev->type) {
	case MLK_EVENT_CLICKDOWN:
		if (is_boxed(cb, &ev->click))
			return cb->checked = !cb->checked;
		break;
	default:
		break;
	}

	return 0;
}

void
mlk_checkbox_draw(const struct mlk_checkbox *cb)
{
	theme_draw_checkbox(cb->theme, cb);
}
