/*
 * animation.c -- basic animations
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

#include "animation.h"
#include "sprite.h"

void
animation_init(struct animation *an, struct sprite *sprite, unsigned int delay)
{
	assert(an);
	assert(sprite);

	an->sprite = sprite;
	an->row = 0;
	an->column = 0;
	an->delay = delay;
	an->elapsed = 0;
}

bool
animation_is_complete(const struct animation *an)
{
	assert(an);

	return an->row == an->sprite->nrows &&
	       an->column == an->sprite->ncols &&
	       an->elapsed >= an->delay;
}

void
animation_start(struct animation *an)
{
	assert(an);

	an->row = 0;
	an->column = 0;
	an->elapsed = 0;
}

bool
animation_update(struct animation *an, unsigned int ticks)
{
	assert(an);

	an->elapsed += ticks;

	if (an->elapsed < an->delay)
		return false;

	/* Increment column first */
	if (++an->column >= an->sprite->ncols) {
		/*
		 * Increment row, if we reach the last row it means we are
		 * at the last frame.
		 */
		if (++an->row >= an->sprite->nrows)
			an->row = an->sprite->nrows;
		else
			an->column = 0;
	}

	return an->elapsed >= an->delay &&
	       an->row >= an->sprite->nrows &&
	       an->column >= an->sprite->ncols;
}

void
animation_draw(struct animation *an, int x, int y)
{
	sprite_draw(an->sprite, an->row, an->column, x, y);
}
