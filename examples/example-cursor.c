/*
 * example-cursor.c -- example on how to change cursor
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

#include <stdio.h>

#include <core/clock.h>
#include <core/core.h>
#include <core/event.h>
#include <core/key.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/sys.h>
#include <core/util.h>
#include <core/window.h>

#include <ui/label.h>
#include <ui/ui.h>

#define W 1280
#define H 720

static char help_text[128];

static struct label help = {
	.x = 10,
	.y = 10,
	.text = help_text,
	.flags = LABEL_FLAGS_SHADOW
};

static void
init(void)
{
	if (!core_init() || !ui_init())
		panic();
	if (!window_open("Example - Cursor", W, H))
		panic();
}

static void
change(enum window_cursor cursor)
{
	static const char *names[] = {
		[WINDOW_CURSOR_ARROW]           = "WINDOW_CURSOR_ARROW",
		[WINDOW_CURSOR_EDIT]            = "WINDOW_CURSOR_EDIT",
		[WINDOW_CURSOR_WAIT]            = "WINDOW_CURSOR_WAIT",
		[WINDOW_CURSOR_CROSSHAIR]       = "WINDOW_CURSOR_CROSSHAIR",
		[WINDOW_CURSOR_SIZE]            = "WINDOW_CURSOR_SIZE",
		[WINDOW_CURSOR_NO]              = "WINDOW_CURSOR_NO",
		[WINDOW_CURSOR_HAND]            = "WINDOW_CURSOR_HAND"
	};

	snprintf(help_text, sizeof (help_text), "Keys: <Left>/<Right> to change cursor. Current: %s", names[cursor]);
	window_set_cursor(cursor);
}

static void
run(void)
{
	struct clock clock = {0};
	enum window_cursor cursor = WINDOW_CURSOR_ARROW;

	clock_start(&clock);
	change(cursor);

	for (;;) {
		union event ev;
		unsigned int elapsed = clock_elapsed(&clock);

		clock_start(&clock);

		while (event_poll(&ev)) {
			switch (ev.type) {
			case EVENT_KEYDOWN:
				switch (ev.key.key) {
				case KEY_LEFT:
					if (cursor > 0)
						change(--cursor);
					break;
				case KEY_RIGHT:
					if (cursor + 1 < WINDOW_CURSOR_LAST)
						change(++cursor);
					break;
				default:
					break;
				}


				break;
			case EVENT_QUIT:
				return;
			default:
				break;
			}
		}

		painter_set_color(0xebede9ff);
		painter_clear();
		label_draw(&help);
		painter_present();

		if ((elapsed = clock_elapsed(&clock)) < 20)
			delay(20 - elapsed);
	}
}

static void
quit(void)
{
	window_finish();
	ui_finish();
	core_finish();
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

