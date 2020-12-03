/*
 * battle-state-opening.c -- battle state (opening)
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
#include <stdbool.h>
#include <stdlib.h>

#include <core/alloc.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/window.h>

#include "battle.h"
#include "battle-state.h"

#define DELAY (1000U)

struct opening {
	struct battle_state self;
	unsigned int elapsed;
};

static bool
update(struct battle_state *st, struct battle *bt, unsigned int ticks)
{
	assert(bt);

	struct opening *opening = st->data;

	opening->elapsed += ticks;

	/*
	 * Those function will effectively change state accordingly to the
	 * order of playing.
	 */
	if (opening->elapsed >= DELAY)
		battle_state_check(bt);

	return false;
}

static void
draw(const struct battle_state *st, const struct battle *bt)
{
	(void)bt;

	assert(bt);

	const struct opening *opening = st->data;
	const unsigned int w = window.w;
	const unsigned int h = window.h / 2;
	const unsigned int ch = opening->elapsed * h / DELAY;

	/* Draw some bezels opening. */
	painter_set_color(0x000000ff);
	painter_draw_rectangle(0, 0, w, h - ch);
	painter_draw_rectangle(0, h + ch, w, h - ch);
}

static void
finish(struct battle_state *st, struct battle *bt)
{
	(void)bt;

	free(st->data);
}

void
battle_state_opening(struct battle *bt)
{
	assert(bt);

	struct opening *opening;

	if (!(opening = alloc_new0(sizeof (*opening))))
		panic();

	opening->self.data = opening;
	opening->self.update = update;
	opening->self.draw = draw;
	opening->self.finish = finish;

	battle_switch(bt, &opening->self);
}
