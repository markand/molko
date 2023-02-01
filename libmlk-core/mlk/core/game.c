/*
 * game.c -- main game object
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

#include <assert.h>
#include <string.h>

#include "clock.h"
#include "event.h"
#include "game.h"
#include "state.h"
#include "util.h"
#include "window.h"

struct game game = {0};

void
game_init(struct state **states, size_t statesz)
{
	assert(states);
	assert(statesz);

	memset(&game, 0, sizeof (game));

	game.states = states;
	game.statesz = statesz;

	for (size_t i = 0; i < game.statesz; ++i)
		game.states[i] = NULL;
}

void
game_push(struct state *state)
{
	assert(state);
	assert(!game.state || game.state != &game.states[game.statesz - 1]);

	if (!game.state) {
		game.state = &game.states[0];
		state_start(*game.state = state);
	} else {
		state_suspend(*game.state);
		state_start(*(++game.state) = state);
	}
}

void
game_pop(void)
{
	assert(game.state);

	state_end(*game.state);
	state_finish(*game.state);

	if (game.state == game.states)
		game.state = NULL;
	else
		state_resume(*--game.state);
}

void
game_handle(const union event *ev)
{
	assert(ev);

	if (*game.state && !(game.inhibit & INHIBIT_STATE_INPUT))
		state_handle(*game.state, ev);
}

void
game_update(unsigned int ticks)
{
	if (*game.state && !(game.inhibit & INHIBIT_STATE_UPDATE))
		state_update(*game.state, ticks);
}

void
game_draw(void)
{
	if (*game.state && !(game.inhibit & INHIBIT_STATE_DRAW))
		state_draw(*game.state);
}

void
game_loop(void)
{
	struct mlk_clock clock = {0};
	unsigned int elapsed = 0;
	unsigned int frametime;

	if (window.framerate > 0)
		frametime = 1000 / window.framerate;
	else
		/* Assuming 50.0 FPS. */
		frametime = 1000.0 / 50.0;

	while (*game.state) {
		mlk_clock_start(&clock);

		for (union event ev; event_poll(&ev); )
			game_handle(&ev);

		game_update(elapsed);
		game_draw();

		/*
		 * If vsync is enabled, it should have wait, otherwise sleep
		 * a little to save CPU cycles.
		 */
		if ((elapsed = mlk_clock_elapsed(&clock)) < frametime)
			util_delay(frametime - elapsed);

		elapsed = mlk_clock_elapsed(&clock);
	}
}

void
game_quit(void)
{
	for (size_t i = 0; i < game.statesz; ++i) {
		if (game.states[i])
			state_finish(game.states[i]);

		game.states[i] = NULL;
	}
}
