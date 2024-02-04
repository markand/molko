/*
 * state.h -- abstract game loop state
 *
 * Copyright (c) 2020-2023 David Demelier <markand@malikania.fr>
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

#ifndef MLK_CORE_STATE_H
#define MLK_CORE_STATE_H

/**
 * \file mlk/core/state.h
 * \brief Abstract game loop state
 */

union mlk_event;

/**
 * \struct mlk_state
 * \brief State structure
 *
 * This structure contains only virtual functions that the user can fill and
 * will be called by the main loop.
 *
 * All of the functions can be NULL but usually indicate an useless game.
 */
struct mlk_state {
	/**
	 * (read-write, borrowed, optional)
	 *
	 * Optional state data.
	 */
	void *data;

	/**
	 * (read-write, borrowed, optional)
	 *
	 * Arbitrary state name for diagnostic purposes.
	 */
	const char *name;

	/**
	 * (read-write, optional)
	 *
	 * Invoked when the state starts, which is called only one time.
	 *
	 * \param self this state
	 */
	void (*start)(struct mlk_state *self);

	/**
	 * (read-write, optional)
	 *
	 * Handle an event.
	 *
	 * \param self this state
	 * \param event the event
	 */
	void (*handle)(struct mlk_state *self, const union mlk_event *event);

	/**
	 * (read-write, optional)
	 *
	 * Update the state.
	 *
	 * \param self this state
	 * \param ticks frame ticks
	 */
	void (*update)(struct mlk_state *self, unsigned int ticks);

	/**
	 * (read-write, optional)
	 *
	 * Draw the state.
	 *
	 * \param self this state
	 */
	void (*draw)(struct mlk_state *self);

	/**
	 * (read-write, optional)
	 *
	 * Suspend this state.
	 *
	 * This function is called just before switching to a new state.
	 *
	 * \param self this state
	 */
	void (*suspend)(struct mlk_state *self);

	/**
	 * (read-write, optional)
	 *
	 * Resume this state.
	 *
	 * This function is called after the previous state has been left.
	 *
	 * \param self this state
	 */
	void (*resume)(struct mlk_state *self);

	/**
	 * (read-write, optional)
	 *
	 * Terminate the state.
	 *
	 * This function is called when the state is terminated.
	 *
	 * \param self this state
	 */
	void (*end)(struct mlk_state *self);

	/**
	 * (read-write, optional)
	 *
	 * Cleanup resources allocated by the state.
	 *
	 * \param self this state
	 */
	void (*finish)(struct mlk_state *self);
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Invoke ::mlk_state::start function if not NULL.
 *
 * \pre state != NULL
 * \param state the state
 */
void
mlk_state_start(struct mlk_state *state);

/**
 * Invoke ::mlk_state::handle function if not NULL.
 *
 * \pre state != NULL
 * \pre event != NULL
 * \param state the state
 * \param event the event
 */
void
mlk_state_handle(struct mlk_state *state, const union mlk_event *event);

/**
 * Invoke ::mlk_state::update function if not NULL.
 *
 * \pre state != NULL
 * \param state the state
 * \param ticks frame ticks
 */
void
mlk_state_update(struct mlk_state *state, unsigned int ticks);

/**
 * Invoke ::mlk_state::draw function if not NULL.
 *
 * \pre state != NULL
 * \param state the state
 */
void
mlk_state_draw(struct mlk_state *state);

/**
 * Invoke ::mlk_state::suspend function if not NULL.
 *
 * \pre state != NULL
 * \param state the state
 */
void
mlk_state_suspend(struct mlk_state *state);

/**
 * Invoke ::mlk_state::resume function if not NULL.
 *
 * \pre state != NULL
 * \param state the state
 */
void
mlk_state_resume(struct mlk_state *state);

/**
 * Invoke ::mlk_state::end function if not NULL
 *
 * \pre state != NULL
 * \param state the state
 */
void
mlk_state_end(struct mlk_state *state);

/**
 * Invoke ::mlk_state::finish function if not NULL
 *
 * \pre state != NULL
 * \param state the state
 */
void
mlk_state_finish(struct mlk_state *state);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_STATE_H */
