/*
 * checkbox.c -- GUI checkbox
 *
 * Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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

#include <core/action.h>
#include <core/event.h>
#include <core/maths.h>
#include <core/painter.h>

#include "label.h"
#include "checkbox.h"
#include "theme.h"

static int
is_boxed(const struct checkbox *cb, const struct event_click *click)
{
	assert(cb);
	assert(click && click->type == EVENT_CLICKDOWN);

	return maths_is_boxed(cb->x, cb->y, cb->w, cb->h, click->x, click->y);
}

static void
handle(struct action *act, const union event *ev)
{
	checkbox_handle(act->data, ev);
}

static void
draw(struct action *act)
{
	checkbox_draw(act->data);
}

void
checkbox_draw_default(const struct theme *t, const struct checkbox *cb)
{
	(void)t;

	assert(cb);

	painter_set_color(0x151d28ff);
	painter_draw_rectangle(cb->x, cb->y, cb->w, cb->h);
	painter_set_color(0xd7b594ff);
	painter_draw_rectangle(cb->x + 1, cb->y + 1, cb->w - 2, cb->h - 2);

	if (cb->checked) {
		painter_set_color(0x341c27ff);
		painter_draw_rectangle(cb->x + 5, cb->y + 5, cb->w - 10, cb->h - 10);
	}
}

void
checkbox_handle(struct checkbox *cb, const union event *ev)
{
	assert(cb);
	assert(ev);

	switch (ev->type) {
	case EVENT_CLICKDOWN:
		if (is_boxed(cb, &ev->click))
			cb->checked = !cb->checked;
		break;
	default:
		break;
	}
}

void
checkbox_draw(const struct checkbox *cb)
{
	theme_draw_checkbox(cb->theme, cb);
}

void
checkbox_action(struct checkbox *cb, struct action *act)
{
	assert(cb);
	assert(act);

	memset(act, 0, sizeof (*act));
	act->data = cb;
	act->handle = handle;
	act->draw = draw;
}
