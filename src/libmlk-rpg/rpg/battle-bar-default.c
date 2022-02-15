/*
 * battle-bar-default.c -- default battle status bar and menu implementation
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <core/alloc.h>
#include <core/event.h>
#include <core/font.h>
#include <core/sprite.h>
#include <core/trace.h>
#include <core/util.h>
#include <core/window.h>

#include <ui/align.h>
#include <ui/theme.h>

#include "battle-bar-default.h"
#include "battle-bar.h"
#include "battle-state-item.h"
#include "battle-state-selection.h"
#include "battle.h"
#include "character.h"
#include "inventory.h"
#include "item.h"
#include "rpg_p.h"
#include "spell.h"

struct self {
	struct battle_bar_default data;
	struct battle_bar bar;
};

/*
 * The following validate_* functions are called when the user has validated a
 * selection depending on the current menu (e.g. Magic, Items).
 *
 * They change the battle state to the appropriate one.
 */

static void
validate_attack(struct battle_bar_default *bar, struct battle *bt, const struct selection *sel)
{
	(void)bar;

	struct character *target;

	if (sel->index_side == 0)
		target = bt->enemies[sel->index_character].ch;
	else
		target = bt->team[sel->index_character].ch;

	battle_attack(bt, bt->order_cur->ch, target);
}

static void
validate_magic(struct battle_bar_default *bar, struct battle *bt, const struct selection *sel)
{
	struct character *source = bt->order_cur->ch;
	const struct spell *spell = source->spells[bar->sub_grid.selected];

	battle_cast(bt, source, spell, sel);
}

static void
validate_item(struct battle_bar_default *bar, struct battle *bt, const struct selection *sel)
{
	struct inventory_slot *slot;
	struct battle_entity *source, *target;

	if (bar->sub_grid.selected >= INVENTORY_ITEM_MAX)
		return;
	if (!(slot = &bt->inventory->items[bar->sub_grid.selected]))
		return;

	source = bt->order_cur;
	target = sel->index_side == 0
		? &bt->enemies[sel->index_character]
		: &bt->team[sel->index_character];

	/* TODO: battle_use? */
	battle_state_item(bt, source, target, slot);
}

/*
 * The following functions are used to switch to the battle selection state
 * using the appropriate selector algorithm. For example, an item can only be
 * used on a unique target while a spell can have multiple choices.
 */

static void
switch_selection_attack(struct battle *bt)
{
	struct selection sel = {
		.allowed_kinds = SELECTION_KIND_ONE,
		.allowed_sides = SELECTION_SIDE_ENEMY,
		.index_side = 0
	};

	/* Just make sure */

	selection_first(&sel, bt);
	battle_state_selection(bt, &sel);
}

static void
switch_selection_spell(struct battle_bar_default *bar, struct battle *bt)
{
	const struct character *ch = bt->order_cur->ch;
	const struct spell *sp = ch->spells[bar->sub_grid.selected];
	struct selection sel = {0};

	/* Don't forget to reset the gridmenu state. */
	gridmenu_reset(&bar->sub_grid);

	if (bar->sub_grid.selected > CHARACTER_SPELL_MAX)
		return;
	if (!(sp = ch->spells[bar->sub_grid.selected]) || sp->mp > (unsigned int)(ch->mp))
		return;

	/* Use the spell selection algorithm to fill default values. */
	spell_select(sp, bt, &sel);
	battle_state_selection(bt, &sel);

	/* A cursor should be present. */
	if (!sprite_ok(BATTLE_THEME(bt)->sprites[THEME_SPRITE_CURSOR]))
		tracef("battle: no cursor sprite in theme");
}

static void
switch_selection_item(struct battle *bt)
{
	const struct selection slt = {
		.allowed_kinds = SELECTION_KIND_ONE,
		.allowed_sides = SELECTION_SIDE_TEAM | SELECTION_SIDE_ENEMY,
		.index_side = 1,
		.index_character = bt->order_curindex
	};

	battle_state_selection(bt, &slt);
}

