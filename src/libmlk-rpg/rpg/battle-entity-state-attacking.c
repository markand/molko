/*
 * battle-entity-state-attacking.h -- the entity is attacking
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
#include <core/animation.h>
#include <core/panic.h>
#include <core/sprite.h>

#include "battle-entity-state-attacking.h"
#include "battle-entity-state.h"
#include "battle-entity.h"

struct self {
	struct battle_entity_state_attacking data;
	struct battle_entity_state state;
};

static int
update(struct battle_entity_state *st, struct battle_entity *et, unsigned int ticks)
{
	(void)et;

	return battle_entity_state_attacking_update(st->data, ticks);
}

static void
draw(const struct battle_entity_state *st, const struct battle_entity *et)
{
	battle_entity_state_attacking_draw(st->data, et);
}

static void
finish(struct battle_entity_state *st, struct battle_entity *et)
{
	(void)et;

	free(st->data);
}

void
battle_entity_state_attacking_init(struct battle_entity_state_attacking *atk, const struct sprite *which)
{
	assert(atk);
	assert(sprite_ok(which));

	animation_init(&atk->anim, which, 100);
	animation_start(&atk->anim);
}

int
battle_entity_state_attacking_update(struct battle_entity_state_attacking *atk, unsigned int ticks)
{
	assert(atk);

	return animation_update(&atk->anim, ticks);
}

void
battle_entity_state_attacking_draw(const struct battle_entity_state_attacking *atk, const struct battle_entity *et)
{
	assert(atk);
	assert(battle_entity_ok(et));

	animation_draw(&atk->anim, et->x, et->y);
}

void
battle_entity_state_attacking(struct battle_entity *et, const struct sprite *which)
{
	assert(battle_entity_ok(et));
	assert(sprite_ok(which));

	struct self *self;

	self = alloc_new0(sizeof (*self));
	self->state.data = self;
	self->state.update = update;
	self->state.draw = draw;
	self->state.finish = finish;

	battle_entity_state_attacking_init(&self->data, which);
	battle_entity_switch(et, &self->state);
}
