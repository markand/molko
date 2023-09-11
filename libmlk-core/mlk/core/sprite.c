/*
 * sprite.c -- image sprites
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

#include "sprite.h"
#include "texture.h"

void
mlk_sprite_init(struct mlk_sprite *sprite)
{
	assert(sprite);

	sprite->nrows = sprite->texture->h / sprite->cellh;
	sprite->ncols = sprite->texture->w / sprite->cellw;
}

int
mlk_sprite_ok(const struct mlk_sprite *sprite)
{
	return sprite && sprite->cellw && sprite->cellh && sprite->texture;
}

int
mlk_sprite_draw(const struct mlk_sprite *sprite, unsigned int r, unsigned int c, int x, int y)
{
	assert(mlk_sprite_ok(sprite));

	return mlk_sprite_scale(sprite, r, c, x, y, sprite->cellw, sprite->cellh);
}

int
mlk_sprite_scale(const struct mlk_sprite *sprite,
	     unsigned int r,
	     unsigned int c,
	     int x,
	     int y,
	     unsigned int w,
	     unsigned int h)
{
	assert(mlk_sprite_ok(sprite));
	assert(r < sprite->nrows);
	assert(c < sprite->ncols);

	return mlk_texture_scale(
		sprite->texture,
		c * sprite->cellw,      /* src y */
		r * sprite->cellh,      /* src x */
		sprite->cellw,          /* src width */
		sprite->cellh,          /* src height */
		x,                      /* dst x */
		y,                      /* dst y */
		w,                      /* dst width */
		h,                      /* dst height */
		0.0                     /* angle */
	);
}