/*
 * The following functions actually draw the bar and their components depending
 * on the current selected menu.
 */

static void
draw_help(const struct battle_bar_default *bar, const struct battle *bt, const char *what)
{
	struct label label = {0};
	unsigned int lw = 0, lh = 0;

	label.flags = LABEL_FLAGS_SHADOW;
	label.text = what;
	label_query(&label, &lw, &lh);
	label.x = bar->sub_grid.x + (bar->sub_grid.w / 2) - (lw / 2);
	label.y = bar->sub_grid.y - lh - BATTLE_THEME(bt)->padding;
	label_draw(&label);
}

static void
draw_spell_help(const struct battle_bar_default *bar, const struct battle *bt)
{
	const struct character *ch = bt->order_cur->ch;
	const struct spell *sp;

	if (bar->sub_grid.selected >= CHARACTER_SPELL_MAX)
		return;
	if (!(sp = ch->spells[bar->sub_grid.selected]))
		return;

	draw_help(bar, bt, sp->description);
}

static void
draw_item_help(const struct battle_bar_default *bar, const struct battle *bt)
{
	const struct inventory_slot *slot;

	if (bar->sub_grid.selected >= INVENTORY_ITEM_MAX)
		return;

	slot = &bt->inventory->items[bar->sub_grid.selected];

	if (!slot->item)
		return;

	draw_help(bar, bt, slot->item->description);
}

static void
draw_status_character_stats(const struct battle *bt,
                            const struct character *ch,
                            int x,
                            int y,
                            unsigned int w,
                            unsigned int h)
{
	struct theme *theme = BATTLE_THEME(bt);
	struct label label;
	unsigned int spacing, lw, lh;
	char line[64];

	/* Compute spacing between elements. */
	spacing = h - (font_height(theme->fonts[THEME_FONT_INTERFACE]) * 3);
	spacing /= 4;

	/* Reuse the same label. */
	label.theme = theme;
	label.text = line;
	label.flags = LABEL_FLAGS_SHADOW;

	/* HP. */
	snprintf(line, sizeof (line), "%d/%u", ch->hp, ch->hpmax);
	label_query(&label, &lw, &lh);
	label.x = x + w - lw - theme->padding;
	label.y = y + spacing;
	label_draw(&label);

	/* MP. */
	snprintf(line, sizeof (line), "%d/%u", ch->mp, ch->mpmax);
	label_query(&label, &lw, &lh);
	label.x = x + w - lw - theme->padding;
	label.y = label.y + lh + spacing;
	label_draw(&label);

	/* Status. */
	/* TODO: list all status. */
}

static void
draw_status_character(const struct battle_bar_default *bar,
                      const struct battle *bt,
                      const struct character *ch,
                      unsigned int index)
{
	int x, y;
	unsigned int w, h;

	/* Compute bounding box for rendering. */
	w = bar->status_frame.w / BATTLE_TEAM_MAX;
	h = bar->status_frame.h;
	x = bar->status_frame.x + (index * w);
	y = bar->status_frame.y;

	draw_status_character_stats(bt, ch, x, y, w, h);
}

static void
draw_status_characters(const struct battle_bar_default *bar, const struct battle *bt)
{
	const struct battle_entity *et;
	unsigned int index = 0;

	BATTLE_TEAM_FOREACH(bt, et) {
		if (character_ok(et->ch))
			draw_status_character(bar, bt, et->ch, index);

		++index;
	}
}

static void
draw_status(const struct battle_bar_default *bar, const struct battle *bt)
{
	frame_draw(&bar->status_frame);
	draw_status_characters(bar, bt);
}

