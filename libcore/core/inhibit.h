/*
 * inhibit.h -- disable specific game behavior
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

#ifndef MOLKO_INHIBIT_H
#define MOLKO_INHIBIT_H

/**
 * \file inhibit.h
 * \brief Disable specific game behavior.
 */

struct action;

/**
 * \brief Game inhibition.
 *
 * This enum is used to alter the game behavior.
 */
enum inhibit {
	/**
	 * Nothing.
	 */
	INHIBIT_NONE,

	/**
	 * Disable input handling in current state.
	 */
	INHIBIT_STATE_INPUT        = (1 << 0),

	/**
	 * Disable current state updates.
	 */
	INHIBIT_STATE_UPDATE       = (1 << 1),

	/**
	 * Disable current state rendering.
	 */
	INHIBIT_STATE_DRAW         = (1 << 2)
};

#endif /* !MOLKO_INHIBIT_H */
