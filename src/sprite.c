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
#include "texture_p.h"
#include "texture.h"

void
sprite_init(struct sprite *sprite, struct texture *tex, uint8_t cellw, uint8_t cellh)
{
	assert(sprite);
	assert(tex);

	sprite->texture = tex;
	sprite->cellw = cellw;
	sprite->cellh = cellh;
}

void
sprite_draw(struct sprite *sprite, unsigned r, unsigned c, int x, int y)
{
	assert(sprite);

	texture_draw_ex(
		sprite->texture,
		r * sprite->cellw,      /* src x */
		c * sprite->cellh,      /* src y */
		sprite->cellw,          /* src width */
		sprite->cellh,          /* src height */
		x,                      /* dst x */
		y,                      /* dst y */
		sprite->cellw,          /* dst width */
		sprite->cellh,          /* dst height */
		0.0                     /* angle */
	);
}
