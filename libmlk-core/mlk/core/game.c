/*
 * game.c -- main game object
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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

#include "clock.h"
#include "event.h"
#include "game.h"
#include "util.h"
#include "window.h"

static int quit;

struct mlk_game mlk_game = {};

void
mlk_game_init(const struct mlk_game_ops *ops)
{
	assert(ops);

	mlk_game.ops = ops;
}

void
mlk_game_loop(void)
{
	struct mlk_clock clock = {};
	unsigned int elapsed = 0;
	unsigned int frametime;

	if (mlk_window.framerate > 0)
		frametime = 1000 / mlk_window.framerate;
	else
		/* Assuming 60.0 FPS. */
		frametime = 1000.0 / 60.0;

	if (mlk_game.ops->start)
		mlk_game.ops->start();

	while (!quit) {
		mlk_clock_start(&clock);

		for (union mlk_event ev; mlk_event_poll(&ev); )
			if (!(mlk_game.inhibit & MLK_GAME_INHIBIT_INPUT) && mlk_game.ops->handle)
				mlk_game.ops->handle(&ev);

		if (!(mlk_game.inhibit & MLK_GAME_INHIBIT_UPDATE) && mlk_game.ops->update)
			mlk_game.ops->update(elapsed);
		if (!(mlk_game.inhibit & MLK_GAME_INHIBIT_DRAW) && mlk_game.ops->draw)
			mlk_game.ops->draw();

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
	}
}

void
mlk_game_quit(void)
{
	quit = 1;
}
