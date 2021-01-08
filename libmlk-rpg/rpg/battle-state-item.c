/*
 * battle-state-item.c -- battle state (using item)
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
#include <core/window.h>

#include <rpg/inventory.h>
#include <rpg/item.h>

#include <ui/align.h>
#include <ui/frame.h>
#include <ui/label.h>

#include "battle-entity-state.h"
#include "battle-state.h"
#include "battle.h"

enum substate {
	SUBSTATE_ADVANCING,
	SUBSTATE_MESSAGE,
	SUBSTATE_RETURNING
};

struct msg {
	struct frame frame;
	struct label label;
	unsigned int elapsed;
};

struct self {
	enum substate substate;
	struct msg msg;
	struct battle_entity *source;
	struct battle_entity *target;
	struct battle_state state;
	struct inventory_slot *slot;
	int origin_x;
};

static bool
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
		self->msg.elapsed += ticks;

		if (self->msg.elapsed >= 2000) {
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

	return false;
}

static void
draw(const struct battle_state *st, const struct battle *bt)
{
	struct self *self = st->data;

	if (self->substate == SUBSTATE_MESSAGE) {
		frame_draw(&self->msg.frame);
		label_draw(&self->msg.label);
	}
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
	unsigned int lw, lh;

	if (!(self = alloc_new0(sizeof (*self))))
		panic();

	self->source = battle_find(bt, source);
	self->target = battle_find(bt, target);
	self->slot = slot;
	self->origin_x = self->source->x;

	/* Prepare message frame. */
	self->msg.frame.w = window.w / 3;
	self->msg.frame.h = window.h / 15;
	self->msg.frame.theme = bt->theme;

	align(ALIGN_TOP,
	    &self->msg.frame.x, &self->msg.frame.y, self->msg.frame.w, self->msg.frame.h,
	    0, 20, window.w, window.h);

	/* Prepare message label box. */
	self->msg.label.text = slot->item->name;
	self->msg.label.flags = LABEL_FLAGS_SHADOW;
	self->msg.label.theme = bt->theme;
	label_query(&self->msg.label, &lw, &lh);

	align(ALIGN_CENTER,
	    &self->msg.label.x, &self->msg.label.y, lw, lh,
	    self->msg.frame.x, self->msg.frame.y, self->msg.frame.w, self->msg.frame.h);

	self->state.data = self;
	self->state.update = update;
	self->state.draw = draw;
	self->state.finish = finish;

	battle_entity_state_moving(self->source, self->origin_x - 100, self->source->y);
	battle_switch(bt, &self->state);
}
