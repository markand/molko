/*
 * battle-state.h -- battle abstract state
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

#ifndef MOLKO_BATTLE_STATE_H
#define MOLKO_BATTLE_STATE_H

/**
 * \file battle-state.h
 * \brief Battle abstract state.
 *
 * As a game battle is split into different steps, they are implemented as
 * multiple states to facilitate transitions and have a less complicated code.
 *
 * Each state can handle, update and draw the game logic. The battle itself
 * does only few things by itself like playing music, handling custom actions
 * and drawables and dispatch the rest to the current state.
 */

#include <stdbool.h>

struct battle;

union event;

/**
 * \brief Battle abstract state.
 */
struct battle_state {
	/**
	 * (+&?) Optional data for the state.
	 */
	void *data;

	/**
	 * (+?) Handle an event.
	 *
	 * \pre bt != NULL
	 * \pre ev != NULL
	 * \param bt the current battle
	 * \param ev the event
	 */
	void (*handle)(struct battle_state *st, struct battle *bt, const union event *ev);

	/**
	 * (+?) Update the battle state.
	 *
	 * \pre bt != NULL
	 * \param bt the current battle
	 * \param ticks the number of milliseconds since last frame
	 * \return True if the battle is considered complete.
	 */
	bool (*update)(struct battle_state *st, struct battle *bt, unsigned int ticks);

	/**
	 * (+?) Draw the battle state.
	 *
	 * Note, the battle itself already draw the background and game entities
	 * see the \ref battle_draw function for more details.
	 *
	 * \pre bt != NULL
	 * \param bt the current battle
	 */
	void (*draw)(const struct battle_state *st, const struct battle *bt);

	/**
	 * (+?) Close internal resources if necessary.
	 *
	 * \pre bt != NULL
	 * \param bt the current battle
	 */
	void (*finish)(struct battle_state *st, struct battle *bt);
};

/**
 * Shortcut for st->handle (if not NULL).
 *
 * \pre st != NULL
 * \pre bt != NULL
 * \pre ev != NULL
 * \param st this state
 * \param bt the current battle
 * \param ev the input event
 */
void
battle_state_handle(struct battle_state *st, struct battle *bt, const union event *ev);

/**
 * Shortcut for st->update (if not NULL).
 *
 * \pre st != NULL
 * \pre bt != NULL
 * \param st this state
 * \param bt the current battle
 * \param ticks the number of milliseconds since last frame
 * \return The result of st->update if not NULL or false otherwise.
 */
bool
battle_state_update(struct battle_state *st, struct battle *bt, unsigned int ticks);

/**
 * Shortcut for st->draw (if not NULL).
 *
 * \pre st != NULL
 * \pre bt != NULL
 * \param st this state
 * \param bt the current battle
 */
void
battle_state_draw(const struct battle_state *st, const struct battle *bt);

/**
 * Shortcut for st->finish (if not NULL).
 *
 * \pre st != NULL
 * \pre bt != NULL
 * \param st this state
 * \param bt the current battle
 */
void
battle_state_finish(struct battle_state *st, struct battle *bt);

#endif /* !MOLKO_BATTLE_STATE_H */
