/*
 * action-stack.h -- convenient stack of actions
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

#include "action-stack.h"
#include "action.h"
#include "err.h"

#define ACTION_FOREACH(st, iter) \
	for (size_t i = 0; i < (st)->actionsz && ((iter) = (st)->actions[i], 1); ++i)

void
action_stack_init(struct action_stack *st, struct action **actions, size_t actionsz)
{
	assert(st);

	st->actions = actions;
	st->actionsz = actionsz;

	for (size_t i = 0; i < st->actionsz; ++i)
		st->actions[i] = NULL;
}

int
action_stack_add(struct action_stack *st, struct action *act)
{
	assert(st);
	assert(act);

	for (size_t i = 0; i < st->actionsz; ++i) {
		if (!st->actions[i]) {
			st->actions[i] = act;
			return 0;
		}
	}

	return ERR_NO_MEM;
}

void
action_stack_handle(struct action_stack *st, const union event *ev)
{
	assert(st);
	assert(ev);

	struct action *act;

	ACTION_FOREACH(st, act)
		if (act)
			action_handle(act, ev);
}

int
action_stack_update(struct action_stack *st, unsigned int ticks)
{
	assert(st);

	struct action *act;

	for (size_t i = 0; i < st->actionsz; ++i) {
		act = st->actions[i];

		if (act && action_update(act, ticks)) {
			action_end(act);
			action_finish(act);
			st->actions[i] = NULL;
		}
	}

	/*
	 * We process all actions again in case the user modified the stack
	 * within their update function.
	 */
	return action_stack_completed(st);
}

void
action_stack_draw(const struct action_stack *st)
{
	assert(st);

	struct action *act;

	ACTION_FOREACH(st, act)
		if (act)
			action_draw(act);
}

int
action_stack_completed(const struct action_stack *st)
{
	assert(st);

	struct action *act;

	ACTION_FOREACH(st, act)
		if (act)
			return 0;

	return 1;
}

void
action_stack_finish(struct action_stack *st)
{
	assert(st);

	struct action *act;

	ACTION_FOREACH(st, act) {
		if (act) {
			action_end(act);
			action_finish(act);
		}
	}

	memset(st, 0, sizeof (*st));
}
