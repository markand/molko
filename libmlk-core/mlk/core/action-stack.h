/*
 * action-stack.h -- convenient stack of actions
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

#ifndef MLK_CORE_ACTION_STACK_H
#define MLK_CORE_ACTION_STACK_H

/**
 * \file mlk/core/action-stack.h
 * \brief Convenient stack of actions.
 *
 * Stack of actions.
 *
 * The purpose of this module is to help managing several actions at once.
 * Actions are automatically removed from the stack if the corresponding update
 * member function returns non-zero after completion.
 */

#include <stddef.h>

struct mlk_action;

union mlk_event;

/**
 * \struct mlk_action_stack
 * \brief Action stack structure
 *
 * This structure holds references to actions to be executed.
 */
struct mlk_action_stack {
	/**
	 * (read-write, borrowed)
	 *
	 * Array of non-owning actions to run.
	 */
	struct mlk_action **actions;

	/**
	 * (read-write)
	 *
	 * Number of actions in array ::mlk_action_script::actions
	 *
	 * \warning changing this value must be kept in sync with the array
	 *          dimension.
	 */
	size_t actionsz;
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Initialize the action sequence structure.
 *
 * This function will set all pointers in the ::mlk_action_stack::actions to
 * NULL.
 *
 * \pre stack != NULL
 * \param stack the action stack
 */
void
mlk_action_stack_init(struct mlk_action_stack *stack);

/**
 * Try to append a new action into the stack if one slot in the
 * ::mlk_action_stack::actions is NULL
 *
 * The action is inserted as-is and ownership is left to the caller.
 *
 * \pre stack != NULL
 * \param stack the action stack
 * \param action the action to append
 * \return 0 on success or ::MLK_ERR_NO_MEM if full.
 */
int
mlk_action_stack_add(struct mlk_action_stack *stack,
                     struct mlk_action *action);

/**
 * Tells if there are still at least one action in the stack.
 *
 * \pre stack != NULL
 * \param stack the action stack
 * \return non-zero if completed
 */
int
mlk_action_stack_completed(const struct mlk_action_stack *stack);


/**
 * Invoke ::mlk_action_start on all actions.
 *
 * \pre stack != NULL
 * \param stack the action stack
 */
void
mlk_action_stack_start(struct mlk_action_stack *stack);

/**
 * Invoke ::mlk_action_handle on all actions.
 *
 * \pre stack != NULL
 * \param stack the action stack
 * \param event the event
 */
void
mlk_action_stack_handle(struct mlk_action_stack *stack,
                        const union mlk_event *event);

/**
 * Invoke ::mlk_action_update on all actions.
 *
 * \pre stack != NULL
 * \param stack the action stack
 * \param ticks frame ticks
 */
int
mlk_action_stack_update(struct mlk_action_stack *stack,
                        unsigned int ticks);

/**
 * Invoke ::mlk_action_draw on all actions.
 *
 * \pre stack != NULL
 * \param stack the action stack
 */
void
mlk_action_stack_draw(const struct mlk_action_stack *stack);

/**
 *
 * \pre stack != NULL
 * \param stack the action stack
 */
void
mlk_action_stack_finish(struct mlk_action_stack *stack);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_ACTION_STACK_H */
