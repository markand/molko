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

#include <mlk/core/alloc.h>
#include <mlk/core/event.h>
#include <mlk/core/font.h>
#include <mlk/core/sprite.h>
#include <mlk/core/trace.h>
#include <mlk/core/util.h>
#include <mlk/core/window.h>

#include <mlk/ui/align.h>
#include <mlk/ui/theme.h>

#include "battle-bar-default.h"
#include "battle-bar.h"
#include "battle-state-item.h"
#include "battle-state-selection.h"
#include "battle.h"
#include "character.h"
#include "inventory.h"
#include "item.h"
#include "spell.h"

#define THEME(bar) ((bar)->theme ? (bar)->theme : theme_default())

struct geo {
	int x, y;
	unsigned int w, h;
};

static inline void
dimensions(struct geo geo[2], const struct battle_bar_default *bar)
{
	/* 0 == main menu */
	geo[0].w = bar->w * 0.2;
	geo[0].h = bar->h;
	geo[0].x = bar->x + (bar->w / 2) - (geo[0].w / 2);
	geo[0].y = window.h - bar->h;

	/* 1 == status frame */
	geo[1].x = geo[0].x + geo[0].w;
	geo[1].y = geo[0].y;
	geo[1].w = (bar->w - geo[0].w) / 2;
	geo[1].h = bar->h;
}

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
		target = bt->enemies[sel->index_character]->ch;
	else
		target = bt->team[sel->index_character]->ch;

	battle_attack(bt, battle_current(bt)->ch, target);
}

static void
validate_magic(struct battle_bar_default *bar, struct battle *bt, const struct selection *sel)
{
	struct character *source = battle_current(bt)->ch;
	const struct spell *spell = source->spells[bar->grid.selected];

	battle_cast(bt, source, spell, sel);
}

static void
validate_item(struct battle_bar_default *bar, struct battle *bt, const struct selection *sel)
{
	struct inventory_slot *slot;
	struct battle_entity *source, *target;

	if (bar->grid.selected >= INVENTORY_ITEM_MAX)
		return;
	if (!(slot = &bt->inventory->items[bar->grid.selected]))
		return;

	source = battle_current(bt);
	target = sel->index_side == 0
		? bt->enemies[sel->index_character]
		: bt->team[sel->index_character];

	/* TODO: battle_use? */
	battle_state_item(bt, source, target, slot);
}

/*
 * The following functions are used to switch to the battle selection state
 * using the appropriate selector algorithm. For example, an item can only be
 * used on a unique target while a spell can have multiple choices.
 */

static void
switch_selection_attack(struct battle_bar_default *bar, struct battle *bt)
{
	struct selection sel = {
		.allowed_kinds = SELECTION_KIND_ONE,
		.allowed_sides = SELECTION_SIDE_ENEMY,
		.index_side = 0
	};

	/* Disable handling anymore. */
	bar->state = BATTLE_BAR_DEFAULT_STATE_NONE;

	selection_first(&sel, bt);
	battle_state_selection(bt, &sel);
}

static void
switch_selection_spell(struct battle_bar_default *bar, struct battle *bt)
{
	const struct character *ch = battle_current(bt)->ch;
	const struct spell *sp = ch->spells[bar->grid.selected];
	struct selection sel = {0};

	if (bar->grid.selected > CHARACTER_SPELL_MAX)
		return;
	if (!(sp = ch->spells[bar->grid.selected]) || sp->mp > (unsigned int)ch->mp)
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
		.index_character = battle_index(bt)
	};

	battle_state_selection(bt, &slt);
}

/*
 * The following functions actually draw the bar and their components depending
 * on the current selected menu.
 */

static void
draw_help(const struct battle_bar_default *bar, const char *what)
{
	struct label label = {0};
	unsigned int lw = 0, lh = 0;

	label.flags = LABEL_FLAGS_SHADOW;
	label.text = what;
	label_query(&label, &lw, &lh);
	label.x = bar->grid.x + (bar->grid.w / 2) - (lw / 2);
	label.y = bar->grid.y - lh - THEME(bar)->padding;
	label_draw(&label);
}

static void
draw_spell_help(const struct battle_bar_default *bar, const struct battle *bt)
{
	const struct character *ch = battle_current(bt)->ch;
	const struct spell *sp;

	if (bar->grid.selected >= CHARACTER_SPELL_MAX)
		return;
	if (!(sp = ch->spells[bar->grid.selected]))
		return;

	draw_help(bar, sp->description);
}

