/*
 * action.h -- generic in game actions
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

#ifndef MLK_CORE_ACTION_H
#define MLK_CORE_ACTION_H

/**
 * \file mlk/core/action.h
 * \brief Generic in game actions.
 *
 * This module help creating user interaction within the gameplay by adding
 * actions. They have the following properties:
 *
 * - Can handle user input and events,
 * - Can be updated through the game loop,
 * - Can be drawn.
 *
 * Most more high level objects can handle actions to add flexibility (like in
 * battles, maps, etc).
 */

#include "core.h"

union mlk_event;

/**
 * \struct mlk_action
 * \brief Action structure
 *
 * Use this structure to create an action that may handle input, be updated and
 * drawn.
 */
struct mlk_action {
	/**
	 * (read-write, borrowed, optional)
	 *
	 * Arbitrary user data.
	 */
	void *data;

	/**
	 * (optional)
	 *
	 * Start the action.
	 *
	 * Use this function when the action should start.
	 *
	 * \param self this action
	 */
	void (*start)(struct mlk_action *self);

	/**
	 * (optional)
	 *
	 * Handle an event.
	 *
	 * \param self this action
	 * \param event the event
	 */
	void (*handle)(struct mlk_action *self, const union mlk_event *event);

	/**
	 * (optional)
	 *
	 * Update the action with the given ticks since last frame.
	 *
	 * The callback should return non-zero if it is considered complete.
	 *
	 * \param self this action
	 * \param ticks frame ticks
	 * \return non-zero if complete
	 */
	int (*update)(struct mlk_action *self, unsigned int ticks);

	/**
	 * (optional)
	 *
	 * Draw the action.
	 *
	 * \param self this action
	 */
	void (*draw)(struct mlk_action *self);

	/**
	 * (optional)
	 *
	 * Terminate the action.
	 *
	 * In contrast to finish, this function should be called after the
	 * action was considered complete.
	 *
	 * \param self this action
	 */
	void (*end)(struct mlk_action *self);

	/**
	 * (optional)
	 *
	 * Dispose resources allocated by/for the action.
	 *
	 * \param self this action
	 */
	void (*finish)(struct mlk_action *);
};

MLK_CORE_BEGIN_DECLS

/**
 * Invoke ::mlk_action::start function if not null.
 *
 * \pre action != NULL
 * \param action the action
 */
void
mlk_action_start(struct mlk_action *action);

/**
 * Invoke ::mlk_action::handle function if not null.
 *
 * \pre action != NULL
 * \param action the action
 * \param event the event
 */
void
mlk_action_handle(struct mlk_action *action, const union mlk_event *event);

/**
 * Invoke ::mlk_action::update function if not null.
 *
 * \pre action != NULL
 * \param action the action
 * \param ticks frame ticks
 */
int
mlk_action_update(struct mlk_action *action, unsigned int ticks);

/**
 * Invoke ::mlk_action::draw function if not null.
 *
 * \pre action != NULL
 * \param action the action
 */
void
mlk_action_draw(struct mlk_action *action);

/**
 * Invoke ::mlk_action::end function if not null.
 *
 * \pre action != NULL
 * \param action the action
 */
void
mlk_action_end(struct mlk_action *action);

/**
 * Invoke ::mlk_action::finish function if not null.
 *
 * \pre action != NULL
 * \param action the action
 */
void
mlk_action_finish(struct mlk_action *action);

MLK_CORE_END_DECLS

#endif /* !MLK_CORE_ACTION_H */
