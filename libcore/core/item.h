/*
 * item.h -- inventory items
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

#ifndef MOLKO_ITEM_H
#define MOLKO_ITEM_H

/**
 * \file item.h
 * \brief Inventory items.
 */

#include <stdbool.h>

/**
 * \brief Maximum count of an item into a stack.
 */
#define ITEM_STACK_MAX  64

struct character;
struct texture;

/**
 * \brief Inventory items.
 */
struct item {
	const char *name;               /*!< (+) Name of item. */
	const char *summary;            /*!< (+) Summary description. */
	struct texture *icon;           /*!< (+&) Icon to show. */
	unsigned int stackable;         /*!< (+) Stack count allowed. */

	/**
	 * (+) Execute the action for this character.
	 */
	void (*exec)(const struct item *, struct character *);

	/**
	 * (+?) Tells if the item can be used in this context.
	 */
	bool (*allowed)(const struct item *, const struct character *);
};

#endif /* !MOLKO_ITEM_H */
