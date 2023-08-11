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

#include <mlk/core/event.h>
#include <mlk/core/maths.h>
#include <mlk/core/painter.h>

#include "checkbox.h"
#include "style.h"

static inline int
is_boxed(const struct mlk_checkbox *cb, const struct mlk_event_click *click)
{
	assert(click && click->type == MLK_EVENT_CLICKDOWN);

	return mlk_maths_is_boxed(click->x, click->y, cb->x, cb->y, cb->w, cb->h);
}

static int
handle(struct mlk_checkbox_delegate *self,
       struct mlk_checkbox *cb,
       const union mlk_event *ev)
{
	(void)self;

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

static void
draw(struct mlk_checkbox_delegate *self, const struct mlk_checkbox *cb)
{
	(void)self;

	const struct mlk_style_attr *attr = &cb->style->normal;

	if (!attr->geo.border) {
		mlk_painter_set_color(attr->color.bg);
		mlk_painter_draw_rectangle(cb->x, cb->y, cb->w, cb->h);
	} else {
		mlk_painter_set_color(attr->color.border);
		mlk_painter_draw_rectangle(cb->x, cb->y, cb->w, cb->h);
		mlk_painter_set_color(attr->color.bg);
		mlk_painter_draw_rectangle(
			cb->x + attr->geo.border,
			cb->y + attr->geo.border,
			cb->w - (attr->geo.border * 2),
			cb->h - (attr->geo.border * 2)
		);
	}

	if (cb->checked) {
		mlk_painter_set_color(attr->color.fg);
		mlk_painter_draw_rectangle(cb->x + 5, cb->y + 5, cb->w - 10, cb->h - 10);
	}
}

struct mlk_checkbox_delegate mlk_checkbox_delegate = {
	.handle = handle,
	.draw = draw
};

void
mlk_checkbox_init(struct mlk_checkbox *cb,
                  struct mlk_checkbox_delegate *dt,
                  struct mlk_style *st)
{
	assert(cb);

	cb->x = 0;
	cb->y = 0;
	cb->w = 0;
	cb->h = 0;
	cb->checked = 0;
	cb->delegate = dt ? dt : &mlk_checkbox_delegate;
	cb->style = st ? st : &mlk_style;
}

int
mlk_checkbox_handle(struct mlk_checkbox *cb, const union mlk_event *ev)
{
	assert(cb);
	assert(ev);

	if (cb->delegate->handle)
		return cb->delegate->handle(cb->delegate, cb, ev);

	return 0;
}

void
mlk_checkbox_update(struct mlk_checkbox *cb, unsigned int ticks)
{
	assert(cb);

	if (cb->delegate->update)
		cb->delegate->update(cb->delegate, cb, ticks);
}

void
mlk_checkbox_draw(const struct mlk_checkbox *cb)
{
	assert(cb);

	if (cb->delegate->draw)
		cb->delegate->draw(cb->delegate, cb);
}

void
mlk_checkbox_finish(struct mlk_checkbox *cb)
{
	assert(cb);

	if (cb->delegate->finish)
		cb->delegate->finish(cb->delegate, cb);
}
