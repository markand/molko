/*
 * inventory_dialog.h -- dialog for items
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

#ifndef INVENTORY_DIALOG_H
#define INVENTORY_DIALOG_H

#include <ui/button.h>
#include <ui/frame.h>
#include <ui/label.h>

union event;

struct inventory;
struct theme;

/**
 * \brief Inventory dialog state.
 */
enum inventory_dialog_state {
	INVENTORY_DIALOG_NONE,
	INVENTORY_DIALOG_SHOWING
};

/**
 * \brief Inventory dialog.
 */
struct inventory_dialog {
	int x;                                  /*!< (+) Position in x. */
	int y;                                  /*!< (+) Position in y. */
	struct inventory *inv;                  /*!< (+&) Inventory to use. */
	struct theme *theme;                    /*!< (+&?) Theme to use. */
	struct button bsort;                    /*!< (-) Button sort. */
	struct frame fgrid;                     /*!< (-) Grid frame. */
	struct frame fname;                     /*!< (-) Frame for name. */
	struct frame fdesc;                     /*!< (-) Frame for description. */
	struct label lname;                     /*!< (-) Label for name. */
	struct label ldesc;                     /*!< (-) Label for description. */
	enum inventory_dialog_state state;      /*!< (-) Current dialog state. */
	unsigned int selrow;                    /*!< (-) Current selected row. */
	unsigned int selcol;                    /*!< (-) Current selected column. */
};

void
inventory_dialog_open(struct inventory_dialog *dlg);

void
inventory_dialog_handle(struct inventory_dialog *dlg, const union event *event);

void
inventory_dialog_update(struct inventory_dialog *dlg, unsigned int ticks);

void
inventory_dialog_move(struct inventory_dialog *dlg, int x, int y);

void
inventory_dialog_draw(struct inventory_dialog *dlg);

void
inventory_dialog_finish(struct inventory_dialog *dlg);

#endif /* !INVENTORY_DIALOG_H */
