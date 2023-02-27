/*
 * drawable-stack.c -- convenient stack of drawables
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
#include <string.h>

#include "drawable.h"
#include "drawable-stack.h"
#include "err.h"

#define DRAWABLE_FOREACH(st, iter) \
	for (size_t i = 0; i < (st)->objectsz && ((iter) = (st)->objects[i], 1); ++i)

void
mlk_drawable_stack_init(struct mlk_drawable_stack *st, struct mlk_drawable **objects, size_t objectsz)
{
	assert(st);

	st->objects = objects;
	st->objectsz = objectsz;

	for (size_t i = 0; i < st->objectsz; ++i)
		st->objects[i] = NULL;
}

int
mlk_drawable_stack_add(struct mlk_drawable_stack *st, struct mlk_drawable *dw)
{
	assert(st);
	assert(dw);

	for (size_t i = 0; i < st->objectsz; ++i) {
		if (!st->objects[i]) {
			st->objects[i] = dw;
			return 0;
		}
	}

	return MLK_ERR_NO_MEM;
}

int
mlk_drawable_stack_update(struct mlk_drawable_stack *st, unsigned int ticks)
{
	assert(st);

	struct mlk_drawable *dw;

	for (size_t i = 0; i < st->objectsz; ++i) {
		dw = st->objects[i];

		if (dw && mlk_drawable_update(dw, ticks)) {
			mlk_drawable_end(dw);
			mlk_drawable_finish(dw);
			st->objects[i] = NULL;
		}
	}

	/*
	 * We process the array again in case a drawable added a new drawable
	 * within the update function.
	 */
	return mlk_drawable_stack_completed(st);
}

void
mlk_drawable_stack_draw(struct mlk_drawable_stack *st)
{
	assert(st);

	struct mlk_drawable *dw;

	DRAWABLE_FOREACH(st, dw)
		if (dw)
			mlk_drawable_draw(dw);
}

int
mlk_drawable_stack_completed(const struct mlk_drawable_stack *st)
{
	assert(st);

	struct mlk_drawable *dw;

	DRAWABLE_FOREACH(st, dw)
		if (dw)
			return 0;

	return 1;
}

void
mlk_drawable_stack_finish(struct mlk_drawable_stack *st)
{
	assert(st);

	struct mlk_drawable *dw;

	DRAWABLE_FOREACH(st, dw) {
		if (dw) {
			mlk_drawable_end(dw);
			mlk_drawable_finish(dw);
		}
	}

	memset(st, 0, sizeof (*st));
}