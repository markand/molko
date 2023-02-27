/*
 * sprite.h -- image sprites
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

#ifndef MLK_CORE_SPRITE_H
#define MLK_CORE_SPRITE_H

#include "core.h"

struct mlk_texture;

struct mlk_sprite {
	struct mlk_texture *texture;
	unsigned int cellw;
	unsigned int cellh;
	unsigned int nrows;
	unsigned int ncols;
};

MLK_CORE_BEGIN_DECLS

void
mlk_sprite_init(struct mlk_sprite *, struct mlk_texture *, unsigned int, unsigned int);

int
mlk_sprite_ok(const struct mlk_sprite *);

int
mlk_sprite_draw(const struct mlk_sprite *, unsigned int, unsigned int, int, int);

int
mlk_sprite_scale(const struct mlk_sprite *,
             unsigned int,
             unsigned int,
             int,
             int,
             unsigned int,
             unsigned int);

MLK_CORE_END_DECLS

#endif /* !MLK_CORE_SPRITE_H */