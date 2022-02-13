/*
 * battle-state-opening.c -- battle state (opening)
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

#include <core/alloc.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/window.h>

#include "battle.h"
#include "battle-state.h"
#include "battle-state-check.h"
#include "battle-state-opening.h"

#define DELAY (1000U)

struct self {
	/* Always keep first. */
	struct battle_state_opening data;
	struct battle_state state;
};

static int
update(struct battle_state *st, struct battle *bt, unsigned int ticks)
{
	(void)bt;

	return battle_state_opening_update(st->data, bt, ticks);
}

static void
draw(const struct battle_state *st, const struct battle *bt)
{
	battle_state_opening_draw(st->data, bt);
}

static void
finish(struct battle_state *st, struct battle *bt)
{
	(void)bt;

	free(st->data);
}

int
battle_state_opening_update(struct battle_state_opening *op, struct battle *bt, unsigned int ticks)
{
	op->elapsed += ticks;

	/*
	 * Those function will effectively change state accordingly to the
	 * order of playing.
	 */
	if (op->elapsed >= DELAY)
		battle_state_check(bt);

	return 0;
}

void
battle_state_opening_draw(const struct battle_state_opening *op, const struct battle *bt)
{
	(void)bt;

	const unsigned int w = window.w;
	const unsigned int h = window.h / 2;
	const unsigned int ch = op->elapsed * h / DELAY;

	/* Draw some bezels opening. */
	painter_set_color(0x000000ff);
	painter_draw_rectangle(0, 0, w, h - ch);
	painter_draw_rectangle(0, h + ch, w, h - ch);
}

void
battle_state_opening(struct battle *bt)
{
	assert(bt);

	struct self *self;

	self = alloc_new0(sizeof (*self));
	self->state.data = self;
	self->state.update = update;
	self->state.draw = draw;
	self->state.finish = finish;

	battle_switch(bt, &self->state);
}
