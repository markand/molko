/*
 * fire-minor.c -- minor fire
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

#include <math.h>
#include <stdlib.h>

#include <core/action.h>
#include <core/alloc.h>
#include <core/animation.h>
#include <core/maths.h>

#include <rpg/battle.h>
#include <rpg/character.h>
#include <rpg/selection.h>

#include <adventure/adventure_p.h>
#include <adventure/assets.h>

#include "fire-minor.h"

struct rendering {
	struct battle *battle;
	struct battle_entity *source;
	struct battle_entity *target;
	struct action action;
	struct animation animation;
};

static int
update(struct action *act, unsigned int ticks)
{
	struct rendering *rdr = act->data;

	return animation_update(&rdr->animation, ticks);
}

static void
draw(struct action *act)
{
	const struct rendering *rdr = act->data;

	animation_draw(&rdr->animation, rdr->target->x, rdr->target->y);
}

static void
end(struct action *act)
{
	struct rendering *rdr = act->data;
	float base;

	/* Compute damage. */
	/* TODO: move this into a general maths computation. */
	/* TODO: move min/max limits outside. */
	base  = util_nrand(50, 70);
	base += base * (maths_scale(rdr->source->ch->atk + rdr->source->ch->atkbonus, 0, 1000, 0, 100) / 100);

	/* Reduce damage taken. */
	base -= base * (maths_scale(rdr->target->ch->atk + rdr->target->ch->atkbonus, 0, 1000, 0, 100) / 200);
	base  = base < 0 ? 0 : base;

	/* TODO: add battle_damage function*/
	rdr->target->ch->hp = fmax(rdr->target->ch->hp - base, rdr->target->ch->hp);
	battle_indicator_hp(rdr->battle, rdr->target->ch, base);
}

static void
finish(struct action *act)
{
	free(act->data);
}

static void
select(const struct battle *bt, struct selection *slt)
{
	slt->index_side = 0;

	selection_first(slt, bt);
}

static void
action(struct battle *bt, struct character *owner, const struct selection *slt)
{
	struct rendering *rdr;

	/* Action. */
	rdr = alloc_new0(sizeof (*rdr));
	rdr->action.data = rdr;
	rdr->action.update = update;
	rdr->action.draw = draw;
	rdr->action.end = end;
	rdr->action.finish = finish;

	/* Battle and target. */
	rdr->battle = bt;
	rdr->source = battle_find(bt, owner);
	rdr->target = &bt->enemies[slt->index_character];

	/* Animation. */
	rdr->animation.delay = 10;
	rdr->animation.sprite = &assets_sprites[ASSETS_SPRITE_EXPLOSION];
	animation_start(&rdr->animation);

	action_stack_add(&bt->actions[0], &rdr->action);
}

const struct spell spell_fire_minor = {
	.name = N_("Fire Minor"),
	.description = N_("A small amount of fire balls"),
	.mp = 10,
	.type = SPELL_TYPE_FIRE,
	.select_kind = SELECTION_KIND_ONE,
	.select_side = SELECTION_SIDE_ENEMY,
	.select = select,
	.action = action
};
