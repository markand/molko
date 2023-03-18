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

#include <assert.h>
#include <string.h>

#include "action-stack.h"
#include "action.h"
#include "core_p.h"
#include "err.h"

#define FOREACH(st, iter) \
	for (size_t i = 0; i < (st)->actionsz && ((iter) = (st)->actions[i], 1); ++i)

void
mlk_action_stack_init(struct mlk_action_stack *st)
{
	assert(st);

	for (size_t i = 0; i < st->actionsz; ++i)
		st->actions[i] = NULL;
}

int
mlk_action_stack_add(struct mlk_action_stack *st, struct mlk_action *act)
{
	assert(st);
	assert(act);

	for (size_t i = 0; i < st->actionsz; ++i) {
		if (!st->actions[i]) {
			st->actions[i] = act;
			return 0;
		}
	}

	return mlk_errf(_("no space in action stack"));
}

int
mlk_action_stack_completed(const struct mlk_action_stack *st)
{
	assert(st);

	struct mlk_action *act;

	FOREACH(st, act)
		if (act)
			return 0;

	return 1;
}

void
mlk_action_stack_start(struct mlk_action_stack *st)
{
	assert(st);

	struct mlk_action *act;

	FOREACH(st, act)
		if (act)
			mlk_action_start(act);
}

void
mlk_action_stack_handle(struct mlk_action_stack *st, const union mlk_event *ev)
{
	assert(st);
	assert(ev);

	struct mlk_action *act;

	FOREACH(st, act)
		if (act)
			mlk_action_handle(act, ev);
}

int
mlk_action_stack_update(struct mlk_action_stack *st, unsigned int ticks)
{
	assert(st);

	struct mlk_action *act;

	for (size_t i = 0; i < st->actionsz; ++i) {
		act = st->actions[i];

		if (act && mlk_action_update(act, ticks)) {
			mlk_action_end(act);
			mlk_action_finish(act);
			st->actions[i] = NULL;
		}
	}

	/*
	 * We process all actions again in case the user modified the stack
	 * within their update function.
	 */
	return mlk_action_stack_completed(st);
}

void
mlk_action_stack_draw(const struct mlk_action_stack *st)
{
	assert(st);

	struct mlk_action *act;

	FOREACH(st, act)
		if (act)
			mlk_action_draw(act);
}

void
mlk_action_stack_finish(struct mlk_action_stack *st)
{
	assert(st);

	struct mlk_action *act;

	FOREACH(st, act) {
		if (act) {
			mlk_action_end(act);
			mlk_action_finish(act);
		}
	}

	for (size_t i = 0; i < st->actionsz; ++i)
		st->actions[i] = NULL;
}
