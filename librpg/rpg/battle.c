/*
 * battle.c -- battles
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <core/alloc.h>
#include <core/event.h>
#include <core/font.h>
#include <core/music.h>
#include <core/painter.h>
#include <core/sprite.h>
#include <core/texture.h>
#include <core/util.h>
#include <core/window.h>

#include <ui/align.h>
#include <ui/frame.h>
#include <ui/label.h>
#include <ui/theme.h>

#include "battle.h"
#include "battle-indicator.h"
#include "battle-state.h"
#include "battle-state-opening.h"
#include "battle-state-menu.h"
#include "battle-state-ai.h"
#include "character.h"
#include "spell.h"

struct indicator {
	struct drawable dw;
	struct battle_indicator bti;
};

static bool
indicator_update(struct drawable *dw, unsigned int ticks)
{
	struct indicator *id = dw->data;

	return battle_indicator_update(&id->bti, ticks);
}

static void
indicator_draw(struct drawable *dw)
{
	const struct indicator *id = dw->data;

	battle_indicator_draw(&id->bti, dw->x, dw->y);
}

static void
indicator_free(struct drawable *dw)
{
	struct indicator *id = dw->data;

	battle_indicator_finish(&id->bti);
	free(id);
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

static int
cmp_order(const void *d1, const void *d2)
{
	const struct battle_entity *et1 = *(const struct battle_entity **)d1;
	const struct battle_entity *et2 = *(const struct battle_entity **)d2;

	return et1->ch->agt < et2->ch->agt;
}

static bool
is_team(const struct battle *bt, const struct battle_entity *et)
{
	for (size_t i = 0; i < BATTLE_TEAM_MAX; ++i)
		if (&bt->team[i] == et)
			return true;

	return false;
}

static void
positionate_name(struct battle_entity *et, const struct battle *bt)
{
	unsigned int lw;

	/* Show the character name below its sprite. */
	et->name.text = et->ch->name;
	et->name.flags = LABEL_FLAGS_SHADOW;
	label_query(&et->name, &lw, NULL);
	et->name.y = et->y + et->ch->sprite->cellh + BATTLE_THEME(bt)->padding;
	et->name.x = et->x + (et->ch->sprite->cellw / 2) - (lw / 2);
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

		if (character_ok(et->ch) && sprite_ok(et->ch->sprite)) {
			nmemb++;
			requirement += et->ch->sprite->cellh;
		}
	}

	/* TODO: compute a correct x placement. */
	spacing = (window.h - requirement) / (nmemb + 1);
	x = window.w - 200;
	y = spacing;
	
	BATTLE_TEAM_FOREACH(bt, et) {
		if (character_ok(et->ch) && sprite_ok(et->ch->sprite)) {
			et->x = x;
			et->y = y;
			y += et->ch->sprite->cellh + spacing;
		}
	}
}

static void
positionate_bar(struct battle *bt)
{
	/* Bar is located at bottom. */
	bt->bar.w = window.w;
	bt->bar.h = window.h * 0.12;
	bt->bar.x = 0;
	bt->bar.y = window.h - bt->bar.h;
	
	battle_bar_positionate(&bt->bar, bt);
}

static void
draw_entity_sprite(const struct battle_entity *et)
{
	struct sprite *sprite = et->ch->sprite;
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

	sprite_draw(et->ch->sprite, row, 0, et->x, et->y);
}

static void
draw_entity_name(const struct battle_entity *et, const struct battle *bt)
{
	struct theme theme;
	struct label label = et->name;

	if (et == bt->order_cur) {
		theme_shallow(&theme, bt->theme);
		label.theme = &theme;
		theme.colors[THEME_COLOR_NORMAL] = theme.colors[THEME_COLOR_SELECTED];
	}

	label_draw(&label);
}

static void
draw_entity(const struct battle_entity *et, const struct battle *bt)
{
	draw_entity_sprite(et);
	draw_entity_name(et, bt);
}

static void
draw_entities(const struct battle *bt, struct battle_entity *entities, size_t entitiesz)
{
	for (size_t i = 0; i < entitiesz; ++i) {
		if (character_ok(entities[i].ch) && sprite_ok(entities[i].ch->sprite))
			draw_entity(&entities[i], bt);
	}
}

