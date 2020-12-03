/*
 * animation.h -- basic animations
 *
 * Copyright (c) 2020 David Demelier <markand@malikania.fr>
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

#ifndef MOLKO_CORE_ANIMATION_H
#define MOLKO_CORE_ANIMATION_H

#include <stdbool.h>

struct drawable;
struct sprite;

struct animation {
	const struct sprite *sprite;
	unsigned int row;
	unsigned int column;
	unsigned int delay;
	unsigned int elapsed;
};

void
animation_init(struct animation *an, const struct sprite *sprite, unsigned int delay);

void
animation_start(struct animation *an);

bool
animation_completed(const struct animation *an);

bool
animation_update(struct animation *an, unsigned int ticks);

bool
animation_draw(const struct animation *an, int x, int y);

void
animation_drawable(struct animation *an, struct drawable *dw, int x, int y);

#endif /* !MOLKO_CORE_ANIMATION_H */
