/*
 * state.h -- abstract state
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

#ifndef MOLKO_STATE_H
#define MOLKO_STATE_H

/**
 * \file state.h
 * \brief Abstract state.
 * \ingroup states
 */

union event;

/**
 * \brief Abstract state.
 */
struct state {
	/**
	 * (+?) This function is called when the state is entered.
	 */
	void (*enter)(void);

	/**
	 * (+?) This function is called when the state is about to be left.
	 */
	void (*leave)(void);

	/**
	 * (+) This function is called for each event that happened.
	 */
	void (*handle)(const union event *);

	/**
	 * (+) Update the state.
	 *
	 * This function is called to update the game, with the number of
	 * milliseconds since the last frame.
	 */
	void (*update)(unsigned int ticks);

	/**
	 * (+) This function is supposed to draw the game.
	 */
	void (*draw)(void);
};

#endif /* !MOLKO_STATE_H */
