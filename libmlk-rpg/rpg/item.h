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

#ifndef MOLKO_RPG_ITEM_H
#define MOLKO_RPG_ITEM_H

#include <stdbool.h>

struct character;
struct texture;

struct item {
	const char *name;
	const char *description;
	struct texture *icon;
	void (*exec)(const struct item *item, struct character *ch);
	bool (*allowed)(const struct item *item, const struct character *ch);
};

void
item_exec(const struct item *item, struct character *ch);

bool
item_allowed(const struct item *item, struct character *ch);

#endif /* !MOLKO_RPG_ITEM_H */
