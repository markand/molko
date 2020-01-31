/*
 * script.c -- convenient sequence of actions
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
#include <stdlib.h>
#include <string.h>

#include "script.h"
#include "util.h"

static void
script_action_finish(struct action *a)
{
	assert(a);

	script_finish(a->data);
	free(a->data);
}

static void
script_action_handle(struct action *a, const union event *ev)
{
	assert(a);
	assert(ev);

	script_handle(a->data, ev);
}

static bool
script_action_update(struct action *a, unsigned int ticks)
{
	assert(a);

	return script_update(a->data, ticks);
}

static void
script_action_draw(struct action *a)
{
	assert(a);

	script_draw(a->data);
}

void
script_init(struct script *s)
{
	assert(s);

	memset(s, 0, sizeof (struct script));
	s->tail = &s->head;
}

void
script_start(struct script *s)
{
	assert(s);

	s->iter = s->head;
}

void
script_append(struct script *s, const struct action *a)
{
	assert(s);
	assert(a);
	assert(a->update);

	struct script_action *iter = ecalloc(1, sizeof (struct script_action));

	memcpy(&iter->action, a, sizeof (struct action));
	*s->tail = iter;
	s->tail = &iter->next;
}

void
script_handle(struct script *s, const union event *ev)
{
	assert(s);
	assert(ev);

	if (s->iter && s->iter->action.handle)
		s->iter->action.handle(&s->iter->action, ev);
}

bool
script_update(struct script *s, unsigned int ticks)
{
	assert(s);

	if (!s->iter)
		return true;

	struct action *a = &s->iter->action;

	if (a->update(a, ticks)) {
		if (a->end)
			a->end(a);
		if (a->finish)
			a->finish(a);

		s->iter = s->iter->next;
	}

	return s->iter == NULL;
}

void
script_draw(struct script *s)
{
	assert(s);

	if (s->iter && s->iter->action.draw)
		s->iter->action.draw(&s->iter->action);
}

void
script_action(const struct script *s, struct action *action)
{
	assert(s);
	assert(action);

	memset(action, 0, sizeof (struct action));
	action->data = ememdup(s, sizeof (struct script));
	action->handle = script_action_handle;
	action->update = script_action_update;
	action->draw = script_action_draw;
	action->finish = script_action_finish;
}

void
script_finish(struct script *s)
{
	assert(s);

	struct script_action *iter, *next;

	for (iter = s->head; iter; iter = next) {
		next = iter->next;
		free(iter);
	}

	memset(s, 0, sizeof (struct script));
}
