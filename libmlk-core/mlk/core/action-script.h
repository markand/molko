/*
 * action-script.h -- convenient sequence of actions module
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

#ifndef MLK_CORE_ACTION_SCRIPT_H
#define MLK_CORE_ACTION_SCRIPT_H

/**
 * \file mlk/core/action-script.h
 * \brief Convenient sequence of actions module.
 *
 * Module to create sequence of actions (see mlk/core/action.h)
 *
 * Those routines wrap individual actions into a sequence of actions into an action
 * itself.
 *
 * This is convenient for scenarios where you need to specify several sequential
 * actions that neet to waid the previous before continuing.
 *
 * In a nutshell, to write a scenario you should:
 *
 * 1. Declare a ::mlk_action array with your own size limits.
 * 2. Declare a ::mlk_action_script and populate ::mlk_action_script::actions
 *    and ::mlk_action_script::actionsz accordingly to the array.
 * 3. Invoke ::mlk_action_script_init.
 * 4. Add as many as actions you with with ::mlk_action_script_append.
 * 5. Call ::mlk_action_script_start if you need (optional).
 * 6. Invoke ::mlk_action_script_handle, ::mlk_action_script_update and
 *    ::mlk_action_script_draw in your game loop until it completes.
 */

#include <stddef.h>

#include "core.h"

struct mlk_action;

union mlk_event;

/**
 * \struct mlk_action_script
 * \brief Actions script sequence
 *
 * This structure holds references to actions to be executed sequentially.
 */
struct mlk_action_script {
	/**
	 * (read-write, borrowed)
	 *
	 * Array of non-owning actions to run in order.
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

	/**
	 * (read-only)
	 *
	 * Number of actions inside of the array.
	 */
	size_t length;

	/**
	 * (read-only)
	 *
	 * Index of current action.
	 */
	size_t current;
};

MLK_CORE_BEGIN_DECLS

/**
 * Initialize the action sequence structure.
 *
 * This function will effectively reset the length, current and set all actions
 * to NULL.
 *
 * \pre script != NULL
 * \param script the action script structure
 */
void
mlk_action_script_init(struct mlk_action_script *script);

/**
 * Try to append a new action into the script.
 *
 * The action is inserted as-is and ownership is left to the caller.
 *
 * \pre script != NULL
 * \param script the action script structure
 * \param action the action to append
 * \return 0 on success or ::MLK_ERR_NO_MEM if full.
 */
int
mlk_action_script_append(struct mlk_action_script *script,
                         struct mlk_action *action);

/**
 * Tells if the sequence is complete.
 *
 * \pre script != NULL
 * \param script the action script structure
 * \return non-zero if completed
 */
int
mlk_action_script_completed(const struct mlk_action_script *script);

/**
 * Invoke ::mlk_action_start on the current action.
 *
 * This function should be called only once because when the action completes
 * the next one will be automatically started.
 *
 * \pre script != NULL
 * \param script the action script structure
 */
void
mlk_action_script_start(struct mlk_action_script *script);

/**
 * Invoke ::mlk_action_handle on the current action.
 *
 * \pre script != NULL
 * \param script the action script structure
 * \param event the event
 */
void
mlk_action_script_handle(struct mlk_action_script *script,
                         const union mlk_event *event);

/**
 * Invoke ::mlk_action_update on the current action.
 *
 * \pre script != NULL
 * \param script the action script structure
 * \param ticks frame ticks
 */
int
mlk_action_script_update(struct mlk_action_script *script, unsigned int ticks);

/**
 * Invoke ::mlk_action_draw on the current action.
 *
 * \pre script != NULL
 * \param script the action script structure
 */
void
mlk_action_script_draw(struct mlk_action_script *script);

/**
 * Invoke ::mlk_action_finish on all actions.
 *
 * After this call, the script array of actions is kept and can be reused if
 * ::mlk_action_script_init is called again.
 *
 * \note If the sequence wasn't completed, the remaining actions are finalized
 *       using ::mlk_action_finish but not terminated through ::mlk_action_end.
 * \pre script != NULL
 * \param script the action script structure
 */
void
mlk_action_script_finish(struct mlk_action_script *script);

MLK_CORE_END_DECLS

#endif /* !MLK_CORE_ACTION_SCRIPT_H */