static void
draw_menu(const struct battle_bar_default *bar, const struct battle *bt)
{
	struct {
		unsigned int w, h;
		enum align align;
		struct label label;
	} buttons[] = {
		{
			.align = ALIGN_TOP,
			.label = {
				.text = _("Attack"),
				.flags = LABEL_FLAGS_SHADOW
			}
		},
		{
			.align = ALIGN_RIGHT,
			.label = {
				.text = _("Magic"),
				.flags = LABEL_FLAGS_SHADOW
			}
		},
		{
			.align = ALIGN_BOTTOM,
			.label = {
				.text = _("Objects"),
				.flags = LABEL_FLAGS_SHADOW
			}
		},
		{
			.align = ALIGN_LEFT,
			.label = {
				.text = _("Special"),
				.flags = LABEL_FLAGS_SHADOW
			}
		}
	};

	struct theme theme;
	int bx, by;
	unsigned int bw, bh;

	/* Copy theme according to menu selection. */
	theme_shallow(&theme, bt->theme);

	/* Compute bounding box with margins removed. */
	bx = bar->menu_frame.x + theme.padding;
	by = bar->menu_frame.y + theme.padding;
	bw = bar->menu_frame.w - theme.padding * 2;
	bh = bar->menu_frame.h - theme.padding * 2;

	/* Draw menu frame. */
	frame_draw(&bar->menu_frame);

	for (size_t i = 0; i < UTIL_SIZE(buttons); ++i) {
		buttons[i].label.theme = &theme;

		label_query(&buttons[i].label, &buttons[i].w, &buttons[i].h);

		/* Change theme if it's selected. */
		if ((size_t)bar->menu == i /*&& bar->state != BATTLE_BAR_DEFAULT_STATE_NONE*/)
			theme.colors[THEME_COLOR_NORMAL] = BATTLE_THEME(bt)->colors[THEME_COLOR_SELECTED];
		else
			theme.colors[THEME_COLOR_NORMAL] = BATTLE_THEME(bt)->colors[THEME_COLOR_NORMAL];

		align(buttons[i].align,
		    &buttons[i].label.x, &buttons[i].label.y, buttons[i].w, buttons[i].h,
		    bx, by, bw, bh);
		label_draw(&buttons[i].label);
	}
}

/*
 * This function is called only in the first level of the bar menu: selecting
 * one of the Attack, Magic, Item and Special items.
 */
