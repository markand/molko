/*
 * drawable-stack.c -- convenient stack of drawables
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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
#include "drawable-stack.h"

#define DRAWABLE_FOREACH(st, iter) \
	for (size_t i = 0; i < DRAWABLE_STACK_MAX && ((iter) = (st)->objects[i], 1); ++i)

void
drawable_stack_init(struct drawable_stack *st)
{
	assert(st);

	memset(st, 0, sizeof (*st));
}

int
drawable_stack_add(struct drawable_stack *st, struct drawable *dw)
{
	assert(st);
	assert(dw);

	for (size_t i = 0; i < DRAWABLE_STACK_MAX; ++i) {
		if (!st->objects[i]) {
			st->objects[i] = dw;
			return 0;
		}
	}

	return -1;
}

int
drawable_stack_update(struct drawable_stack *st, unsigned int ticks)
{
	assert(st);

	for (size_t i = 0; i < DRAWABLE_STACK_MAX; ++i) {
		struct drawable *dw = st->objects[i];

		if (dw && drawable_update(dw, ticks)) {
			drawable_end(dw);
			drawable_finish(dw);
			st->objects[i] = NULL;
		}
	}

	/*
	 * We process the array again in case a drawable added a new drawable
	 * within the update function.
	 */
	return drawable_stack_completed(st);
}

void
drawable_stack_draw(struct drawable_stack *st)
{
	assert(st);

	struct drawable *dw;

	DRAWABLE_FOREACH(st, dw)
		if (dw)
			drawable_draw(dw);
}

int
drawable_stack_completed(const struct drawable_stack *st)
{
	assert(st);

	struct drawable *dw;

	DRAWABLE_FOREACH(st, dw)
		if (dw)
			return 0;

	return 1;
}

void
drawable_stack_finish(struct drawable_stack *st)
{
	assert(st);

	struct drawable *dw;

	DRAWABLE_FOREACH(st, dw) {
		if (dw) {
			drawable_end(dw);
			drawable_finish(dw);
		}
	}

	memset(st, 0, sizeof (*st));
}
