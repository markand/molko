/*
 * battle-entity-state-blinking.c -- the entity is blinking
 *
 * Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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
#include <core/texture.h>

#include "battle-entity.h"
#include "battle-entity-state.h"
#include "character.h"

#define TRANSPARENT     (150)
#define OPAQUE          (255)

struct blink {
	struct battle_entity_state state;
	struct texture *tex;
	unsigned int elapsed;
	unsigned int count;
};

static int
update(struct battle_entity_state *st, struct battle_entity *et, unsigned int ticks)
{
	(void)et;

	struct blink *blk = st->data;

	blk->elapsed += ticks;

	if (blk->elapsed >= 80) {
		blk->count += 1;
		blk->elapsed = 0;
	}

	texture_set_alpha_mod(blk->tex, blk->count % 2 == 0 ? TRANSPARENT : OPAQUE);

	return blk->count >= 3;
}

static void
finish(struct battle_entity_state *st, struct battle_entity *et)
{
	(void)et;

	free(st->data);
}

void
battle_entity_state_blinking(struct battle_entity *et)
{
	assert(et);

	struct blink *blk;

	if (!(blk = alloc_new0(sizeof (*blk))))
		panic();

	blk->tex = et->ch->sprites[CHARACTER_SPRITE_NORMAL]->texture;
	texture_set_alpha_mod(blk->tex, TRANSPARENT);

	blk->state.data = blk;
	blk->state.update = update;
	blk->state.finish = finish;

	battle_entity_switch(et, &blk->state);
}
