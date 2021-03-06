/*
 * action.c -- action states
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

#include "action.h"

void
action_handle(struct action *act, const union event *ev)
{
	assert(act);
	assert(ev);

	if (act->handle)
		act->handle(act, ev);
}

int
action_update(struct action *act, unsigned int ticks)
{
	assert(act);

	if (act->update)
		return act->update(act, ticks);

	/* No function means immortal action. */
	return 0;
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
