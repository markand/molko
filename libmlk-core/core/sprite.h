/*
 * sprite.h -- image sprites
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

#ifndef MOLKO_CORE_SPRITE_H
#define MOLKO_CORE_SPRITE_H

#include <stdbool.h>

#include "core.h"

struct texture;

struct sprite {
	struct texture *texture;
	unsigned int cellw;
	unsigned int cellh;
	unsigned int nrows;
	unsigned int ncols;
};

CORE_BEGIN_DECLS

void
sprite_init(struct sprite *sprite,
            struct texture *tex,
            unsigned int cellw,
            unsigned int cellh);

bool
sprite_ok(const struct sprite *sprite);

bool
sprite_draw(const struct sprite *sprite, unsigned int r, unsigned int c, int x, int y);

bool
sprite_scale(const struct sprite *sprite,
             unsigned int r,
             unsigned int c,
             int x,
             int y,
             unsigned int w,
             unsigned int h);

CORE_END_DECLS

#endif /* !MOLKO_CORE_SPRITE_H */