void
battle_start(struct battle *bt)
{
	struct battle_entity *et;

	BATTLE_TEAM_FOREACH(bt, et) {
		if (character_ok(et->ch)) {
			character_reset(et->ch);
			texture_set_alpha_mod(et->ch->sprite->texture, 255);
		}
	}
	BATTLE_ENEMY_FOREACH(bt, et) {
		if (character_ok(et->ch)) {
			character_reset(et->ch);
			texture_set_alpha_mod(et->ch->sprite->texture, 255);
		}
	}

	positionate_team(bt);
	positionate_bar(bt);
	positionate_names(bt);

	/* Start the state "opening" animation. */
	battle_state_opening(bt);

	/* Play music if present. */
	if (bt->music[0])
		music_play(bt->music[0], MUSIC_LOOP, 0);
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
	struct battle_entity *et, **porder;

	/* First, put a pointer for every enemy/team member. */
	porder = &bt->order[0];

	BATTLE_TEAM_FOREACH(bt, et)
		if (character_ok(et->ch))
			*porder++ = et;
	BATTLE_ENEMY_FOREACH(bt, et)
		if (character_ok(et->ch))
			*porder++ = et;

	/* Now sort. */
	qsort(bt->order, porder - bt->order, sizeof (*porder), cmp_order);
}

void
battle_attack(struct battle *bt, struct character *source, struct character *target)
{
	(void)source;

	assert(bt);
	assert(character_ok(source));
	assert(character_ok(target));

	/* TODO: Extreme computation of damage. */
	unsigned int count = 10;

	if (count > (unsigned int)target->hp)
		target->hp = 0;
	else
		target->hp -= count;

	battle_indicator_hp(bt, target, count);
}

void
battle_cast(struct battle *bt,
            struct character *source,
            const struct spell *spell,
            unsigned int selection)
{
	assert(bt);
	assert(source);
	assert(spell);
	assert((unsigned int)source->mp >= spell->mp);

	source->mp -= spell->mp;
	spell_action(spell, bt, source, selection);
}

void
battle_next(struct battle *bt)
{
	assert(bt);

	battle_bar_reset(&bt->bar);

	if (!bt->order_cur)
		bt->order_cur = bt->order[bt->order_curindex = 0];
	else {
		/* End of turn. */
		if (++bt->order_curindex >= BATTLE_ENTITY_MAX || !bt->order[bt->order_curindex]) {
			battle_order(bt);
			bt->order_cur = bt->order[bt->order_curindex = 0];
		} else
			bt->order_cur = bt->order[bt->order_curindex];
	}

	/* Change state depending on the kind of entity. */
	if (is_team(bt, bt->order_cur)) {
		battle_bar_open_menu(&bt->bar);
		battle_state_menu(bt);
	} else
		battle_state_ai(bt);
}

void
battle_indicator_hp(struct battle *bt, const struct character *target, unsigned int amount)
{
	assert(bt);
	assert(target);

	const struct battle_entity *et = find(bt, target);
	struct indicator *id = alloc_zero(1, sizeof (*id));

	id->bti.color = BATTLE_INDICATOR_HP_COLOR;
	id->bti.amount = amount;

	id->dw.x = et->x + target->sprite->cellw;
	id->dw.y = et->y + target->sprite->cellh;
	id->dw.data = id;
	id->dw.update = indicator_update;
	id->dw.draw = indicator_draw;
	id->dw.finish = indicator_free;

	battle_indicator_start(&id->bti);

	if (!drawable_stack_add(&bt->effects, &id->dw))
		drawable_finish(&id->dw);
}

void
battle_handle(struct battle *bt, const union event *ev)
{
	assert(bt && bt->state);
	assert(ev);

	/* Handle actions. */
	action_stack_handle(&bt->actions[0], ev);
	action_stack_handle(&bt->actions[1], ev);

	/* Handling of action is disallowed if there are pending actions. */
	if (action_stack_completed(&bt->actions[0]))
		battle_state_handle(bt->state, bt, ev);
}

bool
battle_update(struct battle *bt, unsigned int ticks)
{
	assert(bt && bt->state);

	action_stack_update(&bt->actions[0], ticks);
	action_stack_update(&bt->actions[1], ticks);
	drawable_stack_update(&bt->effects, ticks);

	/* Game cannot update if the actions[0] stack isn't completed. */
	if (!action_stack_completed(&bt->actions[0]))
		return false;

	return battle_state_update(bt->state, bt, ticks);
}

void
battle_draw(struct battle *bt)
{
	assert(bt && bt->state);

	painter_set_color(0xffffffff);
	painter_clear();

	if (bt->background && texture_ok(bt->background))
		texture_draw(bt->background, 0, 0);

	/* Draw entities. */
	draw_entities(bt, bt->team, NELEM(bt->team));
	draw_entities(bt, bt->enemies, NELEM(bt->enemies));

	battle_bar_draw(&bt->bar, bt);

	action_stack_draw(&bt->actions[0]);
	action_stack_draw(&bt->actions[1]);

	drawable_stack_draw(&bt->effects);

	return battle_state_draw(bt->state, bt);
}

void
battle_finish(struct battle *bt)
{
	assert(bt);

	if (bt->state)
		battle_state_finish(bt->state, bt);

	drawable_stack_finish(&bt->effects);

	action_stack_finish(&bt->actions[0]);
	action_stack_finish(&bt->actions[1]);

	memset(bt, 0, sizeof (*bt));
}
