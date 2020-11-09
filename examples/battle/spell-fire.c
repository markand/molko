/*
 * spell-fire.c -- example of spell: fire
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

#include <stdlib.h>

#include <core/action.h>
#include <core/animation.h>
#include <core/alloc.h>

#include <ui/align.h>

#include <rpg/battle.h>
#include <rpg/character.h>
#include <rpg/spell.h>

#include "registry.h"
#include "spell-fire.h"

struct data {
	struct battle *battle;
	struct animation animation;
	struct action action;
	unsigned int selection;
};

static bool
update(struct action *act, unsigned int ticks)
{
	struct data *data = act->data;

	return animation_update(&data->animation, ticks);
}

static void
draw(struct action *act)
{
	const struct data *data = act->data;
	const struct battle_entity *et = &data->battle->enemies[data->selection];
	int x, y;

	align(ALIGN_CENTER,
	    &x, &y, data->animation.sprite->cellw, data->animation.sprite->cellh,
	    et->x, et->y, et->ch->sprite->cellw, et->ch->sprite->cellh);

	animation_draw(&data->animation, x, y);
}

static void
end(struct action *act)
{
	struct data *data = act->data;
	struct character *ch = data->battle->enemies[data->selection].ch;

	/* TODO: compute damage. */
	const unsigned int damage = 100;
	if ((unsigned int)ch->hp < damage)
		ch->hp = 0;
	else
		ch->hp -= damage;

	battle_indicator_hp(data->battle, data->battle->enemies[data->selection].ch, 100);
}

static void
finish(struct action *act)
{
	free(act->data);
}

static void
fire_action(struct battle *bt, struct character *owner, unsigned int selection)
{
	struct data *data;

	(void)owner;

	data = alloc_zero(1, sizeof (*data));
	data->battle = bt;
	data->selection = selection;
	data->action.data = data;
	data->action.update = update;
	data->action.draw = draw;
	data->action.finish = finish;
	data->action.end = end;

	animation_init(&data->animation, &registry_sprites[REGISTRY_TEXTURE_EXPLOSION], 12);
	animation_start(&data->animation);

	sound_play(&registry_sounds[REGISTRY_SOUND_FIRE], -1, 0);

	action_stack_add(&bt->actions[0], &data->action);
}

const struct spell spell_fire = {
	.name = "Fire",
	.description = "A delicate fire.",
	.mp = 5,
	.type = SPELL_TYPE_FIRE,
	.selection = SELECTION_ENEMY_ONE,
	.action = fire_action
};
