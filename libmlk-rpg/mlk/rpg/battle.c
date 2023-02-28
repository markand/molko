/*
 * battle.c -- battles
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mlk/core/alloc.h>
#include <mlk/core/event.h>
#include <mlk/core/font.h>
#include <mlk/core/music.h>
#include <mlk/core/sprite.h>
#include <mlk/core/texture.h>
#include <mlk/core/trace.h>
#include <mlk/core/util.h>
#include <mlk/core/window.h>

#include <mlk/ui/align.h>
#include <mlk/ui/frame.h>
#include <mlk/ui/label.h>
#include <mlk/ui/theme.h>

#include "battle-bar.h"
#include "battle-indicator.h"
#include "battle-state-ai.h"
#include "battle-state-attacking.h"
#include "battle-state-check.h"
#include "battle-state-menu.h"
#include "battle-state-opening.h"
#include "battle-state.h"
#include "battle.h"
#include "character.h"
#include "inventory.h"
#include "item.h"
#include "spell.h"

struct indicator {
	struct mlk_drawable dw;
	struct battle_indicator bti;
};

static int
indicator_update(struct mlk_drawable *dw, unsigned int ticks)
{
	struct indicator *id = dw->data;

	return battle_indicator_update(&id->bti, ticks);
}

static void
indicator_draw(struct mlk_drawable *dw)
{
	const struct indicator *id = dw->data;

	battle_indicator_draw(&id->bti, dw->x, dw->y);
}

static void
indicator_free(struct mlk_drawable *dw)
{
	struct indicator *id = dw->data;

	battle_indicator_finish(&id->bti);
	mlk_alloc_free(id);
}

static struct battle_entity *
find(struct battle *bt, const struct character *ch)
{
	struct battle_entity *et;

	BATTLE_TEAM_FOREACH(bt, et)
		if (et->ch == ch)
			return et;
	BATTLE_ENEMY_FOREACH(bt, et)
		if (et->ch == ch)
			return et;

	return NULL;
}

static struct battle_entity *
random_select(struct battle_entity **group, size_t groupsz)
{
	struct battle_entity *ret = NULL, *et = NULL;

	do {
		et = group[mlk_util_nrand(0, groupsz - 1)];

		if (et && et->ch)
			ret = et;
	} while (!ret);

	return ret;
}

static int
cmp_order(const void *d1, const void *d2)
{
	const struct battle_entity *et1 = *(const struct battle_entity **)d1;
	const struct battle_entity *et2 = *(const struct battle_entity **)d2;

	return et2->ch->agt < et1->ch->agt;
}

static int
is_team(const struct battle *bt, const struct character *ch)
{
	for (size_t i = 0; i < bt->teamsz; ++i)
		if (bt->team[i] && bt->team[i]->ch == ch)
			return 1;

	return 0;
}

static void
positionate_name(struct battle_entity *et, const struct battle *bt)
{
	unsigned int lw;
	struct mlk_sprite *sprite;

	/* Show the character name below its sprite. */
	sprite = et->ch->sprites[CHARACTER_SPRITE_NORMAL];

	et->name.text = et->ch->name;
	et->name.flags = MLK_LABEL_FLAGS_SHADOW;
	mlk_label_query(&et->name, &lw, NULL);
	et->name.y = et->y + sprite->cellh + BATTLE_THEME(bt)->padding;
	et->name.x = et->x + (sprite->cellw / 2) - (lw / 2);
}

static void
positionate_names(struct battle *bt)
{
	struct battle_entity *et;

	BATTLE_TEAM_FOREACH(bt, et)
		if (character_ok(et->ch))
			positionate_name(et, bt);
	BATTLE_ENEMY_FOREACH(bt, et)
		if (character_ok(et->ch))
			positionate_name(et, bt);
}

static void
positionate_team(struct battle *bt)
{
	struct battle_entity *et;
	unsigned int requirement = 0, nmemb = 0, spacing;
	int x, y;

	BATTLE_TEAM_FOREACH(bt, et) {
		/* Stop in case any member of the team has been positionated. */
		if (et->x != 0 || et->y != 0)
			return;

		if (battle_entity_ok(bt->team[i])) {
			nmemb++;
			requirement += et->ch->sprites[CHARACTER_SPRITE_NORMAL]->cellh;
		}
	}

	/* TODO: compute a correct x placement. */
	spacing = (mlk_window.h - requirement) / (nmemb + 1);
	x = mlk_window.w - 200;
	y = spacing;

	BATTLE_TEAM_FOREACH(bt, et) {
		if (battle_entity_ok(et)) {
			et->x = x;
			et->y = y;
			y += et->ch->sprites[CHARACTER_SPRITE_NORMAL]->cellh + spacing;
		}
	}
}

