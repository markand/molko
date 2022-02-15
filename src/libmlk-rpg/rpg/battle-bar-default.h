/*
 * battle-bar-default.h -- default battle status bar and menu implementation
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

#ifndef MLK_RPG_BATTLE_BAR_DEFAULT_H
#define MLK_RPG_BATTLE_BAR_DEFAULT_H

#include <core/core.h>

#include <ui/frame.h>
#include <ui/gridmenu.h>

struct battle;
struct character;
struct selection;

union event;

enum battle_bar_default_menu {
	BATTLE_BAR_DEFAULT_MENU_ATTACK,
	BATTLE_BAR_DEFAULT_MENU_MAGIC,
	BATTLE_BAR_DEFAULT_MENU_ITEM,
	BATTLE_BAR_DEFAULT_MENU_SPECIAL
};

enum battle_bar_default_state {
	BATTLE_BAR_DEFAULT_STATE_MENU,
	BATTLE_BAR_DEFAULT_STATE_GRID
};

struct battle_bar_default {
	int x;
	int y;
	unsigned int w;
	unsigned int h;
	enum battle_bar_default_state state;

	/* Right status frame. */
	struct frame status_frame;

	/* Main menu selection. */
	struct frame menu_frame;
	enum battle_bar_default_menu menu;

	/* Sub menu selection (spells/objects). */
	char sub_items[GRIDMENU_ENTRY_MAX][128];
	struct gridmenu sub_grid;
};

CORE_BEGIN_DECLS

void
battle_bar_default_positionate(struct battle_bar_default *, const struct battle *);

void
battle_bar_default_open_magic(struct battle_bar_default *, const struct battle *, struct character *);

void
battle_bar_default_open_item(struct battle_bar_default *, const struct battle *);

void
battle_bar_default_start(struct battle_bar_default *);

void
battle_bar_default_select(struct battle_bar_default *, struct battle *, const struct selection *);

void
battle_bar_default_handle(struct battle_bar_default *bar, struct battle *bt, const union event *);

void
battle_bar_default_draw(const struct battle_bar_default *, const struct battle *);

void
battle_bar_default_finish(struct battle_bar_default *);

void
battle_bar_default(struct battle *);

CORE_END_DECLS

#endif /* !MLK_RPG_BATTLE_BAR_DEFAULT_H */