static void
draw_item_help(const struct battle_bar_default *bar, const struct battle *bt)
{
	const struct inventory_slot *slot;

	if (bar->grid.selected >= INVENTORY_ITEM_MAX)
		return;

	slot = &bt->inventory->items[bar->grid.selected];

	if (!slot->item)
		return;

	draw_help(bar, slot->item->description);
}

static void
draw_status_character_stats(const struct battle_bar_default *bar,
                            const struct character *ch,
                            int x,
                            int y,
                            unsigned int h)
{
	const struct theme *theme = THEME(bar);
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

	/* Name. */
	snprintf(line, sizeof (line), "%s", ch->name);
	label_query(&label, &lw, &lh);
	label.x = x + theme->padding;
	label.y = y + spacing;
	label_draw(&label);

	/* HP. */
	snprintf(line, sizeof (line), "%d/%u", ch->hp, ch->hpmax);
	label_query(&label, &lw, &lh);
	label.x = x + theme->padding;
	label.y = label.y + lh + spacing;
	label_draw(&label);

	/* MP. */
	snprintf(line, sizeof (line), "%d/%u", ch->mp, ch->mpmax);
	label_query(&label, &lw, &lh);
	label.x = x + theme->padding;
	label.y = label.y + lh + spacing;
	label_draw(&label);

	/* Status. */
	/* TODO: list all status. */
}

static void
draw_status_character(const struct battle_bar_default *bar,
                      const struct battle *bt,
                      const struct character *ch,
                      const struct geo *geo,
                      unsigned int index)
{
	int x, y;
	unsigned int w, h;

	/* Compute bounding box for rendering. */
	w = geo->w / bt->teamsz;
	h = geo->h;
	x = geo->x + (index * w);
	y = geo->y;

	draw_status_character_stats(bar, ch, x, y, h);
}

static void
draw_status_characters(const struct battle_bar_default *bar,
                       const struct battle *bt,
                       const struct geo *geo)
{
	const struct battle_entity *et;
	unsigned int index = 0;

	BATTLE_TEAM_FOREACH(bt, et) {
		if (character_ok(et->ch))
			draw_status_character(bar, bt, et->ch, geo, index);

		++index;
	}
}

static void
draw_status(const struct battle_bar_default *bar, const struct battle *bt, const struct geo *geo)
{
	frame_draw(&(const struct frame) {
		.x = geo->x,
		.y = geo->y,
		.w = geo->w,
		.h = geo->h
	});

	draw_status_characters(bar, bt, geo);
}

