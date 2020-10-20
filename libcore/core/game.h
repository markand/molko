/*
 * game.h -- main game object
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

#ifndef MOLKO_GAME_H
#define MOLKO_GAME_H

/**
 * \file game.h
 * \brief Main game object.
 */

#include <stdbool.h>

#include "inhibit.h"

struct state;

union event;

/**
 * \brief Main game object.
 */
struct game {
	/* Inhibition */
	enum inhibit inhibit;           /*!< (+) What to disable. */

	/* Game states. */
	struct state *state;            /*!< (-) Current state  */
	struct state *state_next;       /*!< (-) Next state */
};

/**
 * Global game object.
 */
extern struct game game;

/**
 * Request to change state.
 *
 * This function will only update state after the next \ref  game_update call
 * unless quick is set to true.
 *
 * \warning Passing true to quick will immediately call end/finish state
 *          functions which can destroy your state while still running it.
 * \pre state != NULL
 * \param state the new state
 * \param quick quickly change the state
 */
void
game_switch(struct state *state, bool quick);

/**
 * Handle input event.
 *
 * \param event the event
 */
void
game_handle(const union event *event);

/**
 * Update the game state.
 *
 * \param ticks the number of milliseconds between last frame
 */
void
game_update(unsigned int ticks);

/**
 * Draw the game using the current state.
 */
void
game_draw(void);

/**
 * Stop the game.
 *
 * This will effectively stop the current state but the main loop may continue
 * until it has completed.
 */
void
game_quit(void);

#endif /* !MOLKO_GAME_H */
