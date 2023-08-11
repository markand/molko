/*
 * frame.h -- GUI frame
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

#include <mlk/core/painter.h>

#include "frame.h"
#include "style.h"

static void
draw(struct mlk_frame_delegate *self, const struct mlk_frame *frame)
{
	(void)self;

	const struct mlk_style_attr *attr = &frame->style->normal;

	mlk_painter_set_color(attr->color.border);
	mlk_painter_draw_rectangle(frame->x, frame->y, frame->w, frame->h);
	mlk_painter_set_color(attr->color.bg);
	mlk_painter_draw_rectangle(
		frame->x + attr->geo.border,
		frame->y + attr->geo.border,
		frame->w - (attr->geo.border * 2),
		frame->h - (attr->geo.border * 2)
	);
}

struct mlk_frame_delegate mlk_frame_delegate = {
	.draw = draw
};

void
mlk_frame_init(struct mlk_frame *frame,
               struct mlk_style *st,
               struct mlk_frame_delegate *dt)
{
	assert(frame);

	frame->x = 0;
	frame->y = 0;
	frame->w = 0;
	frame->h = 0;
	frame->style = st ? st : &mlk_style;
	frame->delegate = dt ? dt : &mlk_frame_delegate;
}

void
mlk_frame_update(struct mlk_frame *frame, unsigned int ticks)
{
	assert(frame);

	if (frame->delegate->update)
		frame->delegate->update(frame->delegate, frame, ticks);
}

void
mlk_frame_draw(const struct mlk_frame *frame)
{
	assert(frame);

	if (frame->delegate->draw)
		frame->delegate->draw(frame->delegate, frame);
}
