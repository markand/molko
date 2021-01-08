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

#ifndef MOLKO_RPG_BATTLE_BAR_H
#define MOLKO_RPG_BATTLE_BAR_H

/*
 * The bar is split into three individual pieces.
 *
 * +------------+--------+------------+
 * | Grid menu  |  Menu  |   Status   |
 * +------------+--------+------------+
 *
 *
 * The left grid menu is only shown when member field state is set to \ref
 * BATTLE_BAR_STATE_SUB, it is usually opened when user select a magic or an
 * object.
 *
 * The menu in the middle is the main selection and contains the enumeration
 * battle_bar_menu using top, right, bottom left edges for short selection.
 *
 * The last part on the right is the status pane where the team characters stats
 * are listed.
 *
 * By itself, the bar never modify the battle object, it is the responsability
 * of the battle object or the battle state to change the bar object and to
 * track change on events. As such, the battle object is always passed as const
 * parameter.
 */

#include <stdbool.h>

#include <ui/frame.h>
#include <ui/gridmenu.h>

struct battle;
struct character;

union event;

enum battle_bar_menu {
	BATTLE_BAR_MENU_ATTACK  = 0,
	BATTLE_BAR_MENU_MAGIC   = 1,
	BATTLE_BAR_MENU_OBJECTS = 2,
	BATTLE_BAR_MENU_SPECIAL = 3
};

enum battle_bar_state {
	BATTLE_BAR_STATE_NONE,
	BATTLE_BAR_STATE_MENU,
	BATTLE_BAR_STATE_SUB
};

struct battle_bar {
	int x;
	int y;
	unsigned int w;
	unsigned int h;
	enum battle_bar_state state;

	/* Right status frame. */
	struct frame status_frame;

	/* Main menu selection. */
	struct frame menu_frame;
	enum battle_bar_menu menu;

	/* Sub menu selection (spells/objects). */
	char sub_items[GRIDMENU_ENTRY_MAX][128];
	struct gridmenu sub_grid;
};

void
battle_bar_positionate(struct battle_bar *bar, const struct battle *bt);

bool
battle_bar_handle(struct battle_bar *bar,
                  const struct battle *bt,
                  const union event *ev);

void
battle_bar_reset(struct battle_bar *bar);

void
battle_bar_open_menu(struct battle_bar *bar);

void
battle_bar_open_spells(struct battle_bar *bar, const struct battle *bt, struct character *ch);

void
battle_bar_open_items(struct battle_bar *bar, const struct battle *bt);

void
battle_bar_draw(const struct battle_bar *bar, const struct battle *bt);

void
battle_bar_finish(struct battle_bar *bar);

#endif /* !MOLKO_RPG_BATTLE_BAR_H */
