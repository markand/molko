/*
 * state.c -- abstract state
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

#include "state.h"

void
state_start(struct state *state)
{
	assert(state);

	if (state->start)
		state->start(state);
}

void
state_handle(struct state *state, const union event *ev)
{
	assert(state);
	assert(ev);

	if (state->handle)
		state->handle(state, ev);
}

void
state_update(struct state *state, unsigned int ticks)
{
	assert(state);

	if (state->update)
		state->update(state, ticks);
}

void
state_draw(struct state *state)
{
	assert(state);

	if (state->draw)
		state->draw(state);
}

void
state_suspend(struct state *state)
{
	assert(state);

	if (state->suspend)
		state->suspend(state);
}

void
state_resume(struct state *state)
{
	assert(state);

	if (state->resume)
		state->resume(state);
}

void
state_end(struct state *state)
{
	assert(state);

	if (state->end)
		state->end(state);
}

void
state_finish(struct state *state)
{
	assert(state);

	if (state->finish)
		state->finish(state);
}
