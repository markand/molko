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

#include "clock.h"
#include "event.h"
#include "game.h"
#include "state.h"
#include "painter.h"
#include "util.h"
#include "window.h"

struct game game = {
	.state = &game.states[0],
};

void
game_push(struct state *state)
{
	assert(state);
	assert(game.state != &game.states[GAME_STATE_MAX]);

	if (*game.state) {
		state_suspend(*game.state);
		state_start(*(++game.state) = state);
	} else
		state_start((*game.state) = state);
}

void
game_pop(void)
{
	if (!*game.state)
		return;

	state_end(*game.state);
	state_finish(*game.state);

	if (game.state != &game.states[0])
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
	struct clock clock = {0};
	unsigned int elapsed = 0;
	unsigned int frametime;

	if (window.framerate > 0)
		frametime = 1000 / window.framerate;
	else
		/* Assuming 50.0 FPS. */
		frametime = 1000.0 / 50.0;

	while (*game.state) {
		clock_start(&clock);

		for (union event ev; event_poll(&ev); )
			game_handle(&ev);

		game_update(elapsed);
		game_draw();

		/*
		 * If vsync is enabled, it should have wait, otherwise sleep
		 * a little to save CPU cycles.
		 */
		if ((elapsed = clock_elapsed(&clock)) < frametime)
			util_delay(frametime - elapsed);

		elapsed = clock_elapsed(&clock);
	}
}

void
game_quit(void)
{
	for (size_t i = 0; i < UTIL_SIZE(game.states); ++i)
		if (game.states[i])
			state_finish(game.states[i]);

	*game.state = NULL;
}
