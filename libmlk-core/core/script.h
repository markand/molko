/*
 * script.h -- convenient sequence of actions
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

#ifndef MOLKO_CORE_SCRIPT_H
#define MOLKO_CORE_SCRIPT_H

/**
 * \file script.h
 * \brief Convenient sequence of actions.
 * \ingroup actions
 *
 * Those routines wrap individual actions into a sequence of actions into an
 * action itself.
 *
 * This is convenient for scenarios where you need to specify several
 * sequential actions that neet to wait before continuing.
 *
 * In a nutshell, to write a scenario you should:
 *
 * 1. Create a script with see \ref script_init,
 * 2. Create one or more actions and append with \ref script_append,
 *
 * \warning You must always call \ref script_init before using this object.
 */

#include <stdbool.h>
#include <stddef.h>

struct action;

union event;

/**
 * \brief Maximum number of actions in a script.
 */
#define SCRIPT_ACTION_MAX       (128)

/**
 * \brief Sequence of actions and state holder.
 *
 * Setup the array actions within the structure for each action you want to run
 * in order. You can use the convenient \ref script_append instead. If you do
 * manually don't forget to adjust actionsz field accordingly.
 */
struct script {
	struct action *actions[SCRIPT_ACTION_MAX];	/*!< (+&?) Array of actions. */
	size_t actionsz;                                /*!< (-) Number of actions in array. */
	size_t cur;                                     /*!< (-) Current action index.*/
};

/**
 * Initialize a script.
 *
 * This is not necessary if you zero'ed the structure.
 *
 * \pre s != NULL
 * \param s the script
 */
void
script_init(struct script *s);

/**
 * Append a new action to the script.
 *
 * The action must be kept alive until the script is no longer used.
 *
 * \pre s != NULL
 * \pre a != NULL
 * \param s the script
 * \param a the action to reference
 * \return false if unable to append
 */
bool
script_append(struct script *s, struct action *a);

/**
 * Handle the event into the current action.
 *
 * \pre s != NULL
 * \pre ev != NULL
 * \param s the script
 * \param ev the event
 */
void
script_handle(struct script *s, const union event *ev);

/**
 * Update the current action.
 *
 * \pre s != NULL
 * \param s the script
 * \param ticks the number of milliseconds since last frame
 * \return true if the script completed
 */
bool
script_update(struct script *s, unsigned int ticks);

/**
 * Draw the current action.
 *
 * \pre s != NULL
 * \param s the script
 */
void
script_draw(struct script *s);

/**
 * Tells if the script is terminated.
 *
 * \pre s != NULL
 * \param s the script
 * \return true if all action were completed
 */
bool
script_completed(const struct script *s);

/**
 * Destroy all the actions into the script.
 *
 * \pre s != NULL
 * \param s the script
 */
void
script_finish(struct script *s);

/**
 * Create an action from the script itself for use into the game.
 *
 * The script must be kept alive until the action is no longer needed.
 *
 * \pre s != NULL
 * \pre dst != NULL
 * \param s the script
 * \param dst the action to build with the script
 */
void
script_action(struct script *s, struct action *dst);

#endif /* !MOLKO_CORE_SCRIPT_H */
