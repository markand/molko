/*
 * game.c -- main game object
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

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "game.h"
#include "state.h"
#include "painter.h"

struct game game;

void
game_switch(struct state *state, bool quick)
{
	assert(state);

	if (quick) {
		if (game.state_next)
			state_finish(game.state_next);

		game.state_next = NULL;
		game.state = state;
		state_start(game.state);
	} else
		game.state_next = state;
}

void
game_handle(const union event *ev)
{
	assert(ev);

	if (game.state && !(game.inhibit & INHIBIT_STATE_INPUT))
		state_handle(game.state, ev);
}

void
game_update(unsigned int ticks)
{
	if (game.inhibit & INHIBIT_STATE_UPDATE)
		return;

	/* Change state if any. */
	if (game.state_next) {
		struct state *previous;

		/* Inform the current state we're gonna leave it. */
		if ((previous = game.state))
			state_end(previous);

		/* Change the state and tell we're starting it. */
		if ((game.state = game.state_next))
			state_start(game.state);

		game.state_next = NULL;

		/*
		 * Only call finish at the end of the process because
		 * the user may still use resources from it during the
		 * transition.
		 */
		if (previous)
			state_finish(previous);
	}

	if (game.state)
		state_update(game.state, ticks);
}

void
game_draw(void)
{
	if (game.state && !(game.inhibit & INHIBIT_STATE_DRAW))
		state_draw(game.state);

	painter_present();
}

void
game_quit(void)
{
	/* Close the next state if any. */
	if (game.state_next) {
		state_finish(game.state_next);
		game.state_next = NULL;
	}
	
	if (game.state) {
		state_end(game.state);
		state_finish(game.state);
		game.state = NULL;
	}
}
