/*
 * inventory.c -- inventory of items
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
#include <stddef.h>
#include <string.h>

#include "inventory.h"
#include "item.h"

static bool
can_be_used(struct inventory_slot *slot, const struct item *item)
{
	assert(item);

	/* Empty slot. */
	if (!slot->item)
		return false;

	/* Not same object. */
	if (strcmp(slot->item->name, item->name) != 0)
		return false;

	/* No space in this slot. */
	if (slot->amount >= slot->item->stackable)
		return false;

	return true;
}

static struct inventory_slot *
find(struct inventory *iv, const struct item *item)
{
	assert(iv);
	assert(item);

	struct inventory_slot *slot = NULL;

	/* First pass: find an entry with the same item. */
	for (unsigned int r = 0; r < INVENTORY_ROWS_MAX; ++r)
		for (unsigned int c = 0; c < INVENTORY_COLS_MAX; ++c)
			if (can_be_used(&iv->items[r][c], item))
				return &iv->items[r][c];

	/* Second pass: try to find an empty slot. */
	for (unsigned int r = 0; r < INVENTORY_ROWS_MAX; ++r)
		for (unsigned int c = 0; c < INVENTORY_COLS_MAX; ++c)
			if (!iv->items[r][c].item)
				return &iv->items[r][c];

	return NULL;
}

static unsigned
provide(struct inventory_slot *slot, struct item *item, unsigned int amount)
{
	assert(slot);

	unsigned int avail;

	/* The slot may be empty, make sure it contains this item. */
	slot->item = item;

	/*
	 * Example:
	 *
	 * The slot has already 10 items.
	 * The slot item is stackble up to 64 items.
	 *
	 * When pushing:
	 *
	 * 80: 54 pushed, 26 left
	 * 30: 30 pushed, 0 left.
	 */
	avail = slot->item->stackable - slot->amount;

	if (amount > avail) {
		slot->amount += avail;
		amount -= avail;
	} else {
		slot->amount += amount;
		amount = 0;
	}

	return amount;
}

unsigned int
inventory_push(struct inventory *iv, struct item *item, unsigned int amount)
{
	assert(iv);
	assert(item);

	while (amount) {
		struct inventory_slot *slot;
		unsigned int avail;

		if (!(slot = find(iv, item)))
			break;

		/* Add as much as we can in this slot. */
		amount = provide(slot, item, amount);
	}

	return amount;
}

void
inventory_clear(struct inventory *iv)
{
	assert(iv);

	memset(iv, 0, sizeof (*iv));
}
