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

#include <setjmp.h>
#include <stdnoreturn.h>

#include <core/clock.h>
#include <core/event.h>
#include <core/game.h>
#include <core/panic.h>
#include <core/sys.h>
#include <core/theme.h>
#include <core/util.h>
#include <core/window.h>

#include <adventure/panic_state.h>
#include <adventure/splashscreen_state.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

static jmp_buf panic_buf;

static noreturn void
unrecoverable(void)
{
	longjmp(panic_buf, 1);
}

static void
init(void)
{
	if (!sys_init())
		panic();
	if (!window_init("Molko's Adventure", WINDOW_WIDTH, WINDOW_HEIGHT))
		panic();

	/*
	 * From here, we can setup our panic state which requires a window
	 * to be running.
	 */

	/* Init unrecoverable panic state. */
	panic_state_init();
	panic_handler = unrecoverable;

	if (!theme_init())
		panic();

	/* Default state is splash screen */
	game_switch(&splashscreen_state, true);
}

static void
run(void)
{
	struct clock clock = { 0 };

	while (game.state) {
		unsigned int elapsed = clock_elapsed(&clock);

		clock_start(&clock);

		for (union event ev; event_poll(&ev); ) {
			switch (ev.type) {
			case EVENT_QUIT:
				return;
			default:
				game_handle(&ev);
				break;
			}
		}

		game_update(elapsed);
		game_draw();

		if ((elapsed = clock_elapsed(&clock)) < 20)
			delay(20 - elapsed);
	}
}

static void
close(void)
{
	window_finish();
	sys_finish();
}

int
main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	if (setjmp(panic_buf) == 0) {
		/* Initial game run. */
		init();
		run();
	} else {
		/* Clear event queue to avoid accidental key presses. */
		for (union event ev; event_poll(&ev); )
			continue;

		game_switch(&panic_state, true);
		run();
	}

	close();

	return 0;
}
