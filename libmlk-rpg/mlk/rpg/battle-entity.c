/*
 * battle-entity.c -- in game battle entity
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

#include <mlk/core/sprite.h>
#include <mlk/core/texture.h>

#include "battle.h"
#include "battle-entity.h"
#include "battle-entity-state.h"
#include "battle-entity-state-normal.h"
#include "character.h"

static void
draw_name(const struct battle_entity *et, const struct battle *bt)
{
	(void)bt;

	struct mlk_label label = et->name;

#if 0
	label.theme = BATTLE_THEME(bt);
#endif

#if 0
	if (et == battle_current(bt))
		label.flags |=  MLK_LABEL_FLAGS_SELECTED;
	else
		label.flags &= ~MLK_LABEL_FLAGS_SELECTED;
#endif

	mlk_label_draw(&label);
}

void
battle_entity_init(struct battle_entity *et)
{
	assert(et);

	character_reset(et->ch);
	mlk_texture_set_alpha_mod(et->ch->sprites[CHARACTER_SPRITE_NORMAL]->texture, 255);

	battle_entity_state_normal(et);
}

int
battle_entity_ok(const struct battle_entity *et)
{
	return et && character_ok(et->ch);
}

void
battle_entity_switch(struct battle_entity *et, struct battle_entity_state *st)
{
	assert(et);
	assert(st);

	if (et->state)
		battle_entity_state_finish(et->state, et);

	et->state = st;
}

int
battle_entity_update(struct battle_entity *et, unsigned int ticks)
{
	assert(et);

	return battle_entity_state_update(et->state, et, ticks);
}

void
battle_entity_draw_sprite(const struct battle_entity *et)
{
	struct mlk_sprite *sprite = et->ch->sprites[CHARACTER_SPRITE_NORMAL];
	int row;

	/*
	 * Ennemies are usually defined with a single image as such the
	 * sprite may contain only one cell/row. Otherwise if the user
	 * have provided a structured sprite, use appropriate row.
	 */
	if (sprite->nrows >= 6)
		row = 6;
	else
		row = 0;

	mlk_sprite_draw(sprite, row, 0, et->x, et->y);
}

void
battle_entity_draw(const struct battle_entity *et, const struct battle *bt)
{
	assert(et);
	assert(bt);

	draw_name(et, bt);
	battle_entity_state_draw(et->state, et);
}

void
battle_entity_finish(struct battle_entity *et)
{
	assert(et);

	battle_entity_state_finish(et->state, et);
}
