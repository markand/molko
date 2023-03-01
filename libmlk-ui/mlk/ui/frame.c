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

#define STYLE_INVOKE(s, f, ...)                                                 \
do {                                                                            \
        if (s && s->f)                                                          \
                s->f(s, __VA_ARGS__);                                           \
        else if (mlk_frame_style.f)                                             \
                mlk_frame_style.f(s ? s : &mlk_frame_style, __VA_ARGS__);       \
} while (0)

static void
draw(struct mlk_frame_style *style, const struct mlk_frame *frame)
{
	mlk_painter_set_color(style->border_color);
	mlk_painter_draw_rectangle(frame->x, frame->y, frame->w, frame->h);
	mlk_painter_set_color(style->bg_color);
	mlk_painter_draw_rectangle(frame->x + 1, frame->y + 1, frame->w - 2, frame->h - 2);
}

struct mlk_frame_style mlk_frame_style = {
	.bg_color       = 0xf5f7faff,
	.border_color   = 0xcdd2daff,
	.draw           = draw
};

void
mlk_frame_init(struct mlk_frame *frame)
{
	assert(frame);

	STYLE_INVOKE(frame->style, init, frame);
}

void
mlk_frame_draw(const struct mlk_frame *frame)
{
	assert(frame);

	STYLE_INVOKE(frame->style, draw, frame);
}

void
mlk_frame_finish(struct mlk_frame *frame)
{
	assert(frame);

	STYLE_INVOKE(frame->style, finish, frame);
}
