/*
 * drawable.h -- automatic drawable objects
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

#ifndef MLK_CORE_DRAWABLE_H
#define MLK_CORE_DRAWABLE_H

#include "core.h"

struct mlk_drawable {
	void *data;
	int x;
	int y;
	int (*update)(struct mlk_drawable *, unsigned int);
	void (*draw)(struct mlk_drawable *);
	void (*end)(struct mlk_drawable *);
	void (*finish)(struct mlk_drawable *);
};

MLK_CORE_BEGIN_DECLS

int
mlk_drawable_update(struct mlk_drawable *, unsigned int);

void
mlk_drawable_draw(struct mlk_drawable *);

void
mlk_drawable_end(struct mlk_drawable *);

void
mlk_drawable_finish(struct mlk_drawable *);

MLK_CORE_END_DECLS

#endif /* !MLK_CORE_DRAWABLE_H */
