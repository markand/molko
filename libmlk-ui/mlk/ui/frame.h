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

#ifndef MLK_UI_FRAME_H
#define MLK_UI_FRAME_H

#include <mlk/core/core.h>

struct mlk_frame;

struct mlk_frame_style {
	unsigned long bg_color;
	unsigned long border_color;
	void (*init)(struct mlk_frame_style *, struct mlk_frame *);
	void (*update)(struct mlk_frame_style *, struct mlk_frame *, unsigned int);
	void (*draw)(struct mlk_frame_style *, const struct mlk_frame *);
	void (*finish)(struct mlk_frame_style *, struct mlk_frame *);
};

struct mlk_frame {
	int x, y;
	unsigned int w, h;
	struct mlk_frame_style *style;
};

extern struct mlk_frame_style mlk_frame_style;

MLK_CORE_BEGIN_DECLS

void
mlk_frame_init(struct mlk_frame *);

void
mlk_frame_draw(const struct mlk_frame *);

void
mlk_frame_finish(struct mlk_frame *);

MLK_CORE_END_DECLS

#endif /* !MLK_UI_FRAME_H */
