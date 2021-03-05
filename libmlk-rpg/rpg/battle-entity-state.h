/*
 * battle-entity-state.h -- abstract battle entity state
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

#ifndef MOLKO_RPG_BATTLE_ENTITY_STATE_H
#define MOLKO_RPG_BATTLE_ENTITY_STATE_H

#include <stdbool.h>

#include <core/core.h>

struct battle_entity;
struct sprite;

struct battle_entity_state {
	void *data;
	bool (*update)(struct battle_entity_state *st, struct battle_entity *et, unsigned int ticks);
	void (*draw)(const struct battle_entity_state *st, const struct battle_entity *et);
	void (*finish)(struct battle_entity_state *st, struct battle_entity *et);
};

CORE_BEGIN_DECLS

bool
battle_entity_state_update(struct battle_entity_state *st, struct battle_entity *et, unsigned int ticks);

void
battle_entity_state_draw(const struct battle_entity_state *st, const struct battle_entity *et);

void
battle_entity_state_finish(struct battle_entity_state *st, struct battle_entity *et);

/* Defined in their own files. */
void
battle_entity_state_normal(struct battle_entity *et);

void
battle_entity_state_moving(struct battle_entity *et, int destx, int desty);

void
battle_entity_state_blinking(struct battle_entity *et);

void
battle_entity_state_attacking(struct battle_entity *et, struct sprite *which);

CORE_END_DECLS

#endif /* !MOLKO_RPG_BATTLE_ENTITY_STATE_H */
