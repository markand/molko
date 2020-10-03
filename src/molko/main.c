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

#include <button.h>
#include <checkbox.h>
#include <clock.h>
#include <debug.h>
#include <error.h>
#include <event.h>
#include <font.h>
#include <frame.h>
#include <game.h>
#include <image.h>
#include <inhibit.h>
#include <label.h>
#include <label.h>
#include <map.h>
#include <map_state.h>
#include <message.h>
#include <painter.h>
#include <panic.h>
#include <panic_state.h>
#include <script.h>
#include <splashscreen_state.h>
#include <sprite.h>
#include <sys.h>
#include <theme.h>
#include <util.h>
#include <wait.h>
#include <window.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

static jmp_buf panic_buf;
static struct font debug_font;

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
	if (!theme_init())
		panic();

	/* Init unrecoverable panic state. */
	panic_state_init();
	panic_handler = unrecoverable;

	/* Default state is splash screen */
	game_switch(&splashscreen_state, true);
}

static void
run(void)
{
	struct clock clock = { 0 };
	struct frame mainmenu = {
		.x = 10,
		.y = 10,
		.w = 200,
		.h = 64
	};
	struct checkbox cb = {
		.label = "Play hard mode",
		.x = 20,
		.y = 20,
		.w = 200,
		.h = 16
	};

	for (;;) {
		unsigned int elapsed = clock_elapsed(&clock);

		clock_start(&clock);

		for (union event ev; event_poll(&ev); ) {
			switch (ev.type) {
			case EVENT_QUIT:
				return;
			default:
				checkbox_handle(&cb, &ev);
				break;
			}
		}

		painter_set_color(0xffffffff);
		painter_clear();

		theme_draw_frame(NULL, &(const struct frame){ .x=10, .y=10, .w=500, .h=500 });
		checkbox_draw(&cb);

		painter_present();

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
