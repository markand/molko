/*
 * action.h -- action states
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

#ifndef MOLKO_CORE_ACTION_H
#define MOLKO_CORE_ACTION_H

#include <stdbool.h>

#include "core.h"

#define ACTION_STACK_MAX (128)

union event;

struct action {
	void *data;
	void (*handle)(struct action *act, const union event *ev);
	bool (*update)(struct action *act, unsigned int ticks);
	void (*draw)(struct action *act);
	void (*end)(struct action *act);
	void (*finish)(struct action *act);
};

struct action_stack {
	struct action *actions[ACTION_STACK_MAX];
};

CORE_BEGIN_DECLS

void
action_handle(struct action *act, const union event *ev);

bool
action_update(struct action *act, unsigned int ticks);

void
action_draw(struct action *act);

void
action_end(struct action *act);

void
action_finish(struct action *act);

void
action_stack_init(struct action_stack *st);

bool
action_stack_add(struct action_stack *st, struct action *act);

void
action_stack_handle(struct action_stack *st, const union event *ev);

bool
action_stack_update(struct action_stack *st, unsigned int ticks);

void
action_stack_draw(const struct action_stack *st);

bool
action_stack_completed(const struct action_stack *st);

void
action_stack_finish(struct action_stack *st);

CORE_END_DECLS

#endif /* !MOLKO_CORE_ACTION_H */
