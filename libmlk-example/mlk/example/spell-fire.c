/*
 * spell-fire.c -- example of spell: fire
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

#include <stdlib.h>

#include <mlk/core/action.h>
#include <mlk/core/animation.h>
#include <mlk/core/drawable.h>
#include <mlk/core/alloc.h>

#include <mlk/ui/align.h>

#include <mlk/rpg/battle-state-check.h>
#include <mlk/rpg/battle-state-rendering.h>
#include <mlk/rpg/battle.h>
#include <mlk/rpg/character.h>
#include <mlk/rpg/spell.h>

#include "registry.h"
#include "spell-fire.h"

struct self {
	struct battle *battle;
	struct character *target;
	struct mlk_animation animation;
	struct mlk_drawable drawable;
	unsigned int selection;
};

static int
update(struct mlk_drawable *dw, unsigned int ticks)
{
	struct self *self = dw->data;

	return mlk_animation_update(&self->animation, ticks);
}

static void
draw(struct mlk_drawable *dw)
{
	const struct self *self = dw->data;
	const struct battle_entity *et = self->battle->enemies[self->selection];
	const struct sprite *sprite = et->ch->sprites[CHARACTER_SPRITE_NORMAL];
	int x, y;

	align(ALIGN_CENTER,
	    &x, &y, self->animation.sprite->cellw, self->animation.sprite->cellh,
	    et->x, et->y, sprite->cellw, sprite->cellh);

	mlk_animation_draw(&self->animation, x, y);
}

static void
end(struct mlk_drawable *dw)
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
finish(struct mlk_drawable *dw)
{
	mlk_alloc_free(dw->data);
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

	self = mlk_alloc_new0(1, sizeof (*self));
	self->selection = slt->index_character;
	self->battle = bt;
	self->drawable.data = self;
	self->drawable.update = update;
	self->drawable.draw = draw;
	self->drawable.finish = finish;
	self->drawable.end = end;

	mlk_animation_init(&self->animation, &registry_sprites[REGISTRY_TEXTURE_EXPLOSION], 12);
	mlk_animation_start(&self->animation);

	mlk_sound_play(&registry_sounds[REGISTRY_SOUND_FIRE]);
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
