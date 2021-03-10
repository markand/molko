/*
 * drawable.h -- automatic drawable objects
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

#ifndef MOLKO_CORE_DRAWABLE_H
#define MOLKO_CORE_DRAWABLE_H

#include "core.h"

#define DRAWABLE_STACK_MAX (128)

struct drawable {
	void *data;
	int x;
	int y;
	int (*update)(struct drawable *, unsigned int);
	void (*draw)(struct drawable *);
	void (*end)(struct drawable *);
	void (*finish)(struct drawable *);
};

struct drawable_stack {
	struct drawable *objects[DRAWABLE_STACK_MAX];
};

CORE_BEGIN_DECLS

int
drawable_update(struct drawable *, unsigned int);

void
drawable_draw(struct drawable *);

void
drawable_end(struct drawable *);

void
drawable_finish(struct drawable *);

void
drawable_stack_init(struct drawable_stack *);

int
drawable_stack_add(struct drawable_stack *, struct drawable *);

int
drawable_stack_update(struct drawable_stack *, unsigned int);

void
drawable_stack_draw(struct drawable_stack *);

int
drawable_stack_completed(const struct drawable_stack *);

void
drawable_stack_finish(struct drawable_stack *);

CORE_END_DECLS

#endif /* !MOLKO_CORE_DRAWABLE_H */
