/*
 * battle-entity-state-moving.c -- the entity is moving
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
#include <math.h>
#include <stdlib.h>

#include <core/alloc.h>
#include <core/panic.h>

#include "battle-entity.h"
#include "battle-entity-state.h"
#include "battle-entity-state-moving.h"
#include "character.h"
#include "walksprite.h"

#define SPEED 800
#define SEC   1000
#define WALK  40

struct position {
	struct battle_entity_state state;
	struct walksprite ws;

	/* Destination. */
	int x;
	int y;
};

static inline unsigned int
orientation(const struct position *pos, const struct battle_entity *et)
{
	/* TODO: support diagonal. */
	/* See: walksprite definitions. */
	return pos->x < et->x ? 6 : 2;
}

static bool
update(struct battle_entity_state *st, struct battle_entity *et, unsigned int ticks)
{
	struct position *pos = st->data;
	int step_x, step_y, delta_x, delta_y;

	delta_x = pos->x < et->x ? -1 : +1;
	delta_y = pos->y < et->y ? -1 : +1;
	step_x = fmin(SPEED * ticks / SEC, abs(et->x - pos->x));
	step_y = fmin(SPEED * ticks / SEC, abs(et->y - pos->y));

	et->x += delta_x * step_x;
	et->y += delta_y * step_y;

	if (et->x != pos->x || et->y != pos->y)
		walksprite_update(&pos->ws, ticks);
	else
		walksprite_reset(&pos->ws);

	return et->x == pos->x && et->y == pos->y;
}

static void
draw(const struct battle_entity_state *st, const struct battle_entity *et)
{
	/* TODO: compute orientation. */
	struct position *pos = st->data;

	walksprite_draw(&pos->ws, orientation(pos, et), et->x, et->y);
}

static void
finish(struct battle_entity_state *st, struct battle_entity *et)
{
	(void)et;

	free(st->data);
}

void
battle_entity_state_moving(struct battle_entity *et, int destx, int desty)
{
	assert(et);

	struct position *pos;

	if (!(pos = alloc_new0(sizeof (*pos))))
		panic();

	walksprite_init(&pos->ws, et->ch->sprites[CHARACTER_SPRITE_NORMAL], 40);
	pos->x = destx;
	pos->y = desty;

	pos->state.data = pos;
	pos->state.update = update;
	pos->state.draw = draw;
	pos->state.finish = finish;

	battle_entity_switch(et, &pos->state);
}
