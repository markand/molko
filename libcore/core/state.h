/*
 * state.h -- abstract state
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

#ifndef MOLKO_STATE_H
#define MOLKO_STATE_H

/**
 * \file state.h
 * \brief Abstract state.
 * \ingroup states
 *
 * The state module is a facility that allows changing game context with ease
 * using a single \ref game_switch routine.
 *
 * The user creates any state required, set appropriate functions if needed and
 * place them in the game using \ref game_switch. Then function \ref game_handle
 * \ref game_update and finally \ref game_draw.
 */

union event;

/**
 * \brief Abstract state.
 */
struct state {
	/**
	 * (+&?) Optional user data.
	 */
	void *data;

	/**
	 * (+?) This function is called when the state is about to begin.
	 *
	 * \param state this state
	 */
	void (*start)(struct state *state);

	/**
	 * (+) This function is called for each event that happened.
	 *
	 * \param state this state
	 * \param ev the event
	 */
	void (*handle)(struct state *state, const union event *);

	/**
	 * (+) Update the state.
	 *
	 * This function is called to update the game, with the number of
	 * milliseconds since the last frame.
	 *
	 * \param state this state
	 * \param ev the event
	 */
	void (*update)(struct state *state, unsigned int ticks);

	/**
	 * (+) This function is supposed to draw the game.
	 *
	 * \param state this state
	 */
	void (*draw)(struct state *state);

	/**
	 * (+?) This function is called when the state is about to be switched
	 * away from.
	 *
	 * This function is not called in case `quick` is set to true when
	 * calling \ref game_switch function.
	 *
	 * \param state this state
	 */
	void (*end)(struct state *state);

	/**
	 * (+?) This function is called to close resources if necessary.
	 *
	 * \param state the state
	 */
	void (*finish)(struct state *state);
};

/**
 * Shortcut for state->start (if not NULL)
 *
 * \pre state != NULL
 * \param state the state
 */
void
state_start(struct state *state);

/**
 * Shortcut for state->handle (if not NULL)
 *
 * \pre state != NULL
 * \pre ev != NULL
 * \param state the state
 * \param ev the event
 */
void
state_handle(struct state *state, const union event *ev);

/**
 * Shortcut for state->update (if not NULL)
 *
 * \pre state != NULL
 * \param state the state
 * \param ticks elapsed milliseconds since last frame
 */
void
state_update(struct state *state, unsigned int ticks);

/**
 * Shortcut for state->draw (if not NULL)
 *
 * \pre state != NULL
 * \param state the state
 */
void
state_draw(struct state *state);

/**
 * Shortcut for state->end (if not NULL)
 *
 * \pre state != NULL
 * \param state the state
 */
void
state_end(struct state *state);

/**
 * Shortcut for state->finish (if not NULL)
 *
 * \pre state != NULL
 * \param state the state
 */
void
state_finish(struct state *state);

#endif /* !MOLKO_STATE_H */
