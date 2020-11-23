/*
 * battle-entity-state-attacking.h -- the entity is attacking
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

#ifndef MOLKO_RPG_BATTLE_ENTITY_STATE_ATTACKING_H
#define MOLKO_RPG_BATTLE_ENTITY_STATE_ATTACKING_H

/**
 * \file battle-entity-state-attacking.h
 * \brief The entity is attacking.
 */

struct battle_entity;
struct sprite;

/**
 * Change battle entity state to attacking.
 *
 * This will animate the entity with the given sprite which should contain an
 * animation with the player and its weapon.
 *
 * \pre battle_entity_ok(et)
 * \pre sprite_ok(which)
 * \param et the entity
 * \param which the sprite to use as animation
 * \post et->state->data is set to custom data
 * \post et->state->draw is set
 * \post et->state->update is set
 * \post et->state->finish is set
 */
void
battle_entity_state_attacking(struct battle_entity *et, struct sprite *which);

#endif /* !MOLKO_RPG_BATTLE_ENTITY_STATE_ATTACKING_H */
