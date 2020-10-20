/*
 * sprite.c -- image sprites
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

#include <assert.h>

#include "sprite.h"
#include "texture.h"

void
sprite_init(struct sprite *sprite,
            struct texture *tex,
            unsigned int cellw,
            unsigned int cellh)
{
	assert(sprite);
	assert(tex && texture_ok(tex));

	sprite->texture = tex;
	sprite->cellw = cellw;
	sprite->cellh = cellh;
	sprite->nrows = tex->h / cellh;
	sprite->ncols = tex->w / cellw;
}

bool
sprite_ok(const struct sprite *sprite)
{
	if (!sprite)
		return false;

	return texture_ok(sprite->texture) && sprite->cellw != 0 && sprite->cellh != 0;
}

void
sprite_draw(struct sprite *sprite, unsigned int r, unsigned int c, int x, int y)
{
	assert(sprite_ok(sprite));
	assert(r < sprite->nrows);
	assert(c < sprite->ncols);

	texture_scale(
		sprite->texture,
		c * sprite->cellw,      /* src y */
		r * sprite->cellh,      /* src x */
		sprite->cellw,          /* src width */
		sprite->cellh,          /* src height */
		x,                      /* dst x */
		y,                      /* dst y */
		sprite->cellw,          /* dst width */
		sprite->cellh,          /* dst height */
		0.0                     /* angle */
	);
}
