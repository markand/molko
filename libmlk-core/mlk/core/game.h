/*
 * game.h -- main game object
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

#ifndef MLK_CORE_GAME_H
#define MLK_CORE_GAME_H

/**
 * \file mlk/core/game.h
 * \brief Main game object
 *
 * For convenience, the default game structure is already initialized with an
 * array of 8 states usable.
 */

#include <stddef.h>

struct mlk_state;

union mlk_event;

/**
 * \enum mlk_game_inhibit
 * \brief Inhibit game loop
 */
enum mlk_game_inhibit {
	/**
	 * Inhibit user input.
	 */
	MLK_GAME_INHIBIT_INPUT  = (1 << 0),

	/**
	 * Inhibit game update.
	 */
	MLK_GAME_INHIBIT_UPDATE = (1 << 1),

	/**
	 * Inhibit drawing.
	 *
	 * \note Turning off drawing can introduce weird results on some
	 *       platforms.
	 */
	MLK_GAME_INHIBIT_DRAW   = (1 << 2)
};

/**
 * \struct mlk_game
 * \brief Game structure
 */
struct mlk_game {
	/**
	 * (read-write)
	 *
	 * Inhibit a state function from the game loop.
	 *
	 * Enabling any flags on this field will skip according function from
	 * the loop.
	 */
	enum mlk_game_inhibit inhibit;

	/**
	 * (read-write, borrowed)
	 *
	 * Array of non-owning states.
	 */
	struct mlk_state **states;

	/**
	 * Number of states in array ::mlk_game::states.
	 *
	 * \warning Changing this value must be kept in sync with the array
	 *          dimension.
	 */
	size_t statesz;

	/** \cond MLK_PRIVATE_DECLS */
	struct mlk_state **state;
	/** \endcond MLK_PRIVATE_DECLS */
};

/**
 * \brief Main game loop structure.
 */
extern struct mlk_game mlk_game;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Initialize the main game loop.
 */
void
mlk_game_init(void);

/**
 * Append the state into the game stack and switch to it, suspending current
 * state.
 *
 * The function takes ownership of the state and will be finalized later.
 *
 * \pre state != NULL
 * \param state the state to switch
 */
void
mlk_game_push(struct mlk_state *state);

/**
 * Pop the current state if any and resume the previous one.
 */
_Noreturn void
mlk_game_pop(void);

/**
 * Enter a game loop until there is no more states.
 *
 * The current implementation will perform a loop capped to a 60 FPS rate and
 * update the states with the appropriate number of ticks.
 *
 * \pre state != NULL
 * \param state the first state to run
 */
void
mlk_game_loop(struct mlk_state *state);

/**
 * Request the game loop to stop by removing all states.
 */
void
mlk_game_quit(void);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_GAME_H */
