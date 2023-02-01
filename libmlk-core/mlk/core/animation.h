/*
 * animation.h -- basic animations
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

#ifndef MLK_CORE_ANIMATION_H
#define MLK_CORE_ANIMATION_H

#include "core.h"

struct drawable;
struct sprite;

struct animation {
	const struct sprite *sprite;
	unsigned int row;
	unsigned int column;
	unsigned int delay;
	unsigned int elapsed;
};

CORE_BEGIN_DECLS

void
animation_init(struct animation *, const struct sprite *, unsigned int);

void
animation_start(struct animation *);

int
animation_completed(const struct animation *);

int
animation_update(struct animation *, unsigned int);

int
animation_draw(const struct animation *, int, int);

void
animation_drawable(struct animation *, struct drawable *, int, int);

CORE_END_DECLS

#endif /* !MLK_CORE_ANIMATION_H */
