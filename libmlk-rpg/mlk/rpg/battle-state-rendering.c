/*
 * battle-state-rendering.c -- battle state (rendering an action)
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

#include <mlk/core/alloc.h>
#include <mlk/core/drawable.h>

#include "battle-state-rendering.h"
#include "battle.h"

struct self {
	struct battle_state_rendering data;
	struct battle_state state;
};

static int
update(struct battle_state *st, struct battle *bt, unsigned int ticks)
{
	battle_state_rendering_update(st->data, bt, ticks);

	return 0;
}

static void
draw(const struct battle_state *st, const struct battle *bt)
{
	battle_state_rendering_draw(st->data, bt);
}

static void
finish(struct battle_state *st, struct battle *bt)
{
	(void)bt;

	battle_state_rendering_finish(st->data);
	mlk_alloc_free(st->data);
}

void
battle_state_rendering_init(struct battle_state_rendering *rdr, struct mlk_drawable *dw)
{
	assert(rdr);
	assert(dw);

	rdr->drawable = dw;
}

int
battle_state_rendering_update(struct battle_state_rendering *rdr, struct battle *bt, unsigned int ticks)
{
	assert(rdr);
	assert(battle_ok(bt));

	battle_update_component(bt, BATTLE_COMPONENT_ALL, ticks);

	if (mlk_drawable_update(rdr->drawable, ticks)) {
		mlk_drawable_end(rdr->drawable);
		return 1;
	}

	return 0;
}

void
battle_state_rendering_draw(const struct battle_state_rendering *rdr, const struct battle *bt)
{
	assert(rdr);

	battle_draw_component(bt, BATTLE_COMPONENT_ALL);
	mlk_drawable_draw(rdr->drawable);
}

void
battle_state_rendering_finish(struct battle_state_rendering *rdr)
{
	assert(rdr);

	mlk_drawable_finish(rdr->drawable);
}

void
battle_state_rendering(struct battle *bt, struct mlk_drawable *dw)
{
	assert(bt);
	assert(dw);

	struct self *self;

	self = mlk_alloc_new0(1, sizeof (*self));
	self->state.data = self;
	self->state.update = update;
	self->state.draw = draw;
	self->state.finish = finish;

	battle_state_rendering_init(&self->data, dw);
	battle_switch(bt, &self->state);
}
