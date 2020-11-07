/*
 * battle-bar.h -- battle status bar and menu
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
#include <string.h>

#include <core/event.h>
#include <core/font.h>
#include <core/window.h>
#include <core/util.h>

#include <ui/align.h>
#include <ui/theme.h>

#include "battle.h"
#include "battle-bar.h"
#include "character.h"
#include "spell.h"

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
draw_status_character(const struct battle_bar *bar,
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
draw_status_characters(const struct battle_bar *bar, const struct battle *bt)
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
draw_status(const struct battle_bar *bar, const struct battle *bt)
{
	frame_draw(&bar->status_frame);
	draw_status_characters(bar, bt);
}

static void
draw_menu(const struct battle_bar *bar, const struct battle *bt)
{
	static struct {
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

	for (size_t i = 0; i < NELEM(buttons); ++i) {
		buttons[i].label.theme = &theme;

		label_query(&buttons[i].label, &buttons[i].w, &buttons[i].h);

		/* Change theme if it's selected. */
		if ((size_t)bar->menu == i && bar->state != BATTLE_BAR_STATE_NONE)
			theme.colors[THEME_COLOR_NORMAL] = BATTLE_THEME(bt)->colors[THEME_COLOR_SELECTED];
		else
			theme.colors[THEME_COLOR_NORMAL] = BATTLE_THEME(bt)->colors[THEME_COLOR_NORMAL];

		align(buttons[i].align,
		    &buttons[i].label.x, &buttons[i].label.y, buttons[i].w, buttons[i].h,
		    bx, by, bw, bh);
		label_draw(&buttons[i].label);
	}
}

static void
draw_sub(const struct battle_bar *bar)
{
	gridmenu_draw(&bar->sub_grid);
}

static bool
handle_keydown(struct battle_bar *bar, const union event *ev)
{
	assert(ev->type == EVENT_KEYDOWN);

	switch (bar->state) {
	case BATTLE_BAR_STATE_MENU:
		/* We are selecting a main menu entry. */
		switch (ev->key.key) {
		case KEY_UP:
			bar->menu = BATTLE_BAR_MENU_ATTACK;
			break;
		case KEY_RIGHT:
			bar->menu = BATTLE_BAR_MENU_MAGIC;
			break;
		case KEY_DOWN:
			bar->menu = BATTLE_BAR_MENU_OBJECTS;
			break;
		case KEY_LEFT:
			bar->menu = BATTLE_BAR_MENU_SPECIAL;
			break;
		case KEY_ENTER:
			return true;
		default:
			break;
		}
		break;
	case BATTLE_BAR_STATE_SUB:
		/* We are in the sub menu (objects/spells). */
		gridmenu_handle(&bar->sub_grid, ev);
		return bar->sub_grid.state == GRIDMENU_STATE_ACTIVATED;
	default:
		break;
	}

	return false;
}

static bool
handle_clickdown(struct battle_bar *bar, const union event *ev)
{
	assert(ev->type == EVENT_CLICKDOWN);

	switch (bar->state) {
	case BATTLE_BAR_STATE_MENU:
		/* We are selecting a main menu entry. */
		/* TODO: implement click here too. */
		break;
	case BATTLE_BAR_STATE_SUB:
		/* We are in the sub menu (objects/spells). */
		gridmenu_handle(&bar->sub_grid, ev);
		return bar->sub_grid.state == GRIDMENU_STATE_ACTIVATED;
	default:
		break;
	}

	return false;
}

void
battle_bar_positionate(struct battle_bar *bar, const struct battle *bt)
{
	assert(bar);

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

bool
battle_bar_handle(struct battle_bar *bar, const struct battle *bt, const union event *ev)
{
	assert(bar);
	assert(bt);
	assert(ev);

	if (bar->state == BATTLE_BAR_STATE_NONE)
		return false;

	switch (ev->type) {
	case EVENT_KEYDOWN:
		return handle_keydown(bar, ev);
	case EVENT_CLICKDOWN:
		return handle_clickdown(bar, ev);
	default:
		break;
	}

	return false;
}

void
battle_bar_reset(struct battle_bar *bar)
{
	gridmenu_reset(&bar->sub_grid);

	bar->menu = BATTLE_BAR_MENU_ATTACK;
	bar->state = BATTLE_BAR_STATE_NONE;
}

void
battle_bar_open_menu(struct battle_bar *bar)
{
	bar->state = BATTLE_BAR_STATE_MENU;
	bar->menu = BATTLE_BAR_MENU_ATTACK;
}

void
battle_bar_open_spells(struct battle_bar *bar, const struct battle *bt, struct character *ch)
{
	/* Sub menu bar on the left, take same space as status. */
	bar->sub_grid.x = bar->x;
	bar->sub_grid.y = bar->menu_frame.y;
	bar->sub_grid.w = bar->status_frame.w;
	bar->sub_grid.h = bar->h;
	bar->sub_grid.theme = bt->theme;
	bar->sub_grid.nrows = 3;
	bar->sub_grid.ncols = 4;

	memset(bar->sub_grid.menu, 0, sizeof (bar->sub_grid.menu));

	for (size_t i = 0; i < CHARACTER_SPELL_MAX; ++i) {
		if (ch->spells[i])
			bar->sub_grid.menu[i] = ch->spells[i]->name;
	}

	gridmenu_repaint(&bar->sub_grid);

	bar->state = BATTLE_BAR_STATE_SUB;
}

void
battle_bar_draw(const struct battle_bar *bar, const struct battle *bt)
{
	assert(bar);
	assert(bt);

	draw_status(bar, bt);
	draw_menu(bar, bt);

	/* Sub menu is only shown if state is set to it. */
	if (bar->state == BATTLE_BAR_STATE_SUB)
		draw_sub(bar);
}

void
battle_bar_finish(struct battle_bar *bar)
{
	assert(bar);

	gridmenu_finish(&bar->sub_grid);

	memset(bar, 0, sizeof (*bar));
}
