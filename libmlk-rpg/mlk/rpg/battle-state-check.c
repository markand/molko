/*
 * battle-state-check.c -- battle state (check status)
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

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <mlk/core/alloc.h>
#include <mlk/core/panic.h>
#include <mlk/core/sprite.h>
#include <mlk/core/texture.h>
#include <mlk/core/trace.h>

#include "battle-state-check.h"
#include "battle-state-lost.h"
#include "battle-state-victory.h"
#include "battle-state.h"
#include "battle.h"
#include "character.h"

struct fadeout {
	struct character *ch;
	int x;
	int y;
	struct drawable dw;
	unsigned int alpha;
	unsigned int elapsed;
};

static int
fadeout_update(struct drawable *dw, unsigned int ticks)
{
	struct fadeout *fade = dw->data;

	fade->elapsed += ticks;

	if (fade->elapsed >= 8) {
		fade->elapsed = 0;

		if (fade->alpha == 0)
			return 1;

		fade->alpha -= 10;
	}

	return 0;
}

static void
fadeout_draw(struct drawable *dw)
{
	const struct fadeout *fade = dw->data;
	struct sprite *sprite = fade->ch->sprites[CHARACTER_SPRITE_NORMAL];

	texture_set_alpha_mod(sprite->texture, fade->alpha);
	sprite_draw(sprite, 0, 0, fade->x, fade->y);
	texture_set_alpha_mod(sprite->texture, 255);
}

static void
fadeout_finish(struct drawable *dw)
{
	free(dw->data);
}

static void
fadeout(struct battle *bt, struct battle_entity *et)
{
	struct fadeout *fade;

	if (!bt->effects) {
		tracef("can't create a fadeout effect without a drawable_stack");
		return;
	}

	fade = mlk_alloc_new0(1, sizeof (*fade));
	fade->ch = et->ch;
	fade->x = et->x;
	fade->y = et->y;
	fade->alpha = 250;
	fade->dw.data = fade;
	fade->dw.draw = fadeout_draw;
	fade->dw.update = fadeout_update;
	fade->dw.finish = fadeout_finish;

	if (drawable_stack_add(bt->effects, &fade->dw) < 0)
		mlk_alloc_free(fade);
}

static int
is_dead(const struct battle *bt)
{
	const struct battle_entity *et;

	BATTLE_TEAM_FOREACH(bt, et) {
		if (!character_ok(et->ch))
			continue;
		if (et->ch->hp > 0)
			return 0;
	}

	return 1;
}

static int
is_won(const struct battle *bt)
{
	const struct battle_entity *et;

	BATTLE_ENEMY_FOREACH(bt, et)
		if (character_ok(et->ch))
			return 0;

	return 1;
}

static void
clean(struct battle *bt)
{
	for (size_t i = 0; i < bt->enemiesz; ++i) {
		if (bt->enemies[i] && character_ok(bt->enemies[i]->ch) && bt->enemies[i]->ch->hp == 0) {
			fadeout(bt, bt->enemies[i]);
			bt->enemies[i] = NULL;
		}
	}
}

static int
update(struct battle_state *st, struct battle *bt, unsigned int ticks)
{
	(void)st;
	(void)ticks;

	battle_state_check_update(bt);

	return 0;
}

static void
draw(const struct battle_state *st, const struct battle *bt)
{
	(void)st;

	battle_state_check_draw(bt);
}

static void
finish(struct battle_state *st, struct battle *bt)
{
	(void)bt;

	free(st);
}

void
battle_state_check_update(struct battle *bt)
{
	assert(battle_ok(bt));

	clean(bt);

	if (is_dead(bt))
		battle_state_lost(bt);
	else if (is_won(bt))
		battle_state_victory(bt);
	else
		battle_next(bt);
}

void
battle_state_check_draw(const struct battle *bt)
{
	assert(battle_ok(bt));

	battle_draw_component(bt, BATTLE_COMPONENT_ALL);
}

void
battle_state_check(struct battle *bt)
{
	assert(battle_ok(bt));

	struct battle_state *self;

	self = mlk_alloc_new0(1, sizeof (*self));
	self->data = bt;
	self->update = update;
	self->draw = draw;
	self->finish = finish;

	battle_switch(bt, self);
}
