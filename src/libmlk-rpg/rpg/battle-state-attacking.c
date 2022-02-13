/*
 * battle-state-attacking.c -- battle state (entity is moving for attacking)
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
#include <core/panic.h>
#include <core/sprite.h>

#include "battle-entity-state-attacking.h"
#include "battle-entity-state-blinking.h"
#include "battle-entity-state-moving.h"
#include "battle-entity-state-normal.h"
#include "battle-entity-state.h"
#include "battle-state-attacking.h"
#include "battle-state-check.h"
#include "battle-state.h"
#include "battle.h"
#include "character.h"

struct self {
	struct battle_state_attacking data;
	struct battle_state state;
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

static int
update(struct battle_state *st, struct battle *bt, unsigned int ticks)
{
	(void)ticks;

	return battle_state_attacking_update(st->data, bt);
}

static void
finish(struct battle_state *st, struct battle *bt)
{
	(void)bt;

	free(st->data);
}

void
battle_state_attacking_init(struct battle_state_attacking *atk,
                            struct battle_entity *source,
                            struct battle_entity *target)
{
	assert(atk);
	assert(battle_entity_ok(source));
	assert(battle_entity_ok(target));

	int x, y;

	/* Starts this state with advancing. */
	atk->source = source;
	atk->target = target;
	atk->origin_x = source->x;
	atk->origin_y = source->y;

	/* We go to the enemy. */
	x = target->x + target->ch->sprites[CHARACTER_SPRITE_NORMAL]->cellw;
	y = target->y;

	/* If it is an enemy we don't move it but blink instead. */
	if (source->ch->exec) {
		atk->status = BATTLE_STATE_ATTACKING_BLINKING;
		battle_entity_state_blinking(source);
	} else
		battle_entity_state_moving(source, x, y);
}

int
battle_state_attacking_update(struct battle_state_attacking *atk, struct battle *bt)
{
	assert(atk);
	assert(bt);

	if (!battle_entity_update(atk->source, 0))
		return 0;

	switch (atk->status) {
	case BATTLE_STATE_ATTACKING_ADVANCING:
		/*
		 * Current entity state is battle-entity-state-moving but it is
		 * already updated from the game itself so pass 0 just to check
		 * if it has finished moving.
		 */
		atk->status = BATTLE_STATE_ATTACKING_DAMAGING;

		/* TODO: determine sprite to use about equipment. */
		battle_entity_state_attacking(atk->source, atk->source->ch->sprites[CHARACTER_SPRITE_SWORD]);
		break;
	case BATTLE_STATE_ATTACKING_DAMAGING:
		/* Move back to original position. */
		atk->status = BATTLE_STATE_ATTACKING_RETURNING;
		damage(atk->source, atk->target, bt);
		battle_entity_state_moving(atk->source, atk->origin_x, atk->origin_y);
		break;
	case BATTLE_STATE_ATTACKING_RETURNING:
	case BATTLE_STATE_ATTACKING_BLINKING:
		/* Just wait. */
		battle_entity_state_normal(atk->source);
		damage(atk->source, atk->target, bt);
		battle_state_check(bt);
		break;
	default:
		break;
	}

	return 0;
}

void
battle_state_attacking(struct battle_entity *source, struct battle_entity *target, struct battle *bt)
{
	assert(battle_entity_ok(source));
	assert(battle_entity_ok(target));
	assert(bt);

	struct self *self;

	self = alloc_new0(sizeof (*self));
	self->state.data = self;
	self->state.update = update;
	self->state.finish = finish;

	battle_state_attacking_init(&self->data, source, target);
	battle_switch(bt, &self->state);
}
