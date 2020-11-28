/*
 * character.h -- character definition
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

#ifndef MOLKO_RPG_CHARACTER_H
#define MOLKO_RPG_CHARACTER_H

/**
 * \file character.h
 * \brief Character definition.
 */

#include <stdbool.h>

struct battle;
struct sprite;
struct spell;

/**
 * \brief Maximum number of spells in a character.
 */
#define CHARACTER_SPELL_MAX (64)

/**
 * \brief Character status
 */
enum character_status {
	CHARACTER_STATUS_NORMAL,                        /*!< No status. */
	CHARACTER_STATUS_POISON         = (1 << 0)      /*!< Character is poisoned. */
};

/**
 * \brief Sprites per action.
 *
 * This enumeration should be synced with \ref equipment_type.
 */
enum character_sprite {
	CHARACTER_SPRITE_AXE,           /*!< Attacking with axe. */
	CHARACTER_SPRITE_BOW,           /*!< Attacking with bow. */
	CHARACTER_SPRITE_CROSSBOW,      /*!< Attacking with crossbow. */
	CHARACTER_SPRITE_DAGGER,        /*!< Attacking with dagger. */
	CHARACTER_SPRITE_HAMMER,        /*!< Attacking with hammer. */
	CHARACTER_SPRITE_NORMAL,        /*!< Sprite for walking. */
	CHARACTER_SPRITE_SPIKE,         /*!< Attacking with spike. */
	CHARACTER_SPRITE_SWORD,         /*!< Attacking with sword. */
	CHARACTER_SPRITE_WAND,          /*!< Attacking with wand. */
	CHARACTER_SPRITE_NUM            /*!< Total number of sprites. */
};

/**
 * \brief Equipment per character.
 *
 * This enumeration should be synced with \ref equipment_type.
 */
enum character_equipment {
	CHARACTER_EQUIPMENT_GLOVES,     /*!< Gloves equiped. */
	CHARACTER_EQUIPMENT_HELMET,     /*!< Helmet equiped. */
	CHARACTER_EQUIPMENT_SHIELD,     /*!< Shield equiped. */
	CHARACTER_EQUIPMENT_TOP,        /*!< Top equiped. */
	CHARACTER_EQUIPMENT_TROUSERS,   /*!< Trousers equiped. */
	CHARACTER_EQUIPMENT_WEAPON,     /*!< Weapon equiped. */
	CHARACTER_EQUIPMENT_NUM         /*!< Total number of equipments equiped. */
};

/**
 * \brief Character object
 *
 * This structure owns the current character statistics used in battle.
 */
struct character {
	const char *name;               /*!< (+) Character name. */
	const char *type;               /*!< (+) Type or class name. */
	unsigned int level;             /*!< (+) Character level. */
	enum character_status status;   /*!< (+) Character status. */
	int hp;                         /*!< (+) Heal points. */
	unsigned int hpmax;             /*!< (+) Maximum heal points. */
	unsigned int hpbonus;           /*!< (+) User heal points bonus. */
	int mp;                         /*!< (+) Magic points. */
	unsigned int mpmax;             /*!< (+) Maximum magic points. */
	unsigned int mpbonus;           /*!< (+) User magic points bonus. */
	int atk;                        /*!< (+) Current attack points (increase fire based spells too). */
	unsigned int atkbonus;          /*!< (+) User attack bonus. */
	int def;                        /*!< (+) Current defense points (increase earth based spells too). */
	unsigned int defbonus;          /*!< (+) User defense bonus. */
	int agt;                        /*!< (+) Current agility (increase wind based spells too). */
	unsigned int agtbonus;          /*!< (+) User agility bonus. */
	int luck;                       /*!< (+) Current luck points (increase */
	unsigned int luckbonus;         /*!< (+) User luck bonus. */

	/**
	 * (+&) Sprites to use.
	 */
	struct sprite *sprites[CHARACTER_SPRITE_NUM];

	/**
	 * (+&) Equipments for this character.
	 */
	const struct equipment *equipments[CHARACTER_EQUIPMENT_NUM];

	/**
	 * (+&?) List of spells for this character.
	 */
	const struct spell *spells[CHARACTER_SPELL_MAX];

	/**
	 * (+) Reset statistics from this character class.
	 *
	 * This function must reset the following member variables according
	 * to the class characteristics:
	 *
	 * - hpmax
	 * - mpmax
	 * - atk
	 * - def
	 * - agt
	 * - luck
	 *
	 * \param owner this owner
	 */
	void (*reset)(struct character *owner);

	/**
	 * (+?) Execute an action.
	 *
	 * This function should be present for AI enemies in a battle, it should
	 * be kept NULL for team players unless they have automatic actions
	 * which in that case would skip user input.
	 *
	 * \param owner this owner
	 * \param bt the battle object
	 */
	void (*exec)(struct character *owner, struct battle *bt);
};

/**
 * Check if this is a valid character object.
 *
 * \pre ch != NULL
 * \param ch the character object
 */
bool
character_ok(struct character *ch);

/**
 * Get a string name for the given status.
 *
 * Since status is a bitmask you have to select only one status.
 *
 * \pre status must be valid
 * \param status the status
 * \return A const string.
 */
const char *
character_status_string(enum character_status status);

/**
 * Shortcut for ch->reset.
 *
 * This function is usually called after an equipment change, a level change
 * or and of a battle.
 *
 * \pre ch != NULL
 * \param ch the character object
 */
void
character_reset(struct character *ch);

/**
 * Shortcut for ch->exec (if not NULL)
 *
 * \pre character_ok(ch)
 * \pre bt != NULL
 * \param ch the character
 * \param bt the battle object
 */
void
character_exec(struct character *ch, struct battle *bt);

#endif /* !MOLKO_RPG_CHARACTER_H */
