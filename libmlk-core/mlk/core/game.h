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
 * \struct mlk_game_ops
 * \brief Game loop callbacks.
 *
 * This structure is passed to the ::mlk_game to be called for handling,
 * updating and drawing the game.
 */
struct mlk_game_ops {
	/**
	 * (optional)
	 *
	 * Invoked when the game loop is ready and starting.
	 */
	void (*start)(void);

	/**
	 * (optional)
	 *
	 * Handle an event.
	 *
	 * \param event the last event received
	 */
	void (*handle)(const union mlk_event *event);

	/**
	 * (optional)
	 *
	 * Update since last iteration.
	 *
	 * \param ticks frame ticks
	 */
	void (*update)(unsigned int ticks);

	/**
	 * (optional)
	 *
	 * Rendering callback.
	 */
	void (*draw)(void);
};

/**
 * \struct mlk_game
 * \brief Game structure.
 */
struct mlk_game {
	/**
	 * (read-write)
	 *
	 * Functions table to use.
	 */
	const struct mlk_game_ops *ops;

	/**
	 * (read-write)
	 *
	 * Inhibit a state function from the game loop.
	 *
	 * Enabling any flags on this field will skip according function from
	 * the loop.
	 */
	enum mlk_game_inhibit inhibit;
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
mlk_game_init(const struct mlk_game_ops *ops);

/**
 * Enter game loop until user calls ::mlk_game_quit.
 */
void
mlk_game_loop(void);

/**
 * Request to quit.
 */
void
mlk_game_quit(void);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_GAME_H */
