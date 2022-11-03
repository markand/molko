/*
 * battle-entity-state-moving.c -- the entity is moving
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
#include <math.h>
#include <stdlib.h>

#include <mlk/core/alloc.h>
#include <mlk/core/panic.h>

#include "battle-entity-state-moving.h"
#include "battle-entity-state.h"
#include "battle-entity.h"
#include "character.h"
#include "walksprite.h"

#define SPEED 800
#define SEC   1000
#define WALK  40

struct self {
	struct battle_entity_state_moving data;
	struct battle_entity_state state;
};

static inline unsigned int
orientation(const struct battle_entity_state_moving *mv, const struct battle_entity *et)
{
	/* TODO: support diagonal. */
	/* See: walksprite definitions. */
	return mv->x < et->x ? 6 : 2;
}

static int
update(struct battle_entity_state *st, struct battle_entity *et, unsigned int ticks)
{
	return battle_entity_state_moving_update(st->data, et, ticks);
}

static void
draw(const struct battle_entity_state *st, const struct battle_entity *et)
{
	battle_entity_state_moving_draw(st->data, et);
}

static void
finish(struct battle_entity_state *st, struct battle_entity *et)
{
	(void)et;

	free(st->data);
}

void
battle_entity_state_moving_init(struct battle_entity_state_moving *mv, struct battle_entity *et, int dstx, int dsty)
{
	assert(mv);
	assert(battle_entity_ok(et));

	walksprite_init(&mv->ws, et->ch->sprites[CHARACTER_SPRITE_NORMAL], 40);
	mv->x = dstx;
	mv->y = dsty;
}

int
battle_entity_state_moving_update(struct battle_entity_state_moving *mv, struct battle_entity *et, unsigned int ticks)
{
	assert(mv);
	assert(battle_entity_ok(et));

	int step_x, step_y, delta_x, delta_y;

	delta_x = mv->x < et->x ? -1 : +1;
	delta_y = mv->y < et->y ? -1 : +1;
	step_x = fmin(SPEED * ticks / SEC, abs(et->x - mv->x));
	step_y = fmin(SPEED * ticks / SEC, abs(et->y - mv->y));

	et->x += delta_x * step_x;
	et->y += delta_y * step_y;

	if (et->x != mv->x || et->y != mv->y)
		walksprite_update(&mv->ws, ticks);
	else
		walksprite_reset(&mv->ws);

	return et->x == mv->x && et->y == mv->y;
}

void
battle_entity_state_moving_draw(const struct battle_entity_state_moving *mv, const struct battle_entity *et)
{
	assert(mv);
	assert(battle_entity_ok(et));

	/* TODO: compute orientation. */
	walksprite_draw(&mv->ws, orientation(mv, et), et->x, et->y);
}

void
battle_entity_state_moving(struct battle_entity *et, int dstx, int dsty)
{
	assert(battle_entity_ok(et));

	struct self *self;

	self = mlk_alloc_new0(1, sizeof (*self));
	self->state.data = self;
	self->state.update = update;
	self->state.draw = draw;
	self->state.finish = finish;

	battle_entity_state_moving_init(&self->data, et, dstx, dsty);
	battle_entity_switch(et, &self->state);
}
