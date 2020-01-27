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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clock.h"
#include "error.h"
#include "event.h"
#include "font.h"
#include "game.h"
#include "image.h"
#include "map.h"
#include "map_state.h"
#include "message.h"
#include "painter.h"
#include "splashscreen_state.h"
#include "sprite.h"
#include "sys.h"
#include "util.h"
#include "window.h"
#include "script.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

static void
init(void)
{
	if (!sys_init())
		error_fatal();
	if (!window_init("Molko's Adventure", WINDOW_WIDTH, WINDOW_HEIGHT))
		error_fatal();

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

	if (!(font = font_openf(sys_datapath("fonts/DejaVuSans.ttf"), 15)))
		error_fatal();
	if (!(frame = image_openf(sys_datapath("images/message.png"))))
		error_fatal();

	struct message msg = {
		.text = {
			"Flip a coin.",
			"Try your best my friend."
		},
		.colors = {
			0xd9caddff,
			0x94d5ffff
		},
		.font = font,
		.frame = frame,
		.flags = MESSAGE_QUESTION
	};

	clock_start(&clock);
	message_start(&msg);

	for (;;) {
		unsigned int elapsed = clock_elapsed(&clock);

		clock_start(&clock);

		while (event_poll(&ev)) {
			/* TODO: this must be handled by states. */
			if (ev.type == EVENT_QUIT)
				return;

			//game_handle(&ev);
			if (msg.state)
				message_handle(&msg, &ev);
		}

		if (msg.state)
			message_update(&msg, elapsed);

		painter_set_color(0xffffffff);
		painter_clear();

		if (msg.state)
			message_draw(&msg);
		//game_update(elapsed);
		//game_draw();

		painter_present();

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
	run();
	close();

	return 0;
}
