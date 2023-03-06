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
#include "err.h"
#include "event.h"
#include "game.h"
#include "state.h"
#include "util.h"
#include "window.h"

static struct mlk_state *states[8];

struct mlk_game mlk_game = {
	.states = states,
	.statesz = MLK_UTIL_SIZE(states)
};

void
mlk_game_init(void)
{
	for (size_t i = 0; i < mlk_game.statesz; ++i)
		mlk_game.states[i] = NULL;
}

int
mlk_game_push(struct mlk_state *state)
{
	assert(state);

	if (!mlk_game.state) {
		mlk_game.state = &mlk_game.states[0];
		mlk_state_start(*mlk_game.state = state);
		return 0;
	}

	if (mlk_game.state == &mlk_game.states[mlk_game.statesz - 1])
		return mlk_errf("no space in game states stack");

	mlk_state_suspend(*mlk_game.state);
	mlk_state_start(*(++mlk_game.state) = state);

	return 0;
}

void
mlk_game_pop(void)
{
	if (!mlk_game.state)
		return;

	mlk_state_end(*mlk_game.state);
	mlk_state_finish(*mlk_game.state);

	if (mlk_game.state == mlk_game.states)
		mlk_game.state = NULL;
	else
		mlk_state_resume(*--mlk_game.state);
}

void
mlk_game_handle(const union mlk_event *ev)
{
	assert(ev);

	if (mlk_game.state && !(mlk_game.inhibit & MLK_GAME_INHIBIT_INPUT))
		mlk_state_handle(*mlk_game.state, ev);
}

void
mlk_game_update(unsigned int ticks)
{
	if (mlk_game.state && !(mlk_game.inhibit & MLK_GAME_INHIBIT_UPDATE))
		mlk_state_update(*mlk_game.state, ticks);
}

void
mlk_game_draw(void)
{
	if (mlk_game.state && !(mlk_game.inhibit & MLK_GAME_INHIBIT_DRAW))
		mlk_state_draw(*mlk_game.state);
}

void
mlk_game_loop(void)
{
	struct mlk_clock clock = {0};
	unsigned int elapsed = 0;
	unsigned int frametime;

	if (mlk_window.framerate > 0)
		frametime = 1000 / mlk_window.framerate;
	else
		/* Assuming 50.0 FPS. */
		frametime = 1000.0 / 50.0;

	while (mlk_game.state) {
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
	for (size_t i = 0; i < mlk_game.statesz; ++i) {
		if (mlk_game.states[i])
			mlk_state_finish(mlk_game.states[i]);

		mlk_game.states[i] = NULL;
	}

	mlk_game.state = NULL;
}
