/*
 * example-message.c -- show how to use messages
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
#include <core/message.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/sys.h>
#include <core/util.h>
#include <core/theme.h>
#include <core/window.h>

#define W 1280
#define H 720

static void
init(void)
{
	if (!sys_init() ||
	    !window_init("Example - Message", W, H) ||
	    !theme_init())
		panic();
}

static void
quit(void)
{
	theme_finish();
	window_finish();
	sys_finish();
}

static void
run(struct message *msg)
{
	struct clock clock = {0};

	message_start(msg);
	clock_start(&clock);

	while (msg->state) {
		union event ev;
		unsigned int elapsed = clock_elapsed(&clock);

		clock_start(&clock);

		while (event_poll(&ev)) {
			switch (ev.type) {
			case EVENT_QUIT:
				msg->state = MESSAGE_NONE;
				break;
			default:
				message_handle(msg, &ev);
				break;
			}
		}

		message_update(msg, elapsed);
		painter_set_color(0xffffffff);
		painter_clear();
		message_draw(msg);
		painter_present();

		if ((elapsed = clock_elapsed(&clock)) < 20)
			delay(20 - elapsed);
	}
}

static void
basic(void)
{
	struct message msg = {
		.text = {
			"This is a basic message.",
			"You need to press <Enter> to close it."
		},
	};

	run(&msg);
}

static void
automatic(void)
{
	struct message msg = {
		.text = {
			"This is a an automatic message.",
			"It will disappear in a few seconds.",
			"You can still press <Enter> to close it quicker."
		},
		.flags = MESSAGE_AUTOMATIC
	};

	run(&msg);
}

static void
question(void)
{
	struct message msg = {
		.text = {
			"Okay, I've understood.",
			"Nevermind, I'll do it again."
		},
		.flags = MESSAGE_QUESTION
	};

	run(&msg);
}

int
main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	init();
	basic();
	automatic();
	question();
	quit();
}
