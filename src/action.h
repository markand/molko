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

#ifndef ACTION_H
#define ACTION_H

/**
 * \file action.h
 * \brief Action states.
 */

#include <stdbool.h>

union event;

/**
 * \brief Action flags.
 */
enum action_flags {
	ACTION_NONE,                            /*!< No flags */
	ACTION_AUTO_LEAVE       = (1 << 0)      /*!< Action is removed on state change */
};

/**
 * \brief Action structure.
 */
struct action {
	/**
	 * (RW)
	 *
	 * Optional flags.
	 */
	enum action_flags flags;

	/**
	 * (RW)
	 *
	 * Arbitrary user data.
	 */
	void *data;

	/**
	 * (RW)
	 *
	 * Handle event.
	 */
	void (*handle)(struct action *, const union event *event);

	/**
	 * (RW)
	 *
	 * Update the action.
	 *
	 * If returns true, the action is removed.
	 */
	bool (*update)(struct action *, unsigned int);

	/**
	 * (RW)
	 *
	 * Draw the aciton.
	 */
	void (*draw)(struct action *);

	/**
	 * (RW)
	 *
	 * Close the action before removal.
	 */
	void (*finish)(struct action *);
};

#endif /* !ACTION_H*/
