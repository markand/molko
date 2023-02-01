/*
 * action-stack.h -- convenient stack of actions
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

#ifndef MLK_CORE_ACTION_STACK_H
#define MLK_CORE_ACTION_STACK_H

#include <stddef.h>

#include "core.h"

struct mlk_action;

union event;

struct mlk_action_stack {
	struct mlk_action **actions;
	size_t actionsz;
};

CORE_BEGIN_DECLS

void
mlk_action_stack_init(struct mlk_action_stack *, struct mlk_action **, size_t);

int
mlk_action_stack_add(struct mlk_action_stack *, struct mlk_action *);

void
mlk_action_stack_handle(struct mlk_action_stack *, const union event *);

int
mlk_action_stack_update(struct mlk_action_stack *, unsigned int);

void
mlk_action_stack_draw(const struct mlk_action_stack *);

int
mlk_action_stack_completed(const struct mlk_action_stack *);

void
mlk_action_stack_finish(struct mlk_action_stack *);

CORE_END_DECLS

#endif /* !MLK_CORE_ACTION_STACK_H */
