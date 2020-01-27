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

#ifndef MOLKO_SCRIPT_H
#define MOLKO_SCRIPT_H

/**
 * \file script.h
 * \brief Convenient sequence of actions.
 *
 * Those routines wrap individual actions into a sequence of actions into an
 * action itself.
 *
 * This is convenient for scenarios where you need to specify several
 * sequential actions that neet to wait before continuing.
 *
 * In a nutshell, to write a scenario you should:
 *
 * 1. Create a script with see script_init,
 * 2. Create one or more actions and append with script_append,
 * 3. Start the action using script_start,
 * 4. Put the script into the game using game_add_action.
 */

#include <stdbool.h>

#include "action.h"

union event;

/**
 * \brief Single-linked list of actions.
 */
struct script_action {
	struct action action;           /*!< (RW) Action to use */
	struct script_action *next;     /*!< (RO) Pointer to next action */
};

/**
 * \brief Sequence of actions and state holder.
 */
struct script {
	struct script_action *iter;     /*!< (RO) Current action */
	struct script_action *head;     /*!< (RO) Beginning */
	struct script_action **tail;    /*!< (RO) Pointer to add to tail */
};

/**
 * Initialize a script.
 *
 * This is mandatory before using any functions, do not zero-initialize the
 * structure yourself.
 *
 * \pre s != NULL
 * \param s the script
 */
void
script_init(struct script *s);

/**
 * Call this function before putting the script in the game.
 *
 * \pre s != NULL
 * \param s the script
 */
void
script_start(struct script *s);

/**
 * Append a new action to the script.
 *
 * The action is copied into the script and does not need to be allocated on
 * the heap.
 *
 * The action can be empty but must have at least update member set.
 *
 * \pre s != NULL
 * \pre a != NULL && a->update
 * \param s the script
 * \param a the action to copy
 */
void
script_append(struct script *s, const struct action *a);

/**
 * Handle the event into the current action.
 *
 * \pre s != NULL
 * \pre ev != NULL
 * \param s the script
 * \param ev the event
 * \note You usually don't need to call this yourself.
 */
void
script_handle(struct script *s, const union event *ev);

/**
 * Update the current action.
 *
 * \pre s != NULL
 * \param s the script
 * \param ticks the number of milliseconds since last frame
 * \note You usually don't need to call this yourself.
 */
bool
script_update(struct script *s, unsigned int ticks);

/**
 * Draw the current action.
 *
 * \pre s != NULL
 * \param s the script
 * \note You usually don't need to call this yourself.
 */
void
script_draw(struct script *s);

/**
 * Create an action from the script for use into the game.
 *
 * \pre s != NULL
 * \pre dst != NULL
 * \param s the script
 * \param dst the action to build with the script
 */
void
script_action(const struct script *s, struct action *dst);

/**
 * Destroy all the actions into the script.
 *
 * \pre s != NULL
 * \param s the script
 * \note You usually don't need to call this yourself.
 */
void
script_finish(struct script *s);

#endif /* !MOLKO_SCRIPT_H */
