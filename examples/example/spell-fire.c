/*
 * spell-fire.c -- example of spell: fire
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

#include <stdlib.h>

#include <core/action.h>
#include <core/animation.h>
#include <core/drawable.h>
#include <core/alloc.h>

#include <ui/align.h>

#include <rpg/battle-state-check.h>
#include <rpg/battle-state-rendering.h>
#include <rpg/battle.h>
#include <rpg/character.h>
#include <rpg/spell.h>

#include "registry.h"
#include "spell-fire.h"

struct self {
	struct battle *battle;
	struct character *target;
	struct animation animation;
	struct drawable drawable;
	unsigned int selection;
};

static int
update(struct drawable *dw, unsigned int ticks)
{
	struct self *self = dw->data;

	return animation_update(&self->animation, ticks);
}

static void
draw(struct drawable *dw)
{
	const struct self *self = dw->data;
	const struct battle_entity *et = self->battle->enemies[self->selection];
	const struct sprite *sprite = et->ch->sprites[CHARACTER_SPRITE_NORMAL];
	int x, y;

	align(ALIGN_CENTER,
	    &x, &y, self->animation.sprite->cellw, self->animation.sprite->cellh,
	    et->x, et->y, sprite->cellw, sprite->cellh);

	animation_draw(&self->animation, x, y);
}

static void
end(struct drawable *dw)
{
	struct self *self = dw->data;
	struct character *ch = self->battle->enemies[self->selection]->ch;

	/* TODO: compute damage. */
	const unsigned int damage = 100;

	if ((unsigned int)ch->hp < damage)
		ch->hp = 0;
	else
		ch->hp -= damage;

	battle_indicator_hp(self->battle, self->battle->enemies[self->selection]->ch, 100);
	battle_state_check(self->battle);
}

static void
finish(struct drawable *dw)
{
	free(dw->data);
}

static void
fire_select(const struct battle *bt, struct selection *slt)
{
	slt->index_side = 0;

	selection_first(slt, bt);
}

static void
fire_action(struct battle *bt, struct character *owner, const struct selection *slt)
{
	struct self *self;

	(void)owner;

	self = alloc_new0(sizeof (*self));
	self->selection = slt->index_character;
	self->battle = bt;
	self->drawable.data = self;
	self->drawable.update = update;
	self->drawable.draw = draw;
	self->drawable.finish = finish;
	self->drawable.end = end;

	animation_init(&self->animation, &registry_sprites[REGISTRY_TEXTURE_EXPLOSION], 12);
	animation_start(&self->animation);

	sound_play(&registry_sounds[REGISTRY_SOUND_FIRE]);
	battle_state_rendering(bt, &self->drawable);
}

const struct spell spell_fire = {
	.name = "Fire",
	.description = "A delicate fire.",
	.mp = 5,
	.type = SPELL_TYPE_FIRE,
	.select = fire_select,
	.select_kind = SELECTION_KIND_ONE,
	.select_side = SELECTION_SIDE_ENEMY,
	.action = fire_action
};
