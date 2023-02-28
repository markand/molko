/*
 * script.c -- convenient sequence of actions
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
#include <stdlib.h>
#include <string.h>

#include "action-script.h"
#include "action.h"
#include "err.h"

static inline struct mlk_action *
current(struct mlk_action_script *s)
{
	if (s->cur >= s->len)
		return NULL;

	return s->actions[s->cur];
}

void
mlk_action_script_init(struct mlk_action_script *s, struct mlk_action **actions, size_t cap)
{
	assert(s);

	s->actions = actions;
	s->cap = cap;
	s->len = s->cur = 0;

	for (size_t i = 0; i < s->cap; ++i)
		s->actions[i] = NULL;
}

void
mlk_action_script_start(struct mlk_action_script *s)
{
	assert(s);

	struct mlk_action *a;

	if ((a = current(s)))
		mlk_action_start(a);
}

int
mlk_action_script_append(struct mlk_action_script *s, struct mlk_action *a)
{
	assert(s);
	assert(a);

	if (s->len >= s->cap)
		return MLK_ERR_NO_MEM;

	s->actions[s->len++] = a;

	return 0;
}

void
mlk_action_script_handle(struct mlk_action_script *s, const union mlk_event *ev)
{
	assert(s);
	assert(ev);

	struct mlk_action *a;

	if ((a = current(s)))
		mlk_action_handle(a, ev);
}

int
mlk_action_script_update(struct mlk_action_script *s, unsigned int ticks)
{
	assert(s);

	struct mlk_action *a;

	/* No action left means completed. */
	if (!(a = current(s)))
		return 1;

	if (mlk_action_update(a, ticks)) {
		mlk_action_end(a);
		s->cur++;

		/* Start this action now. */
		if ((a = current(s)))
			mlk_action_start(a);
	}

	return s->cur >= s->len;
}

void
mlk_action_script_draw(struct mlk_action_script *s)
{
	assert(s);

	struct mlk_action *a;

	if ((a = current(s)))
		mlk_action_draw(a);
}

int
mlk_action_script_completed(const struct mlk_action_script *s)
{
	assert(s);

	return s->cur >= s->len;
}

void
mlk_action_script_finish(struct mlk_action_script *s)
{
	assert(s);

	for (size_t i = 0; i < s->len; ++i)
		mlk_action_finish(s->actions[i]);

	memset(s, 0, sizeof (*s));
}