static void
draw_menu(const struct battle_bar_default *bar, const struct geo *geo)
{
	struct {
		unsigned int w, h;
		enum align align;
		struct label label;
	} buttons[] = {
		{
			.align = ALIGN_TOP,
			.label = {
				.text = "Attack",
				.flags = LABEL_FLAGS_SHADOW
			}
		},
		{
			.align = ALIGN_RIGHT,
			.label = {
				.text = "Magic",
				.flags = LABEL_FLAGS_SHADOW
			}
		},
		{
			.align = ALIGN_BOTTOM,
			.label = {
				.text = "Objects",
				.flags = LABEL_FLAGS_SHADOW
			}
		},
		{
			.align = ALIGN_LEFT,
			.label = {
				.text = "Special",
				.flags = LABEL_FLAGS_SHADOW
			}
		}
	};

	const struct theme *theme = THEME(bar);
	int bx, by;
	unsigned int bw, bh;

	/* Compute bounding box with margins removed. */
	bx = geo->x + theme->padding;
	by = geo->y + theme->padding;
	bw = geo->w - theme->padding * 2;
	bh = geo->h - theme->padding * 2;

	/* Draw menu frame. */
	frame_draw(&(const struct frame) {
		.x = geo->x,
		.y = geo->y,
		.w = geo->w,
		.h = geo->h
	});

	for (size_t i = 0; i < UTIL_SIZE(buttons); ++i) {
		buttons[i].label.theme = theme;

		label_query(&buttons[i].label, &buttons[i].w, &buttons[i].h);

		/* Change theme if it's selected. */
		if ((size_t)bar->menu == i)
			buttons[i].label.flags |=  LABEL_FLAGS_SELECTED;
		else
			buttons[i].label.flags &= ~LABEL_FLAGS_SELECTED;

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
			switch_selection_attack(bar, bt);
			break;
		case BATTLE_BAR_DEFAULT_MENU_ITEM:
			battle_bar_default_open_item(bar, bt);
			break;
		case BATTLE_BAR_DEFAULT_MENU_MAGIC:
			battle_bar_default_open_magic(bar, bt, battle_current(bt)->ch);
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
	if (ev->key.key == KEY_ESCAPE)
		bar->state = BATTLE_BAR_DEFAULT_STATE_MENU;
	else if (gridmenu_handle(&bar->grid, ev)) {
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

	if (handlers[bar->state])
		handlers[bar->state](bar, bt, ev);
}

#if 0

static void
handle_clickdown(struct battle_bar_default *bar, struct battle *bt, const union event *ev)
{
	assert(ev->type == EVENT_CLICKDOWN);

	(void)bar;
	(void)bt;

	switch (bar->state) {
	case BATTLE_BAR_DEFAULT_STATE_MENU:
		/* We are selecting a main menu entry. */
		/* TODO: implement click here too. */
		break;
	case BATTLE_BAR_DEFAULT_STATE_SUB:
		/* We are in the sub menu (objects/spells). */
		if (bar->sub_grid.state == GRIDMENU_STATE_ACTIVATED)
	default:
		break;
	}

	return 0;
}

#endif

static void
self_start(struct battle_bar *bar, struct battle *bt)
{
	(void)bt;

	battle_bar_default_start(bar->data);
}

static void
self_select(struct battle_bar *bar, struct battle *bt, const struct selection *sel)
{
	battle_bar_default_select(bar->data, bt, sel);
}

static void
self_handle(struct battle_bar *bar, struct battle *bt, const union event *ev)
{
	battle_bar_default_handle(bar->data, bt, ev);
}

static void
self_draw(const struct battle_bar *bar, const struct battle *bt)
{
	battle_bar_default_draw(bar->data, bt);
}

void
battle_bar_default_init(struct battle_bar_default *bar)
{
	assert(bar);

	struct geo geo[2];

	memset(bar, 0, sizeof (*bar));

	bar->w = window.w;
	bar->h = window.h * 0.12;
	bar->x = 0;
	bar->y = window.h - bar->h;

	dimensions(geo, bar);

	gridmenu_init(&bar->grid, 2, 2, NULL, 0);
	gridmenu_resize(&bar->grid, bar->x, geo[0].y, geo[1].w, bar->h);
	bar->grid.theme = bar->theme;
}

void
battle_bar_default_open_magic(struct battle_bar_default *bar, const struct battle *bt, struct character *ch)
{
	assert(bar);
	assert(bt);
	assert(ch);

	(void)bt;

	bar->items = alloc_rearray0(bar->items, bar->itemsz,
	    CHARACTER_SPELL_MAX, sizeof (*bar->items));
	bar->itemsz = CHARACTER_SPELL_MAX;
	bar->state = BATTLE_BAR_DEFAULT_STATE_GRID;

	for (size_t i = 0; i < CHARACTER_SPELL_MAX; ++i)
		if (ch->spells[i])
			bar->items[i] = ch->spells[i]->name;

	bar->grid.items = bar->items;
	bar->grid.itemsz = bar->itemsz;
}

void
battle_bar_default_open_item(struct battle_bar_default *bar, const struct battle *bt)
{
	assert(bar);
	assert(bt);

	/* TODO: not implemented yet. */
	(void)bar;
	(void)bt;
#if 0
	for (size_t i = 0; i < INVENTORY_ITEM_MAX; ++i) {
		if (bt->inventory->items[i].item) {
			snprintf(bar->sub_items[i], sizeof (bar->sub_items[i]), "%-16s %u",
			    bt->inventory->items[i].item->name, bt->inventory->items[i].amount);
			bar->sub_grid.menu[i] = bar->sub_items[i];
		}
	}

	bar->state = BATTLE_BAR_DEFAULT_STATE_GRID;
#endif
}

void
battle_bar_default_start(struct battle_bar_default *bar)
{
	assert(bar);

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
		[BATTLE_BAR_DEFAULT_MENU_ATTACK]        = validate_attack,
		[BATTLE_BAR_DEFAULT_MENU_ITEM]          = validate_item,
		[BATTLE_BAR_DEFAULT_MENU_MAGIC]         = validate_magic,
		[BATTLE_BAR_DEFAULT_MENU_SPECIAL]       = NULL
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

	struct geo geo[2];

	dimensions(geo, bar);
	draw_menu(bar, &geo[0]);
	draw_status(bar, bt, &geo[1]);

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
		gridmenu_draw(&bar->grid);
}

void
battle_bar_default_finish(struct battle_bar_default *bar)
{
	assert(bar);

	free(bar->items);
	memset(bar, 0, sizeof (*bar));
}

void
battle_bar_default(struct battle_bar_default *self, struct battle_bar *bar)
{
	assert(self);
	assert(bar);

	memset(bar, 0, sizeof (*bar));

	bar->data = self;
	bar->start = self_start;
	bar->select = self_select;
	bar->handle = self_handle;
	bar->draw = self_draw;
}
