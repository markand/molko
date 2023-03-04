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

#include "checkbox.h"
#include "ui.h"
#include "ui_p.h"

static int
is_boxed(const struct mlk_checkbox *cb, const struct mlk_event_click *click)
{
	assert(cb);
	assert(click && click->type == MLK_EVENT_CLICKDOWN);

	return mlk_maths_is_boxed(click->x, click->y, cb->x, cb->y, cb->w, cb->h);
}

static void
draw(struct mlk_checkbox_delegate *delegate, const struct mlk_checkbox *cb)
{
	(void)delegate;

	const struct mlk_checkbox_style *style = MLK__STYLE(cb, mlk_checkbox_style);

	if (!style->border_size) {
		mlk_painter_set_color(style->bg_color);
		mlk_painter_draw_rectangle(cb->x, cb->y, cb->w, cb->h);
	} else {
		mlk_painter_set_color(style->border_color);
		mlk_painter_draw_rectangle(cb->x, cb->y, cb->w, cb->h);
		mlk_painter_set_color(style->bg_color);
		mlk_painter_draw_rectangle(
			cb->x + style->border_size,
			cb->y + style->border_size,
			cb->w - (style->border_size * 2),
			cb->h - (style->border_size * 2)
		);
	}

	if (cb->checked) {
		mlk_painter_set_color(style->check_color);
		mlk_painter_draw_rectangle(cb->x + 5, cb->y + 5, cb->w - 10, cb->h - 10);
	}
}

struct mlk_checkbox_style mlk_checkbox_style = {
	.bg_color = MLK_UI_COLOR_BG,
	.check_color = MLK_UI_COLOR_TEXT,
	.border_color = MLK_UI_COLOR_BORDER,
	.border_size = 1
};

struct mlk_checkbox_delegate mlk_checkbox_delegate = {
	.draw = draw
};

int
mlk_checkbox_ok(const struct mlk_checkbox *cb)
{
	return cb != NULL;
}

int
mlk_checkbox_handle(struct mlk_checkbox *cb, const union mlk_event *ev)
{
	assert(mlk_checkbox_ok(cb));
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
mlk_checkbox_update(struct mlk_checkbox *cb, unsigned int ticks)
{
	assert(mlk_checkbox_ok(cb));

	MLK__DELEGATE_INVOKE(cb->delegate, mlk_checkbox_delegate, update, cb, ticks);
}

void
mlk_checkbox_draw(const struct mlk_checkbox *cb)
{
	assert(mlk_checkbox_ok(cb));

	MLK__DELEGATE_INVOKE(cb->delegate, mlk_checkbox_delegate, draw, cb);
}
