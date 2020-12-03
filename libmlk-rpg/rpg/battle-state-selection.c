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
#include "spell.h"

struct select {
	struct battle_state state;
	enum selection type;
	unsigned int selection;
};

static void
select_adj_in(struct select *select, const struct battle_entity entities[], size_t entitiesz, int step)
{
	assert(select->selection != (unsigned int)-1);

	unsigned int newselection = select->selection;

	if (step < 0) {
		while (newselection > 0) {
			if (character_ok(entities[--newselection].ch)) {
				select->selection = newselection;
				break;
			}
		}
	} else {
		while (newselection < entitiesz) {
			if (character_ok(entities[++newselection].ch)) {
				select->selection = newselection;
				break;
			}
		}
	}
}

static void
select_adj(struct select *select, const struct battle *bt, int step)
{
	switch (select->type) {
	case SELECTION_TEAM_ONE:
	case SELECTION_TEAM_COMBINED:
		select_adj_in(select, bt->team, NELEM(bt->team), step);
		break;
	case SELECTION_ENEMY_ONE:
	case SELECTION_ENEMY_COMBINED:
		select_adj_in(select, bt->enemies, NELEM(bt->enemies), step);
	default:
		break;
	}
}

static void
handle_keydown(struct battle_state *st, struct battle *bt, const union event *ev)
{
	assert(ev->type == EVENT_KEYDOWN);

	struct select *select = st->data;
	struct character *source = bt->order_cur->ch;
	const struct spell *sp = source->spells[bt->bar.sub_grid.selected];

	switch (ev->key.key) {
	case KEY_ESCAPE:
		battle_state_sub(bt);
		break;
	case KEY_ENTER:
		switch (bt->bar.menu) {
		case BATTLE_BAR_MENU_ATTACK:
			battle_attack(bt, source, bt->enemies[select->selection].ch);
			break;
		case BATTLE_BAR_MENU_MAGIC:
			battle_cast(bt, source, sp, select->selection);
			break;
		default:
			break;
		}
		break;
	case KEY_LEFT:
	case KEY_UP:
		select_adj(select, bt, -1);
		break;
	case KEY_RIGHT:
	case KEY_DOWN:
		select_adj(select, bt, +1);
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
            const struct battle_entity entities[],
            size_t entitiesz)
{
	const struct select *select = st->data;

	if (select->selection == (unsigned int)-1) {
		for (size_t i = 0; i < entitiesz; ++i) {
			const struct battle_entity *et = &entities[i];

			if (character_ok(et->ch))
				draw_cursor(bt, et);
		}
	} else
		draw_cursor(bt, &entities[select->selection]);
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

	switch (select->type) {
	case SELECTION_SELF:
		draw_cursor(bt, bt->order_cur);
		break;
	case SELECTION_ENEMY_ALL:
	case SELECTION_ENEMY_ONE:
	case SELECTION_ENEMY_COMBINED:
		draw_cursors(st, bt, bt->enemies, NELEM(bt->enemies));
		break;
	case SELECTION_TEAM_ALL:
	case SELECTION_TEAM_ONE:
	case SELECTION_TEAM_COMBINED:
		draw_cursors(st, bt, bt->team, NELEM(bt->team));
		break;
	default:
		break;
	}

}

static void
finish(struct battle_state *st, struct battle *bt)
{
	(void)bt;

	free(st->data);
}

void
battle_state_selection(struct battle *bt,
		       enum selection type,
		       unsigned int selection)
{
	assert(bt);

	struct select *select;

	if (!(select = alloc_new0(sizeof (*select))))
		panic();

	select->type = type;
	select->selection = selection;
	select->state.data = select;
	select->state.handle = handle;
	select->state.draw = draw;
	select->state.finish = finish;

	battle_switch(bt, &select->state);
}
