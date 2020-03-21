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
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

#include "clock.h"
#include "debug.h"
#include "error.h"
#include "event.h"
#include "font.h"
#include "game.h"
#include "image.h"
#include "inhibit.h"
#include "map.h"
#include "map_state.h"
#include "message.h"
#include "painter.h"
#include "panic.h"
#include "panic_state.h"
#include "script.h"
#include "splashscreen_state.h"
#include "sprite.h"
#include "sys.h"
#include "util.h"
#include "wait.h"
#include "window.h"

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
		error_fatal();
	if (!window_init("Molko's Adventure", WINDOW_WIDTH, WINDOW_HEIGHT))
		error_fatal();

	/* Init unrecoverable panic state. */
	panic_state_init();
	panic_handler = unrecoverable;

	/* Default state is splash screen */
	game_switch(&splashscreen_state, true);
}

static void
run(void)
{
	union event ev;
	struct clock clock;
	struct font *font;
	struct texture *frame;
	int panic_trigger = 0;

	if (!(font = font_openf(sys_datapath("fonts/DejaVuSans.ttf"), 15)))
		error_fatal();
	if (!(debug_options.default_font = font_openf(sys_datapath("fonts/DejaVuSans.ttf"), 10)))
		error_fatal();
	if (!(frame = image_openf(sys_datapath("images/message.png"))))
		error_fatal();

	debug_options.enable = true;

	clock_start(&clock);

	while (game.state) {
		unsigned int elapsed = clock_elapsed(&clock);

		clock_start(&clock);

		while (event_poll(&ev)) {
			/* TODO: this must be handled by states. */
			if (ev.type == EVENT_QUIT)
				return;

			/*
			 * TODO: user panic request.
			 */
			if (game.state != &panic_state &&
			    ev.type == EVENT_KEYDOWN &&
			    ev.key.key == KEY_F12) {
				if (++panic_trigger == 3)
					panic("User panic request.");
			}

			game_handle(&ev);
		}

		painter_set_color(0xffffffff);
		painter_clear();

		game_update(elapsed);
		game_draw();

		if ((elapsed = clock_elapsed(&clock)) < 20)
			delay(20 - elapsed);
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

	if (setjmp(panic_buf) == 0)
		/* Initial game run. */
		run();
	else {
		/* Clear event queue to avoid accidental key presses. */
		for (union event ev; event_poll(&ev); )
			continue;

		game_switch(&panic_state, true);
		run();
	}

	close();

	return 0;
}
