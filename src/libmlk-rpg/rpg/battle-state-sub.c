/*
 * battle-state-sub.c -- battle state (sub)
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

#include <assert.h>

#include <core/event.h>
#include <core/sprite.h>
#include <core/trace.h>

#include <ui/theme.h>

#include <rpg/inventory.h>
#include <rpg/item.h>

#include "battle.h"
#include "battle-bar.h"
#include "battle-state.h"
#include "character.h"
#include "spell.h"

static void
start_select_spell(struct battle *bt)
{
	const struct character *ch = bt->order_cur->ch;
	const struct spell *sp = ch->spells[bt->bar.sub_grid.selected];
	struct selection slt = {0};

	/* Don't forget to reset the gridmenu state. */
	gridmenu_reset(&bt->bar.sub_grid);

	if (bt->bar.sub_grid.selected > CHARACTER_SPELL_MAX)
		return;
	if (!(sp = ch->spells[bt->bar.sub_grid.selected]) || sp->mp > (unsigned int)(ch->mp))
		return;

	spell_select(sp, bt, &slt);
	battle_state_selection(bt, &slt);

	/* A cursor should be present. */
	if (!sprite_ok(BATTLE_THEME(bt)->sprites[THEME_SPRITE_CURSOR]))
		tracef("battle: no cursor sprite in theme");
}

static void
start_select_object(struct battle *bt)
{
	const struct selection slt = {
		.allowed_kinds = SELECTION_KIND_ONE,
		.allowed_sides = SELECTION_SIDE_TEAM | SELECTION_SIDE_ENEMY,
		.index_side = 1,
		.index_character = bt->order_curindex
	};

	battle_state_selection(bt, &slt);
}

static void
draw_help(const struct battle *bt, const char *what)
{
	struct label label = {0};
	unsigned int lw = 0, lh = 0;

	label.flags = LABEL_FLAGS_SHADOW;
	label.text = what;
	label_query(&label, &lw, &lh);
	label.x = bt->bar.sub_grid.x + (bt->bar.sub_grid.w / 2) - (lw / 2);
	label.y = bt->bar.sub_grid.y - lh - BATTLE_THEME(bt)->padding;
	label_draw(&label);
}

static void
draw_spell_help(const struct battle *bt)
{
	const struct character *ch = bt->order_cur->ch;
	const struct spell *sp;

	if (bt->bar.sub_grid.selected >= CHARACTER_SPELL_MAX)
		return;
	if ((sp = ch->spells[bt->bar.sub_grid.selected]))
		return;

	draw_help(bt, sp->description);
}

static void
draw_object_help(const struct battle *bt)
{
	const struct inventory_slot *slot;

	if (bt->bar.sub_grid.selected >= INVENTORY_ITEM_MAX)
		return;

	slot = &bt->inventory->items[bt->bar.sub_grid.selected];

	if (!slot->item)
		return;

	draw_help(bt, slot->item->description);
}

static void
handle(struct battle_state *st, struct battle *bt, const union event *ev)
{
	(void)st;

	switch (ev->type) {
	case EVENT_KEYDOWN:
		switch (ev->key.key) {
		case KEY_ESCAPE:
			/* Escape go to the previous state. */
			battle_state_menu(bt);
			return;
		default:
			break;
		}
	default:
		break;
	}

	if (battle_bar_handle(&bt->bar, bt, ev)) {
		switch (bt->bar.menu) {
		case BATTLE_BAR_MENU_MAGIC:
			start_select_spell(bt);
			break;
		default:
			start_select_object(bt);
			break;
		}
	}
}

static void
draw(const struct battle_state *st, const struct battle *bt)
{
	(void)st;

	battle_bar_draw(&bt->bar, bt);

	if (bt->bar.menu == BATTLE_BAR_MENU_MAGIC)
		draw_spell_help(bt);
	else if (bt->bar.menu == BATTLE_BAR_MENU_OBJECTS)
		draw_object_help(bt);
}

void
battle_state_sub(struct battle *bt)
{
	assert(bt);

	static struct battle_state self = {
		.handle = handle,
		.draw = draw
	};

	battle_switch(bt, &self);
}