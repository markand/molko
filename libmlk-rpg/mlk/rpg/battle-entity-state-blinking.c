/*
 * battle-entity-state-blinking.c -- the entity is blinking
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
#include <mlk/core/panic.h>
#include <mlk/core/sprite.h>
#include <mlk/core/texture.h>

#include "battle-entity-state-blinking.h"
#include "battle-entity-state.h"
#include "battle-entity.h"
#include "character.h"

#define TRANSPARENT     (150)
#define OPAQUE          (255)

struct self {
	struct battle_entity_state_blinking data;
	struct battle_entity_state state;
};

static int
update(struct battle_entity_state *st, struct battle_entity *et, unsigned int ticks)
{
	(void)et;

	return battle_entity_state_blinking_update(st->data, ticks);
}

static void
finish(struct battle_entity_state *st, struct battle_entity *et)
{
	(void)et;

	mlk_alloc_free(st->data);
}

void
battle_entity_state_blinking_init(struct battle_entity_state_blinking *blk, struct battle_entity *et)
{
	assert(blk);
	assert(battle_entity_ok(et));

	blk->tex = et->ch->sprites[CHARACTER_SPRITE_NORMAL]->texture;
	texture_set_alpha_mod(blk->tex, TRANSPARENT);
}

int
battle_entity_state_blinking_update(struct battle_entity_state_blinking *blk, unsigned int ticks)
{
	assert(blk);

	blk->elapsed += ticks;

	if (blk->elapsed >= 80) {
		blk->count += 1;
		blk->elapsed = 0;
	}

	texture_set_alpha_mod(blk->tex, blk->count % 2 == 0 ? TRANSPARENT : OPAQUE);

	return blk->count >= 3;
}

void
battle_entity_state_blinking(struct battle_entity *et)
{
	assert(battle_entity_ok(et));

	struct self *self;

	self = mlk_alloc_new0(1, sizeof (*self));
	self->state.data = self;
	self->state.update = update;
	self->state.finish = finish;

	battle_entity_state_blinking_init(&self->data, et);
	battle_entity_switch(et, &self->state);
}
