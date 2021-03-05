/*
 * battle-entity.h -- in game battle entity
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

#ifndef MOLKO_RPG_BATTLE_ENTITY_H
#define MOLKO_RPG_BATTLE_ENTITY_H

#include <stdbool.h>

#include <core/core.h>

#include <ui/label.h>

struct battle;
struct battle_entity_state;
struct character;

/**
 * \brief In battle entity.
 */
struct battle_entity {
	struct character *ch;                   /*!< (+&?) Character to use. */
	int x;                                  /*!< (+) Position on screen. */
	int y;                                  /*!< (+) Position on screen. */
	struct label name;                      /*!< (*) Where its name label is located. */
	struct battle_entity_state *state;      /*!< (+&) Update/draw state. */
};

CORE_BEGIN_DECLS

void
battle_entity_init(struct battle_entity *et);

bool
battle_entity_ok(const struct battle_entity *et);

void
battle_entity_switch(struct battle_entity *et, struct battle_entity_state *st);

bool
battle_entity_update(struct battle_entity *et, unsigned int ticks);

void
battle_entity_draw(const struct battle_entity *et, const struct battle *bt);

void
battle_entity_draw_sprite(const struct battle_entity *et);

void
battle_entity_finish(struct battle_entity *et);

CORE_END_DECLS

#endif /* !MOLKO_RPG_BATTLE_ENTITY_H */
