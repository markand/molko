/*
 * spell.h -- magic spells
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

#ifndef MOLKO_RPG_SPELL_H
#define MOLKO_RPG_SPELL_H

#include <core/core.h>

#include "selection.h"

struct character;
struct battle;
struct selection;

enum spell_type {
	SPELL_TYPE_NEUTRAL,
	SPELL_TYPE_FIRE,
	SPELL_TYPE_WIND,
	SPELL_TYPE_WATER,
	SPELL_TYPE_EARTH,
	SPELL_TYPE_CHAOS,
	SPELL_TYPE_HOLY,
	SPELL_TYPE_TIME
};

struct spell {
	const char *name;
	const char *description;
	unsigned int mp;
	enum spell_type type;
	enum selection_kind select_kind;
	enum selection_side select_side;

	void (*select)(const struct battle *, struct selection *);
	void (*action)(struct battle *, struct character *, const struct selection *);
	void (*use)(struct character *, const struct selection *);
};

CORE_BEGIN_DECLS

void
spell_select(const struct spell *, const struct battle *, struct selection *);

void
spell_action(const struct spell *, struct battle *, struct character *, const struct selection *);

void
spell_use(struct spell *, struct character *, const struct selection *);

CORE_END_DECLS

#endif /* !MOLKO_RPG_SPELL_H */
