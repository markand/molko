/*
 * battle-state-item.c -- battle state (using item)
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

#include <rpg/inventory.h>
#include <rpg/item.h>

#include "battle-entity-state.h"
#include "battle-message.h"
#include "battle-state.h"
#include "battle.h"

enum substate {
	SUBSTATE_ADVANCING,
	SUBSTATE_MESSAGE,
	SUBSTATE_RETURNING
};

struct self {
	enum substate substate;
	struct battle_message msg;
	struct battle_entity *source;
	struct battle_entity *target;
	struct battle_state state;
	struct inventory_slot *slot;
	int origin_x;
};

static int
update(struct battle_state *st, struct battle *bt, unsigned int ticks)
{
	struct self *self = st->data;

	switch (self->substate) {
	case SUBSTATE_ADVANCING:
		/* Entity is updating from battle, so just inspect its status. */
		if (battle_entity_update(self->source, 0)) {
			self->substate = SUBSTATE_MESSAGE;
			battle_entity_state_normal(self->source);
		}
		break;
	case SUBSTATE_MESSAGE:
		if (battle_message_update(&self->msg, ticks)) {
			self->substate = SUBSTATE_RETURNING;
			battle_entity_state_moving(self->source, self->origin_x, self->source->y);
		}
		break;
	default:
		if (battle_entity_update(self->source, 0)) {
			battle_entity_state_normal(self->source);
			battle_use(bt, self->slot->item, self->source->ch, self->target->ch);
		}
		break;
	}

	return 0;
}

static void
draw(const struct battle_state *st, const struct battle *bt)
{
	(void)bt;

	struct self *self = st->data;

	if (self->substate == SUBSTATE_MESSAGE)
		battle_message_draw(&self->msg);
}

static void
finish(struct battle_state *st, struct battle *bt)
{
	(void)bt;

	free(st->data);
}

void
battle_state_item(struct battle *bt,
                  struct character *source,
                  struct character *target,
                  struct inventory_slot *slot)
{
	assert(bt);
	assert(source);
	assert(target);
	assert(slot);

	struct self *self;

	if (!(self = alloc_new0(sizeof (*self))))
		panic();

	self->source = battle_find(bt, source);
	self->target = battle_find(bt, target);
	self->slot = slot;
	self->origin_x = self->source->x;

	self->msg.text = slot->item->name;
	self->msg.theme = bt->theme;

	self->state.data = self;
	self->state.update = update;
	self->state.draw = draw;
	self->state.finish = finish;

	battle_entity_state_moving(self->source, self->origin_x - 100, self->source->y);
	battle_switch(bt, &self->state);
}
