/*
 * equipment.h -- character equipment
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

#ifndef MLK_RPG_EQUIPMENT_H
#define MLK_RPG_EQUIPMENT_H

#include <mlk/core/core.h>

struct character;
struct mlk_texture;

enum equipment_type {
	/* Attack weapons. */
	EQUIPMENT_TYPE_AXE,
	EQUIPMENT_TYPE_BOW,
	EQUIPMENT_TYPE_CROSSBOW,
	EQUIPMENT_TYPE_DAGGER,
	EQUIPMENT_TYPE_HAMMER,
	EQUIPMENT_TYPE_SPIKE,
	EQUIPMENT_TYPE_SWORD,
	EQUIPMENT_TYPE_WAND,

	/* Defense equipment. */
	EQUIPMENT_TYPE_GLOVES,
	EQUIPMENT_TYPE_HELMET,
	EQUIPMENT_TYPE_SHIELD,
	EQUIPMENT_TYPE_TOP,
	EQUIPMENT_TYPE_TROUSERS,
};

struct equipment {
	const char *name;
	const char *description;
	unsigned int price;
	enum equipment_type type;
	struct mlk_texture *icon;

	void (*equip)(const struct equipment *, struct character *);
};

MLK_CORE_BEGIN_DECLS

int
equipment_ok(const struct equipment *);

void
equipment_equip(const struct equipment *, struct character *);

MLK_CORE_END_DECLS

#endif /* !MLK_RPG_EQUIPMENT_H */
