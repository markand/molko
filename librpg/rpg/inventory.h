/*
 * inventory.h -- inventory of items
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

#ifndef MOLKO_INVENTORY_H
#define MOLKO_INVENTORY_H

/**
 * \file inventory.h
 * \brief Inventory of items.
 */

/**
 * \brief Maximum number of rows.
 */
#define INVENTORY_ROWS_MAX      3

/**
 * \brief Maximum number of columns.
 */
#define INVENTORY_COLS_MAX      10

struct item;

/**
 * \brief Inventory slot.
 *
 * This structure describe a 'cell' into the inventory. It references a item
 * and has a given amount of it.
 */
struct inventory_slot {
	struct item *item;      /*!< (+&?) Pointer to the item. */
	unsigned int amount;    /*!< (-) Number of items in this slot. */
};

/**
 * \brief Inventory structure.
 */
struct inventory {
	/**
	 * (-) Grid of objects.
	 */
	struct inventory_slot items[INVENTORY_ROWS_MAX][INVENTORY_COLS_MAX];
};

/**
 * Try to push as much as possible the given item.
 *
 * \pre iv != NULL
 * \pre item != NULL
 * \param iv the inventory
 * \param item the item to reference
 * \param amount the desired amount
 * \return 0 if all items were pushed or the number left otherwise
 */
unsigned int
inventory_push(struct inventory *iv, struct item *item, unsigned int amount);

/**
 * Sort the inventory.
 *
 * \pre iv != NULL
 * \pre item != NULL
 * \param iv the inventory
 */
void
inventory_sort(struct inventory *iv);

/**
 * Clears the inventory.
 *
 * \pre iv != NULL
 * \param iv the inventory
 */
void
inventory_clear(struct inventory *iv);

#endif /* !MOLKO_INVENTORY_H */
