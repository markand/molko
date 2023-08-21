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
#include "ui_p.h"

static void
draw(struct mlk_frame_style *self, struct mlk_frame *frame)
{
	mlk_painter_set_color(self->border);
	mlk_painter_draw_rectangle(frame->x, frame->y, frame->w, frame->h);
	mlk_painter_set_color(self->background);
	mlk_painter_draw_rectangle(
		frame->x + self->border_size,
		frame->y + self->border_size,
		frame->w - (self->border_size * 2),
		frame->h - (self->border_size * 2)
	);
}


struct mlk_frame_style mlk_frame_style_dark = {
	.background = 0x222323ff,
	.border = 0x141414ff,
	.border_size = 2,
	.draw = draw
};

struct mlk_frame_style mlk_frame_style_light = {
	.background = 0xf5f7faff,
	.border = 0xcdd2daff,
	.border_size = 2,
	.draw = draw
};

struct mlk_frame_style *mlk_frame_style = &mlk_frame_style_light;

void
mlk_frame_update(struct mlk_frame *frame, unsigned int ticks)
{
	assert(frame);

	MLK__STYLE_CALL(frame->style, mlk_frame_style, update, frame, ticks);
}

void
mlk_frame_draw(struct mlk_frame *frame)
{
	assert(frame);

	MLK__STYLE_CALL(frame->style, mlk_frame_style, draw, frame);
}
