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

#include <mlk/core/color.h>
#include <mlk/core/event.h>
#include <mlk/core/maths.h>
#include <mlk/core/painter.h>

#include "checkbox.h"
#include "style.h"
#include "ui_p.h"

static inline int
is_boxed(const struct mlk_checkbox *cb, const struct mlk_event_click *click)
{
	assert(click && click->type == MLK_EVENT_CLICKDOWN);

	return mlk_maths_is_boxed(click->x, click->y, cb->x, cb->y, cb->w, cb->h);
}

static void
draw(struct mlk_checkbox_style *self, struct mlk_checkbox *cb)
{
	if (!self->border_size) {
		mlk_painter_set_color(self->background);
		mlk_painter_draw_rectangle(cb->x, cb->y, cb->w, cb->h);
	} else {
		mlk_painter_set_color(self->border);
		mlk_painter_draw_rectangle(cb->x, cb->y, cb->w, cb->h);
		mlk_painter_set_color(self->background);
		mlk_painter_draw_rectangle(
			cb->x + self->border_size,
			cb->y + self->border_size,
			cb->w - (self->border_size * 2),
			cb->h - (self->border_size * 2)
		);
	}

	if (cb->checked) {
		mlk_painter_set_color(mlk_color_darken(self->background, 0.9));
		mlk_painter_draw_rectangle(cb->x + 5, cb->y + 5, cb->w - 10, cb->h - 10);
	}
}

struct mlk_checkbox_style mlk_checkbox_style_dark = {
	.background = 0x222323ff,
	.border = 0x141414ff,
	.border_size = 2,
	.draw = draw
};

struct mlk_checkbox_style mlk_checkbox_style_light = {
	.background = 0xf5f7faff,
	.border = 0xcdd2daff,
	.border_size = 2,
	.draw = draw
};

struct mlk_checkbox_style *mlk_checkbox_style = &mlk_checkbox_style_light;

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
mlk_checkbox_update(struct mlk_checkbox *cb, unsigned int ticks)
{
	assert(cb);

	MLK__STYLE_CALL(cb->style, mlk_checkbox_style, update, cb, ticks);
}

void
mlk_checkbox_draw(struct mlk_checkbox *cb)
{
	assert(cb);

	MLK__STYLE_CALL(cb->style, mlk_checkbox_style, draw, cb);
}
