/*
 * battle-entity-state-moving.h -- the entity is moving
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

#ifndef MLK_RPG_BATTLE_ENTITY_STATE_MOVING_H
#define MLK_RPG_BATTLE_ENTITY_STATE_MOVING_H

#include <mlk/rpg/walksprite.h>

struct battle_entity;

struct battle_entity_state_moving {
	struct walksprite ws;
	int x;
	int y;
};

void
battle_entity_state_moving_init(struct battle_entity_state_moving *, struct battle_entity *, int, int);

int
battle_entity_state_moving_update(struct battle_entity_state_moving *, struct battle_entity *, unsigned int);

void
battle_entity_state_moving_draw(const struct battle_entity_state_moving *, const struct battle_entity *);

void
battle_entity_state_moving(struct battle_entity *, int, int);

#endif /* !MLK_RPG_BATTLE_ENTITY_STATE_MOVING_H */
