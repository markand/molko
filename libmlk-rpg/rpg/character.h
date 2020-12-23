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

#include <stdbool.h>

#define CHARACTER_SPELL_MAX (64)

struct battle;
struct save;
struct sprite;
struct spell;

enum character_status {
	CHARACTER_STATUS_NORMAL,
	CHARACTER_STATUS_POISON         = (1 << 0)
};

enum character_sprite {
	CHARACTER_SPRITE_AXE,
	CHARACTER_SPRITE_BOW,
	CHARACTER_SPRITE_CROSSBOW,
	CHARACTER_SPRITE_DAGGER,
	CHARACTER_SPRITE_HAMMER,
	CHARACTER_SPRITE_NORMAL,
	CHARACTER_SPRITE_SPIKE,
	CHARACTER_SPRITE_SWORD,
	CHARACTER_SPRITE_WAND,
	CHARACTER_SPRITE_NUM
};

enum character_equipment {
	CHARACTER_EQUIPMENT_GLOVES,
	CHARACTER_EQUIPMENT_HELMET,
	CHARACTER_EQUIPMENT_SHIELD,
	CHARACTER_EQUIPMENT_TOP,
	CHARACTER_EQUIPMENT_TROUSERS,
	CHARACTER_EQUIPMENT_WEAPON,
	CHARACTER_EQUIPMENT_NUM
};

struct character {
	const char *name;
	enum character_status status;
	unsigned int level;
	int hp;
	unsigned int hpmax;
	unsigned int hpbonus;
	int mp;
	unsigned int mpmax;
	unsigned int mpbonus;
	int atk;
	unsigned int atkbonus;
	int def;
	unsigned int defbonus;
	int agt;
	unsigned int agtbonus;
	int luck;
	unsigned int luckbonus;
	unsigned int team_order;

	struct sprite *sprites[CHARACTER_SPRITE_NUM];
	const struct equipment *equipments[CHARACTER_EQUIPMENT_NUM];
	const struct spell *spells[CHARACTER_SPELL_MAX];

	void (*reset)(struct character *owner);
	void (*exec)(struct character *owner, struct battle *bt);
};

bool
character_ok(const struct character *ch);

const char *
character_status_string(enum character_status status);

void
character_reset(struct character *ch);

void
character_exec(struct character *ch, struct battle *bt);

bool
character_save(const struct character *ch, struct save *s);

bool
character_load(struct character *, struct save *);

#endif /* !MOLKO_RPG_CHARACTER_H */
