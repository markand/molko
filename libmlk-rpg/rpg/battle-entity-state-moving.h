/*
 * battle-entity-state-moving.h -- the entity is moving
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

#ifndef MOLKO_RPG_BATTLE_ENTITY_STATE_MOVING_H
#define MOLKO_RPG_BATTLE_ENTITY_STATE_MOVING_H

/**
 * \file battle-entity-state-moving.h
 * \brief The entity is moving.
 */

struct battle_entity;

/**
 * Starts moving the entity at the given destination.
 *
 * This will move the entity to the destination using a walking sprite.
 *
 * \pre battle_entity_ok(et)
 * \param et the entity to move
 * \param destx the destination in x
 * \param desty the destination in y
 * \post et->state->data is set to custom data
 * \post et->state->update is set
 * \post et->state->finish is set
 */
void
battle_entity_state_moving(struct battle_entity *et, int destx, int desty);

#endif /* !MOLKO_RPG_BATTLE_ENTITY_STATE_MOVING_H */
