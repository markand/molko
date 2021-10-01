/*
 * battle-entity-state.h -- abstract battle entity state
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

#ifndef MOLKO_RPG_BATTLE_ENTITY_STATE_H
#define MOLKO_RPG_BATTLE_ENTITY_STATE_H

#include <core/core.h>

struct battle_entity;
struct sprite;

struct battle_entity_state {
	void *data;
	int (*update)(struct battle_entity_state *, struct battle_entity *, unsigned int);
	void (*draw)(const struct battle_entity_state *, const struct battle_entity *);
	void (*finish)(struct battle_entity_state *, struct battle_entity *);
};

CORE_BEGIN_DECLS

int
battle_entity_state_update(struct battle_entity_state *, struct battle_entity *, unsigned int);

void
battle_entity_state_draw(const struct battle_entity_state *, const struct battle_entity *);

void
battle_entity_state_finish(struct battle_entity_state *, struct battle_entity *);

/* Defined in their own files. */
void
battle_entity_state_normal(struct battle_entity *);

void
battle_entity_state_moving(struct battle_entity *, int, int);

void
battle_entity_state_blinking(struct battle_entity *);

void
battle_entity_state_attacking(struct battle_entity *, struct sprite *);

CORE_END_DECLS

#endif /* !MOLKO_RPG_BATTLE_ENTITY_STATE_H */