static void
handle_keydown_menu(struct battle_bar_default *bar, struct battle *bt, const union event *ev)
{
	(void)bt;

	switch (ev->key.key) {
	case KEY_UP:
		bar->menu = BATTLE_BAR_DEFAULT_MENU_ATTACK;
		break;
	case KEY_RIGHT:
		bar->menu = BATTLE_BAR_DEFAULT_MENU_MAGIC;
		break;
	case KEY_DOWN:
		bar->menu = BATTLE_BAR_DEFAULT_MENU_ITEM;
		break;
	case KEY_LEFT:
		bar->menu = BATTLE_BAR_DEFAULT_MENU_SPECIAL;
		break;
	case KEY_ENTER:
		/*
		 * At this step, attack does not require opening the sub menu so
		 * we change selection state immediately if needed.
		 */
		switch (bar->menu) {
		case BATTLE_BAR_DEFAULT_MENU_ATTACK:
			switch_selection_attack(bt);
			break;
		case BATTLE_BAR_DEFAULT_MENU_ITEM:
			battle_bar_default_open_item(bar, bt);
			break;
		case BATTLE_BAR_DEFAULT_MENU_MAGIC:
			battle_bar_default_open_magic(bar, bt, bt->order_cur->ch);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

/*
 * This function is called when we're selecting a submenu entry from Items
 * and Magic.
 */
static void
handle_keydown_grid(struct battle_bar_default *bar, struct battle *bt, const union event *ev)
{
	/* Go back to main menu if I press escape. */
	if (ev->key.key == KEY_ESCAPE) {
		gridmenu_reset(&bar->sub_grid);
		bar->state = BATTLE_BAR_DEFAULT_STATE_MENU;
		return;
	}

	gridmenu_handle(&bar->sub_grid, ev);

	if (bar->sub_grid.state == GRIDMENU_STATE_ACTIVATED) {
		gridmenu_reset(&bar->sub_grid);

		switch (bar->menu) {
		case BATTLE_BAR_DEFAULT_MENU_MAGIC:
			switch_selection_spell(bar, bt);
			break;
		case BATTLE_BAR_DEFAULT_MENU_ITEM:
			switch_selection_item(bt);
			break;
		default:
			break;
		}
	}
}

static void
handle_keydown(struct battle_bar_default *bar, struct battle *bt, const union event *ev)
{
	assert(ev->type == EVENT_KEYDOWN);

	static void (*handlers[])(struct battle_bar_default *, struct battle *, const union event *) = {
		[BATTLE_BAR_DEFAULT_STATE_MENU] = handle_keydown_menu,
		[BATTLE_BAR_DEFAULT_STATE_GRID] = handle_keydown_grid
	};

	handlers[bar->state](bar, bt, ev);
}

#if 0

static void
handle_clickdown(struct battle_bar_default *bar, struct battle *bt, const union event *ev)
{
	(void)bar;
	(void)bt;
	(void)ev;
	assert(ev->type == EVENT_CLICKDOWN);

	switch (bar->state) {
	case BATTLE_BAR_DEFAULT_STATE_MENU:
		/* We are selecting a main menu entry. */
		/* TODO: implement click here too. */
		break;
	case BATTLE_BAR_DEFAULT_STATE_SUB:
		/* We are in the sub menu (objects/spells). */
		gridmenu_handle(&bar->sub_grid, ev);

		if (bar->sub_grid.state == GRIDMENU_STATE_ACTIVATED)
	default:
		break;
	}

	return 0;
}

#endif

static void
init_gridmenu(struct battle_bar_default *bar, const struct battle *bt)
{
	bar->sub_grid.x = bar->x;
	bar->sub_grid.y = bar->menu_frame.y;
	bar->sub_grid.w = bar->status_frame.w;
	bar->sub_grid.h = bar->h;
	bar->sub_grid.theme = bt->theme;
	bar->sub_grid.nrows = 3;
	bar->sub_grid.ncols = 4;

	memset(bar->sub_grid.menu, 0, sizeof (bar->sub_grid.menu));
}

static void
start(struct battle_bar *bar, struct battle *bt)
{
	(void)bt;

	battle_bar_default_start(bar->data);
}

static void
select(struct battle_bar *bar, struct battle *bt, const struct selection *sel)
{
	battle_bar_default_select(bar->data, bt, sel);
}

static void
handle(struct battle_bar *bar, struct battle *bt, const union event *ev)
{
	battle_bar_default_handle(bar->data, bt, ev);
}

static void
draw(const struct battle_bar *bar, const struct battle *bt)
{
	battle_bar_default_draw(bar->data, bt);
}

static void
finish(struct battle_bar *bar, struct battle *bt)
{
	(void)bt;

	battle_bar_default_finish(bar->data);
	free(bar->data);
}

void
battle_bar_default_positionate(struct battle_bar_default *bar, const struct battle *bt)
{
	assert(bar);
	assert(bt);

	bar->w = window.w;
	bar->h = window.h * 0.12;
	bar->x = 0;
	bar->y = window.h - bar->h;

	/* Menu in the middle of the bar (take 20%). */
	bar->menu_frame.w = bar->w * 0.2;
	bar->menu_frame.h = bar->h;
	bar->menu_frame.x = bar->x + (bar->w / 2) - (bar->menu_frame.w / 2);
	bar->menu_frame.y = window.h - bar->h;
	bar->menu_frame.theme = bt->theme;

	/* Status bar on the right. */
	bar->status_frame.x = bar->menu_frame.x + bar->menu_frame.w;
	bar->status_frame.y = bar->menu_frame.y;
	bar->status_frame.w = (bar->w - bar->menu_frame.w) / 2;
	bar->status_frame.h = bar->h;
	bar->status_frame.theme = bt->theme;
}

void
battle_bar_default_open_magic(struct battle_bar_default *bar, const struct battle *bt, struct character *ch)
{
	assert(bar);
	assert(bt);
	assert(ch);

	init_gridmenu(bar, bt);

	for (size_t i = 0; i < CHARACTER_SPELL_MAX; ++i)
		if (ch->spells[i])
			bar->sub_grid.menu[i] = ch->spells[i]->name;

	gridmenu_repaint(&bar->sub_grid);

	bar->state = BATTLE_BAR_DEFAULT_STATE_GRID;
}

void
battle_bar_default_open_item(struct battle_bar_default *bar, const struct battle *bt)
{
	assert(bar);
	assert(bt);

	init_gridmenu(bar, bt);

	for (size_t i = 0; i < INVENTORY_ITEM_MAX; ++i) {
		if (bt->inventory->items[i].item) {
			snprintf(bar->sub_items[i], sizeof (bar->sub_items[i]), "%-16s %u",
			    bt->inventory->items[i].item->name, bt->inventory->items[i].amount);
			bar->sub_grid.menu[i] = bar->sub_items[i];
		}
	}

	gridmenu_repaint(&bar->sub_grid);

	bar->state = BATTLE_BAR_DEFAULT_STATE_GRID;
}

void
battle_bar_default_start(struct battle_bar_default *bar)
{
	assert(bar);

	gridmenu_reset(&bar->sub_grid);

	bar->menu = BATTLE_BAR_DEFAULT_MENU_ATTACK;
	bar->state = BATTLE_BAR_DEFAULT_STATE_MENU;
}

/*
 * Apply the battle selection for the current menu item. This function is called
 * from the battle-state-selection state when the user validated the selection.
 */
void
battle_bar_default_select(struct battle_bar_default *bar, struct battle *bt, const struct selection *sel)
{
	assert(bar);
	assert(bt);
	assert(sel);

	static void (*validate[])(struct battle_bar_default *, struct battle *, const struct selection *) = {
		[BATTLE_BAR_DEFAULT_MENU_ATTACK] = validate_attack,
		[BATTLE_BAR_DEFAULT_MENU_ITEM] = validate_item,
		[BATTLE_BAR_DEFAULT_MENU_MAGIC] = validate_magic,
		[BATTLE_BAR_DEFAULT_MENU_SPECIAL] = NULL
	};

	if (validate[bar->menu])
		validate[bar->menu](bar, bt, sel);
}

void
battle_bar_default_handle(struct battle_bar_default *bar, struct battle *bt, const union event *ev)
{
	assert(bar);
	assert(bt);
	assert(ev);

	static void (*handlers[])(struct battle_bar_default *, struct battle *, const union event *) = {
		[EVENT_KEYDOWN] = handle_keydown,
		[EVENT_NUM] = NULL
	};

	if (handlers[ev->type])
		handlers[ev->type](bar, bt, ev);
}

void
battle_bar_default_draw(const struct battle_bar_default *bar, const struct battle *bt)
{
	assert(bar);
	assert(bt);

	draw_status(bar, bt);
	draw_menu(bar, bt);

	if (bar->state == BATTLE_BAR_DEFAULT_STATE_GRID) {
		switch (bar->menu) {
		case BATTLE_BAR_DEFAULT_MENU_MAGIC:
			draw_spell_help(bar, bt);
			break;
		case BATTLE_BAR_DEFAULT_MENU_ITEM:
			draw_item_help(bar, bt);
			break;
		default:
			break;
		}
	}

	/* Sub menu is only shown if state is set to it. */
	if (bar->state == BATTLE_BAR_DEFAULT_STATE_GRID)
		gridmenu_draw(&bar->sub_grid);
}

void
battle_bar_default_finish(struct battle_bar_default *bar)
{
	assert(bar);

	gridmenu_finish(&bar->sub_grid);

	memset(bar, 0, sizeof (*bar));
}

void
battle_bar_default(struct battle *bt)
{
	assert(bt);

	struct self *self;

	self = alloc_new0(sizeof (*self));
	self->bar.data = self;
	self->bar.start = start;
	self->bar.select = select;
	self->bar.handle = handle;
	self->bar.draw = draw;
	self->bar.finish = finish;

	battle_bar_default_positionate(&self->data, bt);

	bt->bar = &self->bar;
}
