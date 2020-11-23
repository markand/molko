/*
 * battle-state-attacking.h -- battle state (entity is moving for attacking)
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

#include <core/alloc.h>
#include <core/panic.h>
#include <core/sprite.h>

#include "battle-state-check.h"
#include "battle-entity-state-attacking.h"
#include "battle-entity-state-blinking.h"
#include "battle-entity-state-moving.h"
#include "battle-entity-state-normal.h"
#include "battle-state-attacking.h"
#include "battle.h"
#include "battle-state.h"
#include "character.h"

enum substate {
	/* For team. */
	SUBSTATE_ADVANCING,
	SUBSTATE_ATTACKING,
	SUBSTATE_RETURNING,

	/* For enemies. */
	SUBSTATE_BLINKING,
};

/*
 * This state is split into three parts:
 *
 * 1. entity walks a bit in front of its original position.
 * 2. entity animate itself while attacking.
 * 3. entity goes back to its original position.
 */
struct data {
	enum substate substate;
	struct battle_entity *source;
	struct battle_entity *target;
	struct battle_state state;
	int origin_x;
	int origin_y;
};

static void
damage(const struct battle_entity *source, struct battle_entity *target, struct battle *bt)
{
	(void)source;

	/* TODO: math calculation here.*/
	target->ch->hp -= 50;

	if (target->ch->hp < 0)
		target->ch->hp = 0;

	battle_indicator_hp(bt, target->ch, 50);
}

static bool
update(struct battle_state *st, struct battle *bt, unsigned int ticks)
{
	(void)bt;
	(void)ticks;

	struct data *data = st->data;

	if (!battle_entity_update(data->source, 0))
		return false;

	switch (data->substate) {
	case SUBSTATE_ADVANCING:
		/*
		 * Current entity state is battle-entity-state-moving but it is
		 * already updated from the game itself so pass 0 just to check
		 * if it has finished moving.
		 */
		data->substate = SUBSTATE_ATTACKING;
		/* TODO: determine sprite to use about equipment. */
		battle_entity_state_attacking(data->source, data->source->ch->sprites[CHARACTER_SPRITE_SWORD]);
		break;
	case SUBSTATE_ATTACKING:
		/* Move back to original position. */
		data->substate = SUBSTATE_RETURNING;
		damage(data->source, data->target, bt);
		battle_entity_state_moving(data->source, data->origin_x, data->origin_y);
		break;
	case SUBSTATE_RETURNING:
	case SUBSTATE_BLINKING:
		/* Just wait. */
		battle_entity_state_normal(data->source);
		battle_state_check(bt);
		break;
	default:
		break;
	}

	return false;
}

static void
finish(struct battle_state *st, struct battle *bt)
{
	(void)bt;

	free(st->data);
}

void
battle_state_attacking(struct battle *bt, struct character *source, struct character *target)
{
	assert(bt);

	struct data *data;
	int x, y;

	if (!(data = alloc_new0(sizeof (*data))))
		panic();

	/* Starts this state with advancing. */
	data->source = battle_find(bt, source);
	data->target = battle_find(bt, target);
	data->origin_x = data->source->x;
	data->origin_y = data->source->y;

	/* We go to the enemy. */
	x = data->target->x + data->target->ch->sprites[CHARACTER_SPRITE_WALK]->cellw;
	y = data->target->y;

	/* If it is an enemy we don't move it but blink instead. */
	if (data->source->ch->exec) {
		data->substate = SUBSTATE_BLINKING;
		battle_entity_state_blinking(data->source);
	} else
		battle_entity_state_moving(data->source, x, y);

	data->state.data = data;
	data->state.update = update;
	data->state.finish = finish;

	battle_switch(bt, &data->state);
}
