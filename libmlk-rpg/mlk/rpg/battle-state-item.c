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

#include <mlk/core/alloc.h>
#include <mlk/core/panic.h>

#include "battle-entity-state-moving.h"
#include "battle-entity-state-normal.h"
#include "battle-entity-state.h"
#include "battle-message.h"
#include "battle-state-item.h"
#include "battle-state.h"
#include "battle.h"
#include "inventory.h"
#include "item.h"

struct self {
	struct battle_state_item data;
	struct battle_state state;
};

static int
update(struct battle_state *st, struct battle *bt, unsigned int ticks)
{
	return battle_state_item_update(st->data, bt, ticks);
}

static void
draw(const struct battle_state *st, const struct battle *bt)
{
	(void)bt;

	battle_state_item_draw(st->data);
}

static void
finish(struct battle_state *st, struct battle *bt)
{
	(void)bt;

	free(st->data);
}

void
battle_state_item_init(struct battle_state_item *it,
                       struct battle *bt,
                       struct battle_entity *source,
                       struct battle_entity *target,
                       struct inventory_slot *slot)
{
	assert(it);
	assert(bt);
	assert(battle_entity_ok(source));
	assert(battle_entity_ok(target));
	assert(slot);

	it->source = source;
	it->target = target;
	it->slot = slot;
	it->origin_x = it->source->x;

	it->msg.text = slot->item->name;
	it->msg.theme = bt->theme;

	battle_entity_state_moving(it->source, it->origin_x - 100, it->source->y);
}

int
battle_state_item_update(struct battle_state_item *it, struct battle *bt, unsigned int ticks)
{
	assert(it);
	assert(bt);

	switch (it->status) {
	case BATTLE_STATE_ITEM_ADVANCING:
		/* Entity is updating from battle, so just inspect its status. */
		if (battle_entity_update(it->source, 0)) {
			it->status = BATTLE_STATE_ITEM_MESSAGE;
			battle_entity_state_normal(it->source);
		}
		break;
	case BATTLE_STATE_ITEM_MESSAGE:
		if (battle_message_update(&it->msg, ticks)) {
			it->status = BATTLE_STATE_ITEM_RETURNING;
			battle_entity_state_moving(it->source, it->origin_x, it->source->y);
		}
		break;
	default:
		if (battle_entity_update(it->source, 0)) {
			battle_entity_state_normal(it->source);
			battle_use(bt, it->slot->item, it->source->ch, it->target->ch);
		}
		break;
	}

	return 0;
}

void
battle_state_item_draw(struct battle_state_item *it)
{
	assert(it);

	if (it->status == BATTLE_STATE_ITEM_MESSAGE)
		battle_message_draw(&it->msg);
}

void
battle_state_item(struct battle *bt,
                  struct battle_entity *source,
                  struct battle_entity *target,
                  struct inventory_slot *slot)
{
	assert(battle_entity_ok(source));
	assert(battle_entity_ok(target));
	assert(slot);
	assert(bt);

	struct self *self;

	self = alloc_new0(sizeof (*self));
	self->state.data = self;
	self->state.update = update;
	self->state.draw = draw;
	self->state.finish = finish;

	battle_state_item_init(&self->data, bt, source, target, slot);
	battle_switch(bt, &self->state);
}
