/*
 * inventory.c -- item inventory
 *
 * Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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

#include "inventory.h"

static struct inventory_slot *
find(struct inventory *iv, const struct item *item)
{
	for (size_t i = 0; i < INVENTORY_ITEM_MAX; ++i)
		if (iv->items[i].item == item)
			return &iv->items[i];

	return NULL;
}

int
inventory_add(struct inventory *iv, const struct item *item, unsigned int amount)
{
	assert(iv);
	assert(item);

	struct inventory_slot *slot;

	/* Find one existing, otherwise find one empty. */
	if (!(slot = find(iv, item)))
		slot = find(iv, NULL);

	if (!slot)
		return 0;

	slot->item = item;
	slot->amount += amount;

	return -1;
}

void
inventory_consume(struct inventory *iv, const struct item *item, unsigned int amount)
{
	assert(iv);
	assert(item);

	struct inventory_slot *slot;

	if ((slot = find(iv, item))) {
		slot->amount -= amount;

		if (slot->amount == 0)
			slot->item = NULL;
	}
}
