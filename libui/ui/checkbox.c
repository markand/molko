/*
 * checkbox.c -- GUI checkbox
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

#include "checkbox.h"
#include "theme.h"

static bool
is_boxed(const struct checkbox *cb, const struct event_click *click)
{
	assert(cb);
	assert(click && click->type == EVENT_CLICKDOWN);

	return maths_is_boxed(cb->x, cb->y, cb->w, cb->h, click->x, click->y);
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
