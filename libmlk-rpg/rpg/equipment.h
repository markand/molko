/*
 * equipment.h -- character equipment
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

#ifndef MOLKO_RPG_EQUIPMENT_H
#define MOLKO_RPG_EQUIPMENT_H

/**
 * \file equipment.h
 * \brief Character equipment.
 */

#include <stdbool.h>

struct character;
struct texture;

/**
 * \brief Kind of equipment.
 *
 * There are different kind of equipments. Weapons are listed in this
 * enumeration even though a character can carry only one of them at a time. On
 * the other hand, a character is able to carry each of the defensive item.
 */
enum equipment_type {
	/* Attack weapons. */
	EQUIPMENT_TYPE_AXE,             /*!< Axe. */
	EQUIPMENT_TYPE_BOW,             /*!< Bow. */
	EQUIPMENT_TYPE_CROSSBOW,        /*!< Cross bow. */
	EQUIPMENT_TYPE_DAGGER,          /*!< Small dagger. */
	EQUIPMENT_TYPE_HAMMER,          /*!< Large hammer. */
	EQUIPMENT_TYPE_SPIKE,           /*!< Spike. */
	EQUIPMENT_TYPE_SWORD,           /*!< Sword. */
	EQUIPMENT_TYPE_WAND,            /*!< Magic wands. */

	/* Defense equipment. */
	EQUIPMENT_TYPE_GLOVES,          /*!< Hand gloves. */
	EQUIPMENT_TYPE_HELMET,          /*!< Head helmet. */
	EQUIPMENT_TYPE_SHIELD,          /*!< Shield. */
	EQUIPMENT_TYPE_TOP,             /*!< Top protection. */
	EQUIPMENT_TYPE_TROUSERS,        /*!< Trousers. */
};

/**
 * \brief Equipment structure.
 */
struct equipment {
	const char *name;               /*!< (+&) Equipment short name. */
	const char *description;        /*!< (+&) Longer description. */
	unsigned int price;             /*!< (+) Standard price. */
	enum equipment_type type;       /*!< (+) Kind of equipment. */
	struct texture *icon;           /*!< (+&) Icon to show in menus. */

	/**
	 * (+?) Function called when user equip the object.
	 *
	 * \param eq this equipment
	 * \param ch the character owner
	 */
	void (*equip)(const struct equipment *eq, struct character *ch);
};

/**
 * Tells if this equipment object is valid.
 *
 * \param eq the equipment to check (may be NULL)
 * \return True if valid.
 */
bool
equipment_ok(const struct equipment *eq);

/**
 * Shortcut for eq->equip (if not NULL).
 *
 * \pre equipment_ok(eq)
 * \pre character_ok(ch)
 * \param eq this equipment
 * \param ch the character owner
 */
void
equipment_equip(const struct equipment *eq, struct character *ch);

#endif /* !MOLKO_RPG_EQUIPMENT_H */
