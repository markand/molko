/*
 * example-debug.c -- example on how to use debug interface
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

#include <core/clock.h>
#include <core/event.h>
#include <core/sys.h>
#include <core/window.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/trace.h>
#include <core/util.h>

#include <ui/debug.h>
#include <ui/theme.h>

#define W 1280
#define H 720

static void
init(void)
{
	if (!sys_init() ||
	    !window_init("Example - Debug", W, H) ||
	    !theme_init())
		panic();

	debug_options.enable = true;
}

static void
run(void)
{
	struct clock clock = {0};
	int x = 0, y = 0;

	clock_start(&clock);

	for (;;) {
		struct debug_report report = {0};
		union event ev;
		unsigned int elapsed = clock_elapsed(&clock);

		clock_start(&clock);

		while (event_poll(&ev)) {
			switch (ev.type) {
			case EVENT_MOUSE:
				x = ev.mouse.x;
				y = ev.mouse.y;
				break;
			case EVENT_QUIT:
				return;
			default:
				break;
			}
		}

		painter_set_color(0x4f8fbaff);
		painter_clear();
		debug(&report, "Game running.");
		debug(&report, "mouse: %d, %d", x, y);
		painter_present();

		if ((elapsed = clock_elapsed(&clock)) < 20)
			delay(20 - elapsed);
	}
}

static void
quit(void)
{
	theme_finish();
	window_finish();
	sys_finish();
}

int
main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	init();
	run();
	quit();
}

