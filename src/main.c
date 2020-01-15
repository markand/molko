/*
 * main.c -- Molko's Adventure
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

#include "clock.h"
#include "game.h"
#include "error.h"
#include "event.h"
#include "sys.h"
#include "window.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

static void
init(void)
{
	if (!sys_init())
		error_fatal();
	if (!window_init("Molko's Adventure", WINDOW_WIDTH, WINDOW_HEIGHT))
		error_fatal();
}

static void
run(void)
{
	union event ev;
	struct clock clock;

	clock_start(&clock);

	for (;;) {
		uint64_t elapsed = clock_elapsed(&clock);

		clock_start(&clock);

		while (event_poll(&ev)) {
			/* TODO: this must be handled by states. */
			if (ev.type == EVENT_QUIT)
				return;

			game_handle(&ev);
		}

		game_update(elapsed);
		game_draw();
	}
}

static void
close(void)
{
	window_close();
	sys_close();
}

int
main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	init();
	run();
	close();

	return 0;
}
