/*
 * battle-state-check.c -- battle state (check status)
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
#include <string.h>

#include <core/alloc.h>
#include <core/panic.h>
#include <core/sprite.h>
#include <core/texture.h>

#include "battle.h"
#include "battle-state.h"
#include "battle-state-check.h"
#include "battle-state-lost.h"
#include "battle-state-victory.h"
#include "character.h"

struct fadeout {
	struct character *ch;
	int x;
	int y;
	struct action action;
	unsigned int alpha;
	unsigned int elapsed;
};

static bool
fadeout_update(struct action *act, unsigned int ticks)
{
	struct fadeout *fade = act->data;
	struct character *ch = fade->ch;

	fade->elapsed += ticks;

	if (fade->elapsed >= 8) {
		fade->elapsed = 0;

		if (fade->alpha == 0)
			return true;

		fade->alpha -= 10;
		texture_set_alpha_mod(ch->sprites[CHARACTER_SPRITE_WALK]->texture, fade->alpha);
	}

	return false;
}

static void
fadeout_draw(struct action *act)
{
	const struct fadeout *fade = act->data;

	sprite_draw(fade->ch->sprites[CHARACTER_SPRITE_WALK], 0, 0, fade->x, fade->y);
}

static void
fadeout_finish(struct action *act)
{
	free(act->data);
}

static void
fadeout(struct battle *bt, struct battle_entity *et)
{
	struct fadeout *fade;

	if (!(fade = alloc_new0(sizeof (*fade))))
		panic();

	fade->ch = et->ch;
	fade->x = et->x;
	fade->y = et->y;
	fade->alpha = 250;
	fade->action.data = fade;
	fade->action.draw = fadeout_draw;
	fade->action.update = fadeout_update;
	fade->action.finish = fadeout_finish;

	if (!action_stack_add(&bt->actions[1], &fade->action))
		free(fade);

	memset(et, 0, sizeof (*et));
}

static bool
is_dead(const struct battle *bt)
{
	const struct battle_entity *et;

	BATTLE_TEAM_FOREACH(bt, et) {
		if (!character_ok(et->ch))
			continue;
		if (et->ch->hp > 0)
			return false;
	}

	return true;
}

static bool
is_won(const struct battle *bt)
{
	const struct battle_entity *et;

	BATTLE_ENEMY_FOREACH(bt, et)
		if (character_ok(et->ch))
			return false;

	return true;
}

static void
clean(struct battle *bt)
{
	struct battle_entity *et;

	BATTLE_ENEMY_FOREACH(bt, et)
		if (character_ok(et->ch) && et->ch->hp == 0)
			fadeout(bt, et);
}

static bool
update(struct battle_state *st, struct battle *bt, unsigned int ticks)
{
	(void)st;
	(void)ticks;

	clean(bt);

	if (is_dead(bt))
		battle_state_lost(bt);
	else if (is_won(bt))
		battle_state_victory(bt);
	else
		battle_next(bt);

	return false;
}

void
battle_state_check(struct battle *bt)
{
	assert(bt);

	static struct battle_state self = {
		.update = update
	};

	battle_switch(bt, &self);
}
