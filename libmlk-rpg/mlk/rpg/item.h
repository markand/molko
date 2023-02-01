/*
 * item.h -- inventory items
 *
 * Copyright (c) 2020-2023 David Demelier <markand@malikania.fr>
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

#ifndef MLK_RPG_ITEM_H
#define MLK_RPG_ITEM_H

#include <mlk/core/core.h>

struct battle;
struct character;
struct texture;

struct item {
	const char *name;
	const char *description;
	struct texture *icon;

	void (*exec_menu)(const struct item *, struct character *);

	void (*exec_battle)(const struct item *,
	                    struct battle *,
	                    struct character *,
	                    struct character *);
};

CORE_BEGIN_DECLS

void
item_exec_menu(const struct item *, struct character *);

void
item_exec_battle(const struct item *,
                 struct battle *,
                 struct character *,
                 struct character *);

CORE_END_DECLS

#endif /* !MLK_RPG_ITEM_H */
