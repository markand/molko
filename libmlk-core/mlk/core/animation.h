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

struct mlk_drawable;
struct sprite;

struct mlk_animation {
	const struct sprite *sprite;
	unsigned int row;
	unsigned int column;
	unsigned int delay;
	unsigned int elapsed;
};

MLK_CORE_BEGIN_DECLS

void
mlk_animation_init(struct mlk_animation *, const struct sprite *, unsigned int);

void
mlk_animation_start(struct mlk_animation *);

int
mlk_animation_completed(const struct mlk_animation *);

int
mlk_animation_update(struct mlk_animation *, unsigned int);

int
mlk_animation_draw(const struct mlk_animation *, int, int);

void
mlk_animation_drawable(struct mlk_animation *, struct mlk_drawable *, int, int);

MLK_CORE_END_DECLS

#endif /* !MLK_CORE_ANIMATION_H */
