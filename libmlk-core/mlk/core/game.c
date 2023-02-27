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

struct mlk_game game = {0};

void
mlk_game_init(struct mlk_state **states, size_t statesz)
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
mlk_game_push(struct mlk_state *state)
{
	assert(state);
	assert(!game.state || game.state != &game.states[game.statesz - 1]);

	if (!game.state) {
		game.state = &game.states[0];
		mlk_state_start(*game.state = state);
	} else {
		mlk_state_suspend(*game.state);
		mlk_state_start(*(++game.state) = state);
	}
}

void
mlk_game_pop(void)
{
	assert(game.state);

	mlk_state_end(*game.state);
	mlk_state_finish(*game.state);

	if (game.state == game.states)
		game.state = NULL;
	else
		mlk_state_resume(*--game.state);
}

void
mlk_game_handle(const union mlk_event *ev)
{
	assert(ev);

	if (*game.state && !(game.inhibit & MLK_INHIBIT_STATE_INPUT))
		mlk_state_handle(*game.state, ev);
}

void
mlk_game_update(unsigned int ticks)
{
	if (*game.state && !(game.inhibit & MLK_INHIBIT_STATE_UPDATE))
		mlk_state_update(*game.state, ticks);
}

void
mlk_game_draw(void)
{
	if (*game.state && !(game.inhibit & MLK_INHIBIT_STATE_DRAW))
		mlk_state_draw(*game.state);
}

void
mlk_game_loop(void)
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

		for (union mlk_event ev; mlk_event_poll(&ev); )
			mlk_game_handle(&ev);

		mlk_game_update(elapsed);
		mlk_game_draw();

		/*
		 * If vsync is enabled, it should have wait, otherwise sleep
		 * a little to save CPU cycles.
		 */
		if ((elapsed = mlk_clock_elapsed(&clock)) < frametime)
			mlk_util_delay(frametime - elapsed);

		elapsed = mlk_clock_elapsed(&clock);
	}
}

void
mlk_game_quit(void)
{
	for (size_t i = 0; i < game.statesz; ++i) {
		if (game.states[i])
			mlk_state_finish(game.states[i]);

		game.states[i] = NULL;
	}
}
