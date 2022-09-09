/*
 * frame.h -- GUI frame
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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

#ifndef MLK_UI_FRAME_H
#define MLK_UI_FRAME_H

#include <core/core.h>

struct theme;

enum frame_style {
	FRAME_STYLE_NORMAL,
	FRAME_STYLE_BOX
};

struct frame {
	int x;
	int y;
	unsigned int w;
	unsigned int h;
	enum frame_style style;
	const struct theme *theme;
};

CORE_BEGIN_DECLS

void
frame_draw_default(const struct theme *, const struct frame *);

void
frame_draw(const struct frame *);

CORE_END_DECLS

#endif /* !MLK_UI_FRAME_H */
