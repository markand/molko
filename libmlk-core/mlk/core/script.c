/*
 * script.c -- convenient sequence of actions
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
#include <stdlib.h>
#include <string.h>

#include "action.h"
#include "err.h"
#include "script.h"

static struct action *
current(struct script *s)
{
	if (s->cur >= s->actionsz)
		return NULL;

	return s->actions[s->cur];
}

static void
handle(struct action *a, const union event *ev)
{
	script_handle(a->data, ev);
}

static int
update(struct action *a, unsigned int ticks)
{
	return script_update(a->data, ticks);
}

static void
draw(struct action *a)
{
	script_draw(a->data);
}

static void
finish(struct action *a)
{
	script_finish(a->data);
}

void
script_init(struct script *s)
{
	assert(s);

	memset(s, 0, sizeof (*s));
}

int
script_append(struct script *s, struct action *a)
{
	assert(s);
	assert(a);

	if (s->actionsz >= SCRIPT_ACTION_MAX)
		return ERR_NO_MEM;

	s->actions[s->actionsz++] = a;

	return 0;
}

void
script_handle(struct script *s, const union event *ev)
{
	assert(s);
	assert(ev);

	struct action *a = current(s);

	if (a)
		action_handle(a, ev);
}

int
script_update(struct script *s, unsigned int ticks)
{
	assert(s);

	struct action *a = current(s);

	if (!a)
		return 1;

	if (action_update(a, ticks)) {
		action_end(a);
		s->cur++;
	}

	return s->cur >= s->actionsz;
}

void
script_draw(struct script *s)
{
	assert(s);

	struct action *a = current(s);

	if (a)
		action_draw(a);
}

int
script_completed(const struct script *s)
{
	assert(s);

	return s->cur >= s->actionsz;
}

void
script_finish(struct script *s)
{
	assert(s);

	for (size_t i = 0; i < s->actionsz; ++i)
		action_finish(s->actions[i]);

	memset(s, 0, sizeof (*s));
}

void
script_action(struct script *s, struct action *action)
{
	assert(s);
	assert(action);

	memset(action, 0, sizeof (*action));
	action->data = s;
	action->handle = handle;
	action->update = update;
	action->draw = draw;
	action->finish = finish;
}
