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
#include <string.h>

#include <mlk/core/painter.h>

#include "frame.h"
#include "theme.h"

void
frame_draw_default(const struct theme *t, const struct frame *frame)
{
	assert(t);
	assert(frame);

	(void)t;

	if (frame->style == FRAME_STYLE_BOX)
		mlk_painter_set_color(0x7a4841ff);
	else
		mlk_painter_set_color(0xad7757ff);

	mlk_painter_draw_rectangle(frame->x, frame->y, frame->w, frame->h);
}

void
frame_draw(const struct frame *frame)
{
	assert(frame);

	theme_draw_frame(frame->theme, frame);
}
