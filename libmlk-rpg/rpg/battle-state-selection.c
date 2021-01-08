/*
 * battle-state-selection.h -- battle state (selection)
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
#include <core/event.h>
#include <core/panic.h>
#include <core/sprite.h>
#include <core/util.h>

#include <ui/theme.h>

#include "battle.h"
#include "battle-bar.h"
#include "battle-state.h"
#include "character.h"
#include "inventory.h"
#include "selection.h"
#include "spell.h"

struct select {
	struct battle_state state;
	struct selection slt;
};

static void
use(const struct select *select, struct battle *bt)
{
	struct inventory_slot *slot;
	struct character *source, *target;

	if (bt->bar.sub_grid.selected >= INVENTORY_ITEM_MAX)
		return;
	if (!(slot = &bt->inventory->items[bt->bar.sub_grid.selected]))
		return;

	source = bt->order_cur->ch;
	target = select->slt.index_side == 0
		? bt->enemies[select->slt.index_character].ch
		: bt->team[select->slt.index_character].ch;

	battle_state_item(bt, source, target, slot);
}

static void
attack(struct select *select, struct battle *bt)
{
	struct character *target;

	if (select->slt.index_side == 0)
		target = bt->enemies[select->slt.index_character].ch;
	else
		target = bt->team[select->slt.index_character].ch;

	battle_attack(bt, bt->order_cur->ch, target);
}

static void
cast(struct select *select, struct battle *bt)
{
	struct character *source = bt->order_cur->ch;
	const struct spell *spell = source->spells[bt->bar.sub_grid.selected];

	battle_cast(bt, source, spell, &select->slt);
}

static void
select_adj_in(struct select *select, const struct battle_entity *entities, size_t entitiesz, int step)
{
	assert(select->slt.index_character != (unsigned int)-1);

	unsigned int newselection = select->slt.index_character;

	if (step < 0) {
		while (newselection > 0) {
			if (character_ok(entities[--newselection].ch)) {
				select->slt.index_character = newselection;
				break;
			}
		}
	} else {
		while (newselection < entitiesz) {
			if (character_ok(entities[++newselection].ch)) {
				select->slt.index_character = newselection;
				break;
			}
		}
	}
}

static void
select_adj(struct select *select, const struct battle *bt, int step)
{
	if (select->slt.index_side == 0)
		select_adj_in(select, bt->enemies, UTIL_SIZE(bt->enemies), step);
	else
		select_adj_in(select, bt->team, UTIL_SIZE(bt->team), step);
}

static void
handle_keydown(struct battle_state *st, struct battle *bt, const union event *ev)
{
	assert(ev->type == EVENT_KEYDOWN);

	struct select *select = st->data;

	switch (ev->key.key) {
	case KEY_ESCAPE:
		switch (bt->bar.menu) {
		case BATTLE_BAR_MENU_MAGIC:
		case BATTLE_BAR_MENU_OBJECTS:
			battle_state_sub(bt);
			break;
		default:
			battle_state_menu(bt);
			break;
		}
		break;
	case KEY_ENTER:
		switch (bt->bar.menu) {
		case BATTLE_BAR_MENU_ATTACK:
			attack(select, bt);
			break;
		case BATTLE_BAR_MENU_MAGIC:
			cast(select, bt);
			break;
		case BATTLE_BAR_MENU_OBJECTS:
			use(select, bt);
			break;
		default:
			break;
		}
		break;
	case KEY_LEFT:
		if (select->slt.allowed_sides & SELECTION_SIDE_ENEMY)
			select->slt.index_side = 0;
		break;
	case KEY_RIGHT:
		if (select->slt.allowed_sides & SELECTION_SIDE_TEAM)
			select->slt.index_side = 1;
		break;
	case KEY_UP:
		select_adj(select, bt, -1);
		break;
	case KEY_DOWN:
		select_adj(select, bt, +1);
		break;
	case KEY_TAB:
		if (select->slt.allowed_kinds == SELECTION_KIND_BOTH)
			select->slt.index_character = -1;
		break;
	default:
		break;
	}
}

static void
draw_cursor(const struct battle *bt, const struct battle_entity *et)
{
	const struct theme *theme = BATTLE_THEME(bt);
	const struct sprite *cursor = theme->sprites[THEME_SPRITE_CURSOR];
	int x, y;
	unsigned int lh;

	if (!cursor)
		return;

	label_query(&et->name, NULL, &lh);

	x = et->name.x - cursor->cellw - theme->padding;
	y = et->name.y + (((int)(lh) - (int)(cursor->cellh)) / 2);

	sprite_draw(cursor, 1, 2, x, y);
}

static void
draw_cursors(const struct battle_state *st,
             const struct battle *bt,
             const struct battle_entity *entities,
             size_t entitiesz)
{
	for (size_t i = 0; i < entitiesz; ++i) {
		const struct battle_entity *et = &entities[i];

		if (character_ok(et->ch))
			draw_cursor(bt, et);
	}
}

static void
handle(struct battle_state *st, struct battle *bt, const union event *ev)
{
	(void)st;

	switch (ev->type) {
	case EVENT_KEYDOWN:
		handle_keydown(st, bt, ev);
		break;
	default:
		break;
	}
}

static void
draw(const struct battle_state *st, const struct battle *bt)
{
	const struct select *select = st->data;

	if (select->slt.index_character == -1) {
		/* All selected. */
		if (select->slt.index_side == 0)
			draw_cursors(st, bt, bt->enemies, UTIL_SIZE(bt->enemies));
		else
			draw_cursors(st, bt, bt->team, UTIL_SIZE(bt->team));
	} else {
		/* Select one. */
		if (select->slt.index_side == 0)
			draw_cursor(bt, &bt->enemies[select->slt.index_character]);
		else
			draw_cursor(bt, &bt->team[select->slt.index_character]);
	}
}

static void
finish(struct battle_state *st, struct battle *bt)
{
	(void)bt;

	free(st->data);
}

void
battle_state_selection(struct battle *bt, const struct selection *slt)
{
	assert(bt);

	struct select *select;

	if (!(select = alloc_new0(sizeof (*select))))
		panic();

	select->state.data = select;
	select->state.handle = handle;
	select->state.draw = draw;
	select->state.finish = finish;
	memcpy(&select->slt, slt, sizeof (*slt));

	battle_switch(bt, &select->state);
}
