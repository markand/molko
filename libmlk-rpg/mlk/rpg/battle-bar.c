/*
 * battle-bar.c -- abstract battle bar
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

#include "battle-bar.h"

void
battle_bar_start(struct battle_bar *bar, struct battle *bt)
{
	assert(bar);
	assert(bt);

	if (bar->start)
		bar->start(bar, bt);
}

void
battle_bar_select(struct battle_bar *bar, struct battle *bt, const struct selection *sel)
{
	assert(bar);
	assert(bt);
	assert(sel);

	if (bar->select)
		bar->select(bar, bt, sel);

}

void
battle_bar_handle(struct battle_bar *bar, struct battle *bt, const union mlk_event *ev)
{
	assert(bar);
	assert(bt);
	assert(ev);

	if (bar->handle)
		bar->handle(bar, bt, ev);
}

void
battle_bar_update(struct battle_bar *bar, struct battle *bt, unsigned int ticks)
{
	assert(bar);
	assert(bt);

	if (bar->update)
		bar->update(bar, bt, ticks);
}

void
battle_bar_draw(const struct battle_bar *bar, const struct battle *bt)
{
	assert(bar);
	assert(bt);

	if (bar->draw)
		bar->draw(bar, bt);
}

void
battle_bar_finish(struct battle_bar *bar, struct battle *bt)
{
	assert(bar);
	assert(bt);

	if (bar->finish)
		bar->finish(bar, bt);
}
