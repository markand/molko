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

#include <stddef.h>

#include "game.h"
#include "state.h"

struct game game = {
	.state = NULL,
	.state_next = NULL
};

void
game_handle(const union event *event)
{
	if (game.state)
		game.state->handle(event);
}

void
game_update(unsigned int ticks)
{
	/* Change state if any. */
	if (game.state_next) {
		/* Inform the current state we're gonna leave it. */
		if (game.state)
			game.state->leave();

		game.state = game.state_next;
		game.state->enter();
	}

	if (game.state)
		game.state->update(ticks);
}

void
game_draw(void)
{
	if (game.state)
		game.state->draw();
}
