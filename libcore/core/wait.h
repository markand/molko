/*
 * wait.h -- wait action
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

#ifndef MOLKO_WAIT_H
#define MOLKO_WAIT_H

/**
 * \file wait.h
 * \brief Wait action.
 * \ingroup actions
 *
 * This module is meant to create a delayed action.
 *
 * Combined with \ref script.h, you can create a sequence of actions with
 * delays between each.
 *
 * \code
 * struct script script;
 * struct action action;
 *
 * // Prepare the script.
 * script_init(&script);
 *
 * // Add some actions to script using script_append.
 * // ...
 *
 * // Wait one second delay before next action.
 * wait_action(&(struct wait) { .delay = 1000 }, &action);
 * script_append(&script, &action);
 *
 * // Add more actions after this delay.
 * // ...
 * \endcode
 */

#include <stdbool.h>

struct action;

/**
 * \brief Wait action.
 */
struct wait {
	unsigned int delay;             /*!< (+) Time to wait in milliseconds */
	unsigned int elapsed;           /*!< (-) Elapsed time */
};

/**
 * Start the wait action.
 *
 * This function is equivalent to `w->elapsed = 0`;
 *
 * \pre w != NULL
 * \param w the wait object
 */
void
wait_start(struct wait *w);

/**
 * Update the wait object.
 *
 * \pre w != NULL
 * \param w the wait object
 * \param ticks the number of milliseconds since last frame
 * \return true if complete
 */
bool
wait_update(struct wait *w, unsigned int ticks);

/**
 * Create an action from the wait object.
 *
 * The wait action must be kept alive until the action completes.
 *
 * \pre w != NULL
 * \pre act != NULL
 * \param w the wait object to copy from
 * \param act the action to fill
 */
void
wait_action(struct wait *w, struct action *act);

#endif /* !MOLKO_WAIT_H */
