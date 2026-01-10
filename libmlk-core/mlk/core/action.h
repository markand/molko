/*
 * action.h -- generic in game actions
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

#include <assert.h>

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
	 * (optional)
	 *
	 * Restart the action.
	 */
	void (*start)(struct mlk_action *self);

	/**
	 * (optional)
	 *
	 * Handle an event.
	 *
	 * \param event the last event received
	 */
	void (*handle)(struct mlk_action *self, const union mlk_event *event);

	/**
	 * (optional)
	 *
	 * Update the action with the given ticks since last frame.
	 *
	 * The callback should return true if it is considered complete.
	 *
	 * \param ticks frame ticks
	 * \return true if action completed
	 */
	int (*update)(struct mlk_action *self, unsigned int ticks);

	/**
	 * (optional)
	 *
	 * Draw the action.
	 */
	void (*draw)(struct mlk_action *self);

	/**
	 * (optional)
	 *
	 * Terminate the action.
	 *
	 * In contrast to finish, this function should be called after the
	 * action was considered complete.
	 */
	void (*end)(struct mlk_action *self);

	/**
	 * (optional)
	 *
	 * Dispose resources allocated by/for the action.
	 *
	 * This function is always called the least meaning that user is able
	 * to free the pointer if dynamically allocated.
	 */
	void (*finish)(struct mlk_action *self);
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Invoke ::mlk_action::start function if not null.
 */
static inline void
mlk_action_start(struct mlk_action *action)
{
	assert(action);

	if (action->start)
		action->start(action);
}

/**
 * Invoke ::mlk_action::handle function if not null.
 */
static inline void
mlk_action_handle(struct mlk_action *action, const union mlk_event *event)
{
	assert(action);

	if (action->handle)
		action->handle(action, event);
}

/**
 * Invoke ::mlk_action::update function if not null.
 */
static inline int
mlk_action_update(struct mlk_action *action, unsigned int ticks)
{
	assert(action);

	if (action->update)
		return action->update(action, ticks);

	return 0;
}

/**
 * Invoke ::mlk_action::draw function if not null.
 */
static inline void
mlk_action_draw(struct mlk_action *action)
{
	assert(action);

	if (action->draw)
		action->draw(action);
}

/**
 * Invoke ::mlk_action::end function if not null.
 */
static inline void
mlk_action_end(struct mlk_action *action)
{
	assert(action);

	if (action->end)
		action->end(action);
}

/**
 * Invoke ::mlk_action::finish function if not null.
 */
static inline void
mlk_action_finish(struct mlk_action *action)
{
	assert(action);

	if (action->finish)
		action->finish(action);
}

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_ACTION_H */
