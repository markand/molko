/*
 * battle-state.c -- battle abstract state
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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

#include "battle-state.h"

void
battle_state_handle(struct battle_state *st, struct battle *bt, const union mlk_event *ev)
{
	assert(st);
	assert(bt);
	assert(ev);

	if (st->handle)
		st->handle(st, bt, ev);
}

int
battle_state_update(struct battle_state *st, struct battle *bt, unsigned int ticks)
{
	assert(st);
	assert(bt);

	if (st->update)
		return st->update(st, bt, ticks);

	return 0;
}

void
battle_state_draw(const struct battle_state *st, const struct battle *bt)
{
	assert(st);
	assert(bt);

	if (st->draw)
		st->draw(st, bt);
}

void
battle_state_finish(struct battle_state *st, struct battle *bt)
{
	assert(st);
	assert(bt);

	if (st->finish)
		st->finish(st, bt);
}
