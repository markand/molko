/*
 * action.c -- action states
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
#include <string.h>

#include "action.h"

#define ACTION_FOREACH(st, iter) \
	for (size_t i = 0; i < ACTION_STACK_MAX && ((iter) = (st)->actions[i], 1); ++i)

void
action_handle(struct action *act, const union event *ev)
{
	assert(act);
	assert(ev);

	if (act->handle)
		act->handle(act, ev);
}

bool
action_update(struct action *act, unsigned int ticks)
{
	assert(act);

	if (act->update)
		return act->update(act, ticks);

	return false;
}

void
action_draw(struct action *act)
{
	assert(act);

	if (act->draw)
		act->draw(act);
}

void
action_end(struct action *act)
{
	assert(act);

	if (act->end)
		act->end(act);
}

void
action_finish(struct action *act)
{
	assert(act);

	if (act->finish)
		act->finish(act);
}

void
action_stack_init(struct action_stack *st)
{
	assert(st);

	memset(st, 0, sizeof (*st));
}

bool
action_stack_add(struct action_stack *st, struct action *act)
{
	assert(st);
	assert(act);

	for (size_t i = 0; i < ACTION_STACK_MAX; ++i) {
		if (!st->actions[i]) {
			st->actions[i] = act;
			return true;
		}
	}

	return false;
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

bool
action_stack_update(struct action_stack *st, unsigned int ticks)
{
	assert(st);

	struct action *act;

	for (size_t i = 0; i < ACTION_STACK_MAX; ++i) {
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

bool
action_stack_completed(const struct action_stack *st)
{
	assert(st);

	struct action *act;

	ACTION_FOREACH(st, act)
		if (act)
			return false;

	return true;
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
