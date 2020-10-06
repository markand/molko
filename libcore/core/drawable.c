/*
 * drawable.c -- automatic drawable objects
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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "animation.h"
#include "drawable.h"
#include "util.h"
#include "sprite.h"

bool
drawable_update(struct drawable *dw, unsigned int ticks)
{
	assert(dw);

	return dw->update(dw, ticks);
}

void
drawable_draw(struct drawable *dw)
{
	assert(dw);

	dw->draw(dw);
}

void
drawable_finish(struct drawable *dw)
{
	if (dw->finish)
		dw->finish(dw);
}

static bool
drawable_animation_update(struct drawable *dw, unsigned int ticks)
{
	return animation_update(dw->data, ticks);
}

static void
drawable_animation_draw(struct drawable *dw)
{
	return animation_draw(dw->data, dw->x, dw->y);
}

static void
drawable_animation_finish(struct drawable *dw)
{
	free(dw->data);
}

void
drawable_from_animation(struct drawable *dw,
			const struct animation *an,
			int x,
			int y)
{
	assert(dw);
	assert(an);

	memset(dw, 0, sizeof (*dw));
	
	dw->data = ememdup(an, sizeof (*an));
	dw->x = x - (an->sprite->cellw / 2);
	dw->y = y - (an->sprite->cellh / 2);
	dw->update = drawable_animation_update;
	dw->draw = drawable_animation_draw;
	dw->finish = drawable_animation_finish;
}

void
drawable_stack_init(struct drawable_stack *st)
{
	assert(st);

	memset(st, 0, sizeof (*st));
}

bool
drawable_stack_add(struct drawable_stack *st, const struct drawable *dw)
{
	/* Find an empty slot. */
	struct drawable *slot = NULL;

	for (size_t i = 0; i < DRAWABLE_STACK_MAX; ++i) {
		struct drawable *dtmp = &st->objects[i];

		dtmp = &st->objects[i];

		if (!dtmp->update && !dtmp->draw) {
			slot = dtmp;
			break;
		}
	}

	if (slot) {
		memcpy(slot, dw, sizeof (*dw));
		return true;
	}
	
	return false;
}

void
drawable_stack_update(struct drawable_stack *st, unsigned int ticks)
{
	assert(st);

	for (size_t i = 0; i < DRAWABLE_STACK_MAX; ++i) {
		struct drawable *dw = &st->objects[i];

		if (dw->update && dw->update(dw, ticks))
			memset(dw, 0, sizeof (*dw));
	}
}

void
drawable_stack_draw(struct drawable_stack *st)
{
	assert(st);

	for (size_t i = 0; i < DRAWABLE_STACK_MAX; ++i) {
		struct drawable *dw = &st->objects[i];

		if (dw->draw)
			dw->draw(dw);
	}
}

void
drawable_stack_finish(struct drawable_stack *st)
{
	for (size_t i = 0; i < DRAWABLE_STACK_MAX; ++i) {
		struct drawable *dw = &st->objects[i];

		if (dw->finish)
			dw->finish(dw);
	}

	memset(st, 0, sizeof (*st));
}
