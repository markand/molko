/*
 * spell.h -- magic spells
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

#ifndef MOLKO_SPELL_H
#define MOLKO_SPELL_H

/**
 * \file spell.h
 * \brief Magic spells.
 */

#include "selection.h"

struct character;
struct battle;

/**
 * \brief Kind of spell.
 */
enum spell_type {
	SPELL_TYPE_NEUTRAL,     /*!< No type. */
	SPELL_TYPE_FIRE,        /*!< Fire (affected by attack). */
	SPELL_TYPE_WIND,        /*!< Wind (affected by agility). */
	SPELL_TYPE_WATER,       /*!< Water (affected by luck). */
	SPELL_TYPE_EARTH,       /*!< Earth (affected by defense). */
	SPELL_TYPE_CHAOS,       /*!< Chaotic. */
	SPELL_TYPE_HOLY,        /*!< Holy. */
	SPELL_TYPE_TIME         /*!< Chrono. */
};

/**
 * \brief Spell structure.
 *
 * A spell is a magical object owned by a character and can be used in a battle
 * and/or outside of a battle. It costs a certain amount of magic points and is
 * typed into a category (earth, fire, etc, …).
 *
 * A spell can select one character or all.
 */
struct spell {
	const char *name;               /*!< (+&) Spell name. */
	const char *description;        /*!< (+&) Long description. */
	unsigned int mp;                /*!< (+) Number of MP required. */
	enum spell_type type;           /*!< (+) Kind of spell. */
	enum selection selection;       /*!< (+) Kind of selection. */

	/**
	 * (+) Execute the spell in a battle.
	 *
	 * \param bt the current battle
	 * \param owner the spell owner
	 * \param selection the selection (-1 == all)
	 */
	void (*action)(struct battle *bt, struct character *owner, unsigned int selection);

	/**
	 * (+) Use the spell outside of a battle.
	 *
	 * This function is optional.
	 *
	 * \param owner the spell owner
	 * \param selection the selection flags
	 */
	void (*use)(struct character *owner, int selection);
};

/**
 * Cast this spell within a battle.
 *
 * \pre s != NULL && s->action
 * \pre bt != NULL
 * \pre owner != NULL
 * \param s the spell
 * \param bt the battle
 * \param owner the owner
 * \param selection the selection (index or -1 for all)
 */
void
spell_action(const struct spell *s, struct battle *bt, struct character *owner, unsigned int selection);

/**
 * Use this spell immediately.
 *
 * \pre s != NULL && s->use
 * \pre owner != NULL
 * \param s the spell
 * \param owner the owner
 * \param selection the selection flags
 */
void
spell_use(struct spell *s, struct character *owner, int selection);

#endif /* !MOLKO_SPELL_H */
