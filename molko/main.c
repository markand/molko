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
#include <core/core.h>
#include <core/event.h>
#include <core/game.h>
#include <core/panic.h>
#include <core/state.h>
#include <core/sys.h>
#include <core/util.h>
#include <core/window.h>

#include <ui/theme.h>
#include <ui/ui.h>

#include <rpg/rpg.h>

#include <adventure/state/panic.h>
#include <adventure/state/splashscreen.h>
#include <adventure/state/mainmenu.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

static struct {
	struct state splash;
	struct state mainmenu;
	struct state panic;
} states;

static jmp_buf panic_buf;

static noreturn void
unrecoverable(void)
{
	longjmp(panic_buf, 1);
}

static void
init(void)
{
	if (!core_init() || !ui_init() || !rpg_init())
		panic();
	if (!window_open("Molko's Adventure", WINDOW_WIDTH, WINDOW_HEIGHT))
		panic();

	/*
	 * From here, we can setup our panic state which requires a window
	 * to be running.
	 */

	/* Init unrecoverable panic state. */
	panic_state(&states.panic);
	panic_handler = unrecoverable;

	/* Init states. */
	splashscreen_state(&states.splash, &states.mainmenu);
	mainmenu_state(&states.mainmenu);

	game_switch(&states.splash, true);
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
	rpg_finish();
	ui_finish();
	core_finish();
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

		game_switch(&states.panic, true);
		run();
	}

	close();

	return 0;
}
