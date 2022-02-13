/*
 * battle-state-item.h -- battle state (using item)
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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

#ifndef MLK_RPG_BATTLE_STATE_ITEM_H
#define MLK_RPG_BATTLE_STATE_ITEM_H

#include <rpg/battle-message.h>

struct battle;
struct battle_entity;
struct inventory_slot;

enum battle_state_item_status {
	BATTLE_STATE_ITEM_ADVANCING,
	BATTLE_STATE_ITEM_MESSAGE,
	BATTLE_STATE_ITEM_RETURNING
};

struct battle_state_item {
	enum battle_state_item_status status;
	struct battle_message msg;
	struct battle_entity *source;
	struct battle_entity *target;
	struct battle_state state;
	struct inventory_slot *slot;
	int origin_x;
};

void
battle_state_item_init(struct battle_state_item *,
                       struct battle *,
                       struct battle_entity *,
                       struct battle_entity *,
                       struct inventory_slot *);

int
battle_state_item_update(struct battle_state_item *, struct battle *, unsigned int);

void
battle_state_item_draw(struct battle_state_item *);

void
battle_state_item(struct battle *,
                  struct battle_entity *,
                  struct battle_entity *,
                  struct inventory_slot *);


#endif /* !MLK_RPG_BATTLE_STATE_ITEM_H */
