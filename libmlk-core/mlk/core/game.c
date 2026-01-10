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
#include <setjmp.h>

#include "clock.h"
#include "core_p.h"
#include "err.h"
#include "event.h"
#include "game.h"
#include "state.h"
#include "util.h"
#include "window.h"

enum {
	JMP_OK = 0,
	JMP_PUSH,
	JMP_POP
};

static struct mlk_state *states[8];
static jmp_buf game_jmp_buf;
static int game_jmp_allowed;
static int game_run = 1;

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

void
mlk_game_push(struct mlk_state *state)
{
	assert(state);
	assert(game_jmp_allowed);

	if (mlk_game.state == &mlk_game.states[mlk_game.statesz - 1])
		mlk_errf(_("no space in game states stack"));

	mlk_game.state[1] = state;
	longjmp(game_jmp_buf, JMP_PUSH);
}

_Noreturn void
mlk_game_pop(void)
{
	assert(game_jmp_allowed);

	longjmp(game_jmp_buf, JMP_POP);
}

void
mlk_game_loop(struct mlk_state *state)
{
	assert(state);

	struct mlk_clock clock = {};
	unsigned int elapsed = 0;
	unsigned int frametime;

	if (mlk_window.framerate > 0)
		frametime = 1000 / mlk_window.framerate;
	else
		/* Assuming 60.0 FPS. */
		frametime = 1000.0 / 60.0;

	game_jmp_allowed = 1;

	while (game_run) {
		switch (setjmp(game_jmp_buf)) {
		case JMP_OK:
			/* Initial entrypoint. */
			if (!mlk_game.state) {
				mlk_game.states[0] = state;
				mlk_game.state = &mlk_game.states[0];
				mlk_state_start(state);
			}

			mlk_clock_start(&clock);

			for (union mlk_event ev; mlk_event_poll(&ev); ) {
				if (mlk_game.state && !(mlk_game.inhibit & MLK_GAME_INHIBIT_INPUT))
					mlk_state_handle(*mlk_game.state, &ev);
			}

			if (mlk_game.state && !(mlk_game.inhibit & MLK_GAME_INHIBIT_UPDATE))
				mlk_state_update(*mlk_game.state, elapsed);
			if (mlk_game.state && !(mlk_game.inhibit & MLK_GAME_INHIBIT_DRAW))
				mlk_state_draw(*mlk_game.state);

			/*
			 * If vsync is enabled, it should have wait, otherwise
			 * sleep a little to save CPU cycles.
			 */
			if ((elapsed = mlk_clock_elapsed(&clock)) < frametime)
				mlk_util_sleep(frametime - elapsed);

			elapsed = mlk_clock_elapsed(&clock);

			/*
			 * Cap to frametime if it's too slow because it would
			 * create unexpected results otherwise.
			 */
			if (elapsed > frametime)
				elapsed = frametime;
			break;
		case JMP_PUSH:
			/*
			 * We have pushed a new state, suspend should not modify
			 * the stack because we would have a leak in the next
			 * state being overriden without being finalized.
			 */
			game_jmp_allowed = 0;
			mlk_state_suspend(*mlk_game.state);
			game_jmp_allowed = 1;

			/* Start next state. */
			mlk_state_start(*++mlk_game.state);
			break;
		case JMP_POP:
			/*
			 * We need to finalize this state so the end/finish
			 * functions are not allowed to modify the stack.
			 */
			game_jmp_allowed = 0;
			mlk_state_end(*mlk_game.state);
			mlk_state_finish(*mlk_game.state);
			game_jmp_allowed = 1;

			/* Resume previous state. */
			if (mlk_game.state == mlk_game.states)
				mlk_game.state = NULL;
			else
				mlk_state_resume(*--mlk_game.state);
			break;
		default:
			break;
		}
	}
}

void
mlk_game_quit(void)
{
	game_jmp_allowed = 0;
	game_run = 0;

	for (size_t i = 0; i < mlk_game.statesz; ++i) {
		if (mlk_game.states[i])
			mlk_state_finish(mlk_game.states[i]);

		mlk_game.states[i] = NULL;
	}

	mlk_game.state = NULL;
}
