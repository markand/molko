/*
 * battle-entity-state-blinking.h -- the entity is blinking
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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

#ifndef MLK_RPG_BATTLE_ENTITY_STATE_BLINKING_H
#define MLK_RPG_BATTLE_ENTITY_STATE_BLINKING_H

struct battle_entity;
struct mlk_texture;

struct battle_entity_state_blinking {
	struct mlk_texture *tex;
	unsigned int elapsed;
	unsigned int count;
};

void
battle_entity_state_blinking_init(struct battle_entity_state_blinking *, struct battle_entity *et);

int
battle_entity_state_blinking_update(struct battle_entity_state_blinking *, unsigned int);

void
battle_entity_state_blinking(struct battle_entity *);

#endif /* !MLK_RPG_BATTLE_ENTITY_STATE_BLINKING_H */
