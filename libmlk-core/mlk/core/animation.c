/*
 * animation.c -- basic animations
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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
mlk_animation_start(struct mlk_animation *an)
{
	assert(an);

	an->row = 0;
	an->column = 0;
	an->elapsed = 0;
}

int
mlk_animation_completed(const struct mlk_animation *an)
{
	assert(an);

	return an->elapsed >= an->delay &&
	       an->row >= an->sprite->nrows &&
	       an->column >= an->sprite->ncols;
}

int
mlk_animation_update(struct mlk_animation *an, unsigned int ticks)
{
	assert(an);
	assert(!mlk_animation_completed(an));

	int complete;

	an->elapsed += ticks;

	if (an->elapsed < an->delay)
		return 0;

	/* Increment column first */
	if (++an->column >= an->sprite->ncols) {
		/*
		 * Increment row, if we reach the last row it means we are
		 * at the last frame.
		 */
		if (++an->row >= an->sprite->nrows)
			an->row = an->sprite->nrows;
		else
			an->column = an->elapsed = 0;
	} else
		an->elapsed = 0;

	complete = mlk_animation_completed(an);

	/* Animation looping? Reset automatically. */
	if (complete && (an->flags & MLK_ANIMATION_FLAGS_LOOP)) {
		mlk_animation_start(an);
		complete = 0;
	}

	return complete;
}

int
mlk_animation_draw(const struct mlk_animation *an, int x, int y)
{
	assert(an);
	assert(!mlk_animation_completed(an));

	return mlk_sprite_draw(an->sprite, an->row, an->column, x, y);
}
