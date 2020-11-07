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

#ifndef MOLKO_BATTLE_BAR
#define MOLKO_BATTLE_BAR

/**
 * \file battle-bar.h
 * \brief Battle status bar and menu.
 *
 * This module is a view and user input for the game battle. It is used by the
 * \ref battle.h object.
 */

#include <stdbool.h>

#include <core/plat.h>

#include <ui/frame.h>
#include <ui/gridmenu.h>

struct battle;
struct character;

union event;

/**
 * \brief Main menu selection.
 */
enum battle_bar_menu {
	BATTLE_BAR_MENU_ATTACK  = 0,    /*!< Attack. */
	BATTLE_BAR_MENU_MAGIC   = 1,    /*!< Cast a spell. */
	BATTLE_BAR_MENU_OBJECTS = 2,    /*!< Use an object*/
	BATTLE_BAR_MENU_SPECIAL = 3     /*!< Use a special character action. */
};

/**
 * \brief Bar state.
 */
enum battle_bar_state {
	BATTLE_BAR_STATE_NONE,          /*!< No state yet. */
	BATTLE_BAR_STATE_MENU,          /*!< Main menu selection. */
	BATTLE_BAR_STATE_SUB            /*!< Sub grid menu opened. */
};

/**
 * \brief Battle bar.
 *
 * The bar is split into three individual pieces.
 *
 * ```
 * +------------+--------+------------+
 * | Grid menu  |  Menu  |   Status   |
 * +------------+--------+------------+
 * ```
 *
 * The left grid menu is only shown when member field state is set to \ref
 * BATTLE_BAR_STATE_SUB, it is usually opened when user select a magic or an
 * object.
 *
 * The menu in the middle is the main selection and contains the enumeration
 * \ref battle_bar_menu using top, right, bottom left edges for short selection.
 *
 * The last part on the right is the status pane where the team characters stats
 * are listed.
 *
 * By itself, the bar never modify the battle object, it is the responsability
 * of the battle object or the battle state to change the bar object and to
 * track change on events. As such, the battle object is always passed as const
 * parameter.
 */
struct battle_bar {
	int x;                          /*!< (+) Position in x. */
	int y;                          /*!< (+) Position in y. */
	unsigned int w;                 /*!< (+) Width. */
	unsigned int h;                 /*!< (+) Height. */
	enum battle_bar_state state;    /*!< (-) Current state. */

	/* Right status frame. */
	struct frame status_frame;      /*!< (-) Right status frame. */

	/* Main menu selection. */
	struct frame menu_frame;        /*!< (-) Main menu frame. */
	enum battle_bar_menu menu;      /*!< (-) Main menu selection. */

	/* Sub menu selection (spells/objects). */
	struct gridmenu sub_grid;       /*!< (-) Sub menu object. */
};

/**
 * Repositionate every elements in the bar.
 *
 * Call this function at least once, when you have set the x, y, w and h fields
 * of the bar.
 *
 * \pre bar != NULL
 * \pre bt != NULL
 * \param bar the bar to set
 * \param bt the current battle
 */
void
battle_bar_positionate(struct battle_bar *bar, const struct battle *bt);

/**
 * Handle an event.
 *
 * Depending on the current bar state, it will either update the main menu or
 * the sub menu if opened. The function returns true if an element is considered
 * activated (usually with the return key).
 *
 * You must not ignore the return value because the battle state should change.
 *
 * \pre bar != NULL
 * \pre bt != NULL
 * \pre ev != NULL
 * \param bar this bar
 * \param bt the current battle
 * \param ev the current event
 * \return True if an element has been activated.
 */
bool
battle_bar_handle(struct battle_bar *bar,
                  const struct battle *bt,
                  const union event *ev) PLAT_NODISCARD;

/**
 * Reset the battle bar selection, state and grid.
 *
 * Call this function each time a new player is selected.
 *
 * \pre bar != NULL
 * \param bar the bar to reset
 */
void
battle_bar_reset(struct battle_bar *bar);

void
battle_bar_open_menu(struct battle_bar *bar);

/**
 * Open the view to select a spell by opening the grid menu.
 *
 * \pre bar != NULL
 * \pre character_ok(ch)
 * \param bar this bar
 * \param ch the owner of spells
 */
void
battle_bar_open_spells(struct battle_bar *bar, const struct battle *bt, struct character *ch);

/**
 * Draw the bar.
 *
 * \pre bar != NULL
 * \pre bt != NULL
 * \param bar the bar to draw
 * \param bt the current battle
 */
void
battle_bar_draw(const struct battle_bar *bar, const struct battle *bt);

/**
 * Close internal resources if necessary.
 *
 * \pre bar != NULL
 * \param bar the bar to clear
 */
void
battle_bar_finish(struct battle_bar *bar);

#endif /* !MOLKO_BATTLE_BAR */
