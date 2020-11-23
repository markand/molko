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

/**
 * \file battle-entity-state.h
 * \brief Abstract battle entity state.
 *
 * To animate and update battle entities this structure help splitting the logic
 * into differents parts.
 *
 * In contrast to battle states, battle entity states should not switch
 * themselves into an other one because battle states may need to wait until the
 * entity state has finished to continue their own work.
 *
 * \note Do not call pointer functions directly but use the functions from this
 *       file since they may do additional work.
 */

#include <stdbool.h>

struct battle_entity;

/**
 * \brief Abstract battle entity state.
 */
struct battle_entity_state {
	/**
	 * (+&?) Optional state data.
	 */
	void *data;

	/**
	 * (+?) Update the entity.
	 *
	 * This function should understand a value of 0 as ticks to just request
	 * the state's status.
	 *
	 * \pre st != NULL
	 * \pre et != NULL
	 * \param st this state
	 * \param et the entity owner
	 * \param ticks elapsed milliseconds since last frame
	 * \return True if complete.
	 */
	bool (*update)(struct battle_entity_state *st, struct battle_entity *et, unsigned int ticks);

	/**
	 * (+?) Draw the entity.
	 *
	 * If this function is NULL, the default behavior is to draw the entity
	 * sprite at its current position.
	 *
	 * \pre st != NULL
	 * \pre et != NULL
	 * \param st this state
	 * \param et the entity owner
	 */
	void (*draw)(const struct battle_entity_state *st, const struct battle_entity *et);

	/**
	 * (+?) Clear resources.
	 *
	 * \pre st != NULL
	 * \pre et != NULL
	 * \param st this state
	 * \param et the entity owner
	 */
	void (*finish)(struct battle_entity_state *st, struct battle_entity *et);
};

/**
 * Shortcut for st->update (if not NULL).
 *
 * \pre st != NULL
 * \pre et != NULL
 * \param st this state
 * \param et the entity owner
 * \param ticks elapsed milliseconds since last frame
 * \return True if complete.
 */
bool
battle_entity_state_update(struct battle_entity_state *st, struct battle_entity *et, unsigned int ticks);

/**
 * Call st->draw or default implementation if NULL.
 *
 * \pre st != NULL
 * \pre et != NULL
 * \param st this state
 * \param et the entity owner
 */
void
battle_entity_state_draw(const struct battle_entity_state *st, const struct battle_entity *et);

/**
 * Shortcut for st->finish (if not NULL).
 *
 * \pre st != NULL
 * \pre et != NULL
 * \param st this state
 * \param et the entity owner
 */
void
battle_entity_state_finish(struct battle_entity_state *st, struct battle_entity *et);

#endif /* !MOLKO_RPG_BATTLE_ENTITY_STATE_H */