static void
draw_entities(const struct battle *bt, struct battle_entity **entities, size_t entitiesz)
{
	for (size_t i = 0; i < entitiesz; ++i) {
		if (battle_entity_ok(entities[i]))
			battle_entity_draw(entities[i], bt);
	}
}

static void
update_entities(struct battle_entity **entities, size_t entitiesz, unsigned int ticks)
{
	for (size_t i = 0; i < entitiesz; ++i) {
		if (battle_entity_ok(entities[i]))
			battle_entity_update(entities[i], ticks);
	}
}

void
battle_init(struct battle *bt)
{
	assert(bt);

	memset(bt, 0, sizeof (*bt));
}

int
battle_ok(const struct battle *bt)
{
	return bt && bt->state && bt->bar && bt->enemiesz && bt->team;
}

void
battle_start(struct battle *bt)
{
	assert(bt);

	struct battle_entity *et;

	BATTLE_TEAM_FOREACH(bt, et)
		if (battle_entity_ok(et))
			battle_entity_init(et);
	BATTLE_ENEMY_FOREACH(bt, et)
		if (battle_entity_ok(et))
			battle_entity_init(et);

	positionate_team(bt);
	positionate_names(bt);

	/* Start the state "opening" animation. */
	battle_state_opening(bt);

	/* Play music if present. */
	if (bt->music[0])
		mlk_music_play(bt->music[0], MLK_MUSIC_LOOP);

	battle_order(bt);
}

void
battle_switch(struct battle *bt, struct battle_state *st)
{
	assert(bt);
	assert(st);

	if (bt->state)
		battle_state_finish(bt->state, bt);

	bt->state = st;
}

void
battle_order(struct battle *bt)
{
	struct battle_entity **porder;

	/* Create a pointer list to every entity. */
	bt->ordersz = bt->teamsz + bt->enemiesz;

	if (!bt->order)
		bt->order = mlk_alloc_new0(bt->ordersz, sizeof (*bt->order));
	else
		bt->order = mlk_alloc_renew0(bt->order, bt->ordersz);

	bt->ordercur = porder = bt->order;

	for (size_t i = 0; i < bt->teamsz; ++i)
		if (bt->team[i] && character_ok(bt->team[i]->ch))
			*porder++ = bt->team[i];
	for (size_t i = 0; i < bt->enemiesz; ++i)
		if (bt->enemies[i] && character_ok(bt->enemies[i]->ch))
			*porder++ = bt->enemies[i];

	/* Now sort. */
	qsort(bt->order, bt->ordersz, sizeof (*bt->order), cmp_order);
}

struct battle_entity *
battle_current(const struct battle *bt)
{
	assert(bt);

	return *bt->ordercur;
}

size_t
battle_index(const struct battle *bt)
{
	assert(bt);

	return bt->ordercur - bt->order;
}

void
battle_attack(struct battle *bt,
              struct character *source,
              struct character *target)
{
	assert(bt);
	assert(character_ok(source));

	/* Target is empty? select randomly. */
	if (!target) {
		if (is_team(bt, source))
			target = random_select(bt->enemies, bt->enemiesz)->ch;
		else
			target = random_select(bt->team, bt->teamsz)->ch;
	}

	battle_state_attacking(battle_find(bt, source), battle_find(bt, target), bt);
}

void
battle_cast(struct battle *bt,
            struct character *source,
            const struct spell *spell,
            const struct selection *selection)
{
	assert(bt);
	assert(source);
	assert(spell);
	assert((unsigned int)source->mp >= spell->mp);

	/* TODO: animate. */
	source->mp -= spell->mp;
	spell_action(spell, bt, source, selection);
}

void
battle_use(struct battle *bt, const struct item *item, struct character *owner, struct character *target)
{
	assert(bt);
	assert(item);
	assert(owner);
	assert(target);

	/*
	 * Change the state to check prior to execute the item so it can change to something else
	 * if needed.
	 */
	battle_state_check(bt);

	inventory_consume(bt->inventory, item, 1);
	item_exec_battle(item, bt, owner, target);
}

