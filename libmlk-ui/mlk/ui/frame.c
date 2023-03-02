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
#include <stddef.h>

#include <mlk/core/painter.h>

#include "frame.h"
#include "ui.h"
#include "ui_p.h"

static void
delegate_draw(struct mlk_frame_delegate *delegate, const struct mlk_frame *frame)
{
	(void)delegate;

	const struct mlk_frame_style *style = MLK__STYLE(frame, mlk_frame_style);

	mlk_painter_set_color(style->border_color);
	mlk_painter_draw_rectangle(frame->x, frame->y, frame->w, frame->h);
	mlk_painter_set_color(style->bg_color);
	mlk_painter_draw_rectangle(
		frame->x + style->border_size,
		frame->y + style->border_size,
		frame->w - (style->border_size * 2),
		frame->h - (style->border_size * 2)
	);
}

struct mlk_frame_style mlk_frame_style = {
	.bg_color       = MLK_UI_COLOR_BG,
	.border_color   = MLK_UI_COLOR_BORDER,
	.border_size    = 2
};

struct mlk_frame_delegate mlk_frame_delegate = {
	.draw           = delegate_draw
};

int
mlk_frame_ok(const struct mlk_frame *frame)
{
	return frame != NULL;
}

void
mlk_frame_update(struct mlk_frame *frame, unsigned int ticks)
{
	assert(frame);

	MLK__DELEGATE_INVOKE(frame->delegate, mlk_frame_delegate, update, frame, ticks);
}

void
mlk_frame_draw(const struct mlk_frame *frame)
{
	assert(frame);

	MLK__DELEGATE_INVOKE(frame->delegate, mlk_frame_delegate, draw, frame);
}
