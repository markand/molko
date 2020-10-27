/*
 * gridmenu.h -- GUI grid menu
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

#ifndef MOLKO_GRIDMENU_H
#define MOLKO_GRIDMENU_H

/**
 * \file gridmenu.h
 * \brief GUI grid menu.
 */

#include <stddef.h>

#include <core/texture.h>

#include "label.h"

/**
 * \brief Maximum number of entries.
 */
#define GRIDMENU_ENTRY_MAX (256)

struct theme;

union event;

/**
 * \brief Grid menu state.
 */
enum gridmenu_state {
	GRIDMENU_STATE_NONE,            /*!< No state yet. */
	GRIDMENU_STATE_SELECTED         /*!< An entry has been selected. */
};

/**
 * \brief Internal texture representation.
 *
 * This structure contain several information for rendering the underlying grid
 * into the screen.
 */
struct gridmenu_texture {
	int rely;		/*!< (-) View start in y. */
	unsigned int relh;      /*!< (-) Real texture height. */
	unsigned int eltw;      /*!< (-) Maximum label width */
	unsigned int elth;      /*!< (-) Maximum label height. */
	unsigned int spacev;    /*!< (-) Vertical space between labels. */
	unsigned int spaceh;    /*!< (-) Horizontal space between labels. */
	struct texture texture; /*!< (*) The texture itself. */

	/**
	 * (-) The list of labels.
	 */
	struct label labels[GRIDMENU_ENTRY_MAX];
};

/**
 * \brief Grid menu for selecting items.
 *
 * This menu offers a grid where user can specify a maximum number of columns to
 * show entries. Content is automatically paginated vertically according to the
 * current selection and the menu's length.
 *
 * It uses \ref frame.h and \ref label.h to draw elements so you may change the
 * referenced theme if you want a different style.
 *
 * This module being a bit complex uses internal data for rendering that is
 * repainted in case of event (when using \ref gridmenu_handle) but if you do
 * modify the menu, you'll have to call \ref gridmenu_repaint yourself and you
 * need to call it at least once before drawing.
 */
struct gridmenu {
	int x;                          /*!< (+) Position in x. */
	int y;                          /*!< (+) Position in y. */
	unsigned int w;                 /*!< (+) Width. */
	unsigned int h;                 /*!< (+) Height. */
	enum gridmenu_state state;      /*!< (+) Menu state. */
	size_t selected;                /*!< (+) User selection. */
	const struct theme *theme;      /*!< (+&?) Optional theme to use. */

	/**
	 * (+&?) List of entries to show.
	 */
	const char *menu[GRIDMENU_ENTRY_MAX];

	/**
	 * (+) Number of rows allowed per page.
	 */
	unsigned int nrows;

	/**
	 * (+) Number of columns allowed per page.
	 *
	 * \warning You must make sure to use a number of columns that can fit
	 *          GRIDMENU_ENTRY_MAX, in other terms
	 *          `GRIDMENU_ENTRY_MAX % ncols == 0`
	 */
	unsigned int ncols;

	/**
	 * (*) Internal grid menu texture.
	 */
	struct gridmenu_texture tex;
};

/**
 * Reset the menu->state flag.
 *
 * \pre menu != NULL
 * \param menu the menu to reset
 */
void
gridmenu_reset(struct gridmenu *menu);

/**
 * Rebuild internal texture for rendering.
 *
 * \pre menu != NULL
 * \pre GRIDMENU_ENTRY_MAX % menu->ncols == 0
 * \param menu the menu to repaint
 */
void
gridmenu_repaint(struct gridmenu *menu);

/**
 * Handle an event in the menu.
 *
 * Mouse click will test the coordinate of the mouse to check if the pointer is
 * on a menu item region but keyboard events aren't so make sure to have user
 * "focus" prior to calling this function.
 *
 * \pre menu != NULL
 * \pre ev != NULL
 * \param menu the menu to use
 * \param ev the event
 */
void
gridmenu_handle(struct gridmenu *menu, const union event *ev);

/**
 * Draw the menu.
 *
 * \pre menu != NULL && menu->nrows > 0 && menu->ncols > 0
 * \param menu the menu to draw
 */
void
gridmenu_draw(const struct gridmenu *menu);

/**
 * Close internal resources.
 *
 * \pre menu != NULL
 * \param menu the menu to close
 */
void
gridmenu_finish(struct gridmenu *menu);

#endif /* !MOLKO_GRIDMENU_H */
