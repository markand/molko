/*
 * battle-entity-state-attacking.h -- the entity is attacking
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
#include <core/animation.h>
#include <core/panic.h>
#include <core/sprite.h>

#include "battle-entity.h"
#include "battle-entity-state.h"

struct data {
	struct battle_entity_state state;
	struct animation anim;
};

static int
update(struct battle_entity_state *st, struct battle_entity *et, unsigned int ticks)
{
	(void)et;

	struct data *data = st->data;

	return animation_update(&data->anim, ticks);
}

static void
draw(const struct battle_entity_state *st, const struct battle_entity *et)
{
	const struct data *data = st->data;

	animation_draw(&data->anim, et->x, et->y);
}

static void
finish(struct battle_entity_state *st, struct battle_entity *et)
{
	(void)et;

	free(st->data);
}

void
battle_entity_state_attacking(struct battle_entity *et, struct sprite *which)
{
	assert(battle_entity_ok(et));
	assert(sprite_ok(which));

	struct data *data;

	if (!(data = alloc_new0(sizeof (*data))))
		panic();

	animation_init(&data->anim, which, 100);
	animation_start(&data->anim);

	data->state.data = data;
	data->state.update = update;
	data->state.draw = draw;
	data->state.finish = finish;

	battle_entity_switch(et, &data->state);
}