void
battle_next(struct battle *bt)
{
	assert(bt);

	if (!bt->ordercur)
		battle_order(bt);
	else {
		if (bt->ordercur - bt->order + (size_t)1U >= bt->ordersz)
			battle_order(bt);
		else
			bt->ordercur++;
	}

	if (is_team(bt, battle_current(bt)->ch)) {
		battle_bar_start(bt->bar, bt);
		battle_state_menu(bt);
	} else
		battle_state_ai(bt);
}

struct battle_entity *
battle_find(struct battle *bt, const struct character *ch)
{
	assert(bt);

	return find(bt, ch);
}

void
battle_indicator_hp(struct battle *bt, const struct character *target, long amount)
{
	assert(bt);
	assert(target);

	const struct battle_entity *et = find(bt, target);
	struct indicator *id;

	if (!(bt->effects)) {
		mlk_tracef("unable to add id without a drawable_stack");
		return;
	}

	id = mlk_alloc_new0(1, sizeof (*id));
	id->bti.color = BATTLE_INDICATOR_HP_COLOR;
	id->bti.amount = labs(amount);

	/* TODO: positionate better. */
	id->dw.x = et->x + target->sprites[CHARACTER_SPRITE_NORMAL]->cellw;
	id->dw.y = et->y + target->sprites[CHARACTER_SPRITE_NORMAL]->cellh;
	id->dw.data = id;
	id->dw.update = indicator_update;
	id->dw.draw = indicator_draw;
	id->dw.finish = indicator_free;

	battle_indicator_start(&id->bti);

	if (mlk_drawable_stack_add(bt->effects, &id->dw) < 0)
		mlk_drawable_finish(&id->dw);
}

void
battle_handle_component(struct battle *bt, const union mlk_event *ev, enum battle_component comp)
{
	assert(bt);
	assert(ev);

	if (comp & BATTLE_COMPONENT_BAR)
		battle_bar_handle(bt->bar, bt, ev);
	if ((comp & BATTLE_COMPONENT_ACTIONS) && bt->actions)
		mlk_action_stack_handle(bt->actions, ev);
}

void
battle_handle(struct battle *bt, const union mlk_event *ev)
{
	assert(bt);
	assert(ev);

	battle_state_handle(bt->state, bt, ev);
}

void
battle_update_component(struct battle *bt, unsigned int ticks, enum battle_component comp)
{
	assert(bt);

	if (comp & BATTLE_COMPONENT_ENTITIES) {
		update_entities(bt->team, bt->teamsz, ticks);
		update_entities(bt->enemies, bt->enemiesz, ticks);
	}
	if (comp & BATTLE_COMPONENT_BAR)
		battle_bar_update(bt->bar, bt, ticks);
	if ((comp & BATTLE_COMPONENT_ACTIONS) && bt->actions)
		mlk_action_stack_update(bt->actions, ticks);
	if ((comp & BATTLE_COMPONENT_DRAWABLES) && bt->effects)
		mlk_drawable_stack_update(bt->effects, ticks);
}

int
battle_update(struct battle *bt, unsigned int ticks)
{
	assert(bt && bt->state);

	return battle_state_update(bt->state, bt, ticks);
}

void
battle_draw_component(const struct battle *bt, enum battle_component comp)
{
	assert(bt);

	if ((comp & BATTLE_COMPONENT_BACKGROUND) && mlk_texture_ok(bt->background))
		mlk_texture_scale(bt->background,
		    0, 0, bt->background->w, bt->background->h,
		    0, 0, mlk_window.w, mlk_window.h,
		    0.f);
	if (comp & BATTLE_COMPONENT_ENTITIES) {
		draw_entities(bt, bt->team, bt->teamsz);
		draw_entities(bt, bt->enemies, bt->enemiesz);
	}
	if (comp & BATTLE_COMPONENT_BAR)
		battle_bar_draw(bt->bar, bt);
	if ((comp & BATTLE_COMPONENT_ACTIONS) && bt->actions)
		mlk_action_stack_draw(bt->actions);
	if ((comp & BATTLE_COMPONENT_DRAWABLES) && bt->effects)
		mlk_drawable_stack_draw(bt->effects);
}

void
battle_draw(const struct battle *bt)
{
	assert(battle_ok(bt));

	battle_state_draw(bt->state, bt);
}

void
battle_finish(struct battle *bt)
{
	assert(bt);

	if (bt->state)
		battle_state_finish(bt->state, bt);

	mlk_alloc_free(bt->order);
	memset(bt, 0, sizeof (*bt));
}
