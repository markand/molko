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

#define STYLE_INVOKE(s, f, ...)                                                 \
do {                                                                            \
        if (s && s->f)                                                          \
                s->f(s, __VA_ARGS__);                                           \
        else if (mlk_checkbox_style.f)                                          \
                mlk_checkbox_style.f(s ? s : &mlk_checkbox_style, __VA_ARGS__); \
} while (0)

static int
is_boxed(const struct mlk_checkbox *cb, const struct mlk_event_click *click)
{
	assert(cb);
	assert(click && click->type == MLK_EVENT_CLICKDOWN);

	return mlk_maths_is_boxed(cb->x, cb->y, cb->w, cb->h, click->x, click->y);
}

static void
draw(struct mlk_checkbox_style *style, const struct mlk_checkbox *cb)
{
	mlk_painter_set_color(style->bg_color);
	mlk_painter_draw_rectangle(cb->x, cb->y, cb->w, cb->h);
	mlk_painter_set_color(style->border_color);
	mlk_painter_draw_rectangle(cb->x + 1, cb->y + 1, cb->w - 2, cb->h - 2);

	if (cb->checked) {
		mlk_painter_set_color(style->check_color);
		mlk_painter_draw_rectangle(cb->x + 5, cb->y + 5, cb->w - 10, cb->h - 10);
	}
}

struct mlk_checkbox_style mlk_checkbox_style = {
	.bg_color       = 0x151d28ff,
	.border_color   = 0xd7b594ff,
	.check_color    = 0x341c27ff,
	.draw           = draw
};

void
mlk_checkbox_init(struct mlk_checkbox *cb)
{
	assert(cb);

	STYLE_INVOKE(cb->style, init, cb);
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
	assert(cb);

	STYLE_INVOKE(cb->style, draw, cb);
}

void
mlk_checkbox_finish(struct mlk_checkbox *cb)
{
	assert(cb);

	STYLE_INVOKE(cb->style, finish, cb);

}
