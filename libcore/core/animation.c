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
#include <string.h>

#include "drawable.h"
#include "animation.h"
#include "sprite.h"

static bool
update(struct drawable *dw, unsigned int ticks)
{
	return animation_update(dw->data, ticks);
}

static void
draw(struct drawable *dw)
{
	animation_draw(dw->data, dw->x, dw->y);
}

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

void
animation_start(struct animation *an)
{
	assert(an);

	an->row = 0;
	an->column = 0;
	an->elapsed = 0;
}

bool
animation_completed(const struct animation *an)
{
	return an->elapsed >= an->delay &&
	       an->row >= an->sprite->nrows &&
	       an->column >= an->sprite->ncols;
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
			an->column = an->elapsed = 0;
	} else
		an->elapsed = 0;

	return animation_completed(an);
}

bool
animation_draw(const struct animation *an, int x, int y)
{
	return sprite_draw(an->sprite, an->row, an->column, x, y);
}

void
animation_drawable(struct animation *an, struct drawable *dw, int x, int y)
{
	assert(an);
	assert(dw);

	memset(dw, 0, sizeof (*dw));
	
	dw->data = an;
	dw->x = x - (an->sprite->cellw / 2);
	dw->y = y - (an->sprite->cellh / 2);
	dw->update = update;
	dw->draw = draw;

	animation_start(an);
}
