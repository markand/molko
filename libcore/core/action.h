/*
 * action.h -- action states
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

#ifndef MOLKO_ACTION_H
#define MOLKO_ACTION_H

/**
 * \file action.h
 * \brief Action states.
 * \ingroup actions
 */

#include <stdbool.h>

/**
 * \brief Maximum number of actions in stack.
 */
#define ACTION_STACK_MAX        128

union event;

/**
 * \brief Action structure.
 *
 * Use this structure to create an action that reacts to user events.
 *
 * The purpose of actions is to simplify user interaction within a specific
 * state or a dedicated user routine. With the help of the companion
 * \ref action_stack it is easy to manage actions in a specific game logic
 * state.
 *
 * All members can be NULL.
 */
struct action {
	/**
	 * (RW, optional)
	 *
	 * Arbitrary user data.
	 */
	void *data;

	/**
	 * (RW, optional)
	 *
	 * Handle event.
	 *
	 * \param act this action
	 * \param ev the event
	 */
	void (*handle)(struct action *act, const union event *ev);

	/**
	 * (RW, optional)
	 *
	 * Update the action.
	 *
	 * \param act this action
	 * \param ticks the number of milliseconds since last frame
	 * \return true if action has terminated
	 */
	bool (*update)(struct action *act, unsigned int ticks);

	/**
	 * (RW)
	 *
	 * Draw the action.
	 *
	 * \param act this action
	 */
	void (*draw)(struct action *act);

	/**
	 * (RW)
	 *
	 * Called when the action was completed.
	 *
	 * This callback is mostly provided to allow the user doing something
	 * else once an action is complete. Predefined actions should not use
	 * this callback by themselves.
	 *
	 * \param act this action
	 */
	void (*end)(struct action *act);

	/**
	 * (RW)
	 *
	 * Close the action before removal. This function should be used to
	 * deallocate memory if necessary.
	 *
	 * \param act this action
	 */
	void (*finish)(struct action *act);
};

/**
 * Shortcut for act->handle (if not NULL).
 *
 * \pre act != NULL
 * \pre ev != NULL
 * \param act the action
 * \param ev the event
 */
void
action_handle(struct action *act, const union event *ev);

/**
 * Shortcut for act->update (if not NULL).
 *
 * \pre act != NULL
 * \param act the action
 * \param ticks the number of milliseconds since last frame
 */
bool
action_update(struct action *act, unsigned int ticks);

/**
 * Shortcut for act->draw (if not NULL).
 *
 * \pre act != NULL
 * \param act the action
 */
void
action_draw(struct action *act);

/**
 * Shortcut for act->end (if not NULL).
 *
 * \pre act != NULL
 * \param act the action
 */
void
action_end(struct action *act);

/**
 * Shortcut for act->finish (if not NULL).
 *
 * \pre act != NULL
 * \param act the action
 */
void
action_finish(struct action *act);

/**
 * \brief Stack of actions.
 *
 * The purpose of this structure is to help managing several actions at once.
 * Actions are automatically removed from the stack if the corresponding update
 * member function returns true after completion.
 *
 * This structure contains pointers to actions that must be kept until the stack
 * is destroyed. User is responsible of deallocating them if they were allocated
 * from the heap.
 */
struct action_stack {
	struct action *actions[ACTION_STACK_MAX];        /*!< (RW) Actions */
};

/**
 * Initalize the action stack.
 *
 * It is unnecessary if the object was zero'ed.
 *
 * \pre st != NULL
 * \param st the stack
 */
void
action_stack_init(struct action_stack *st);

/**
 * Add an action to the stack.
 *
 * \pre st != NULL
 * \pre act != NULL
 * \param st the stack
 * \param act the action
 * \note The pointer must be kept alive.
 */
bool
action_stack_add(struct action_stack *st, struct action *act);

/**
 * Handle an event for all actions.
 *
 * \pre st != NULL
 * \pre ev != NULL
 * \param st the stack
 * \param ev the event
 */
void
action_stack_handle(struct action_stack *st, const union event *ev);

/**
 * Update all actions.
 *
 * \pre st != NULL
 * \param st the stack
 * \param ticks the number of milliseconds since last frame
 * \return true if all actions completed
 */
bool
action_stack_update(struct action_stack *st, unsigned int ticks);

/**
 * Draw all actions.
 *
 * \pre st != NULL
 * \param st the stack
 */
void
action_stack_draw(struct action_stack *st);

/**
 * Tells if there is any pending action in the stack.
 *
 * \pre st != NULL
 * \param st the stack
 * \return true if there is at least one action in the stack
 */
bool
action_stack_completed(const struct action_stack *st);

/**
 * Terminate all actions and clear the stack.
 *
 * \pre st != NULL
 * \param st the stack
 */
void
action_stack_finish(struct action_stack *st);

#endif /* !MOLKO_ACTION_H */
