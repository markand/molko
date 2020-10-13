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

#include <string.h>

#include <core/clock.h>
#include <core/event.h>
#include <core/frame.h>
#include <core/message.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/sys.h>
#include <core/util.h>
#include <core/theme.h>
#include <core/window.h>

#define W       (1280)
#define H       (720)

#define MW      (W * 0.75)
#define MH      (H * 0.125)
#define MX      ((W / 2) - (MW / 2))
#define MY      (100)

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
				msg->state = MESSAGE_STATE_NONE;
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
my_draw_frame(struct theme *th, const struct frame *f)
{
	(void)th;

	painter_set_color(0xff0000ff);
	painter_draw_rectangle(f->x, f->y, f->w, f->h);
}

static void
basic(void)
{
	struct message msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.h = MH,
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
		.x = MX,
		.y = MY,
		.w = MW,
		.h = MH,
		.timeout = MESSAGE_TIMEOUT_DEFAULT,
		.text = {
			"This is a an automatic message.",
			"It will disappear in a few seconds.",
			"You can still press <Enter> to close it quicker."
		},
		.flags = MESSAGE_FLAGS_AUTOMATIC
	};

	run(&msg);
}

static void
fadein(void)
{
	struct message msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.h = MH,
		.delay = MESSAGE_DELAY_DEFAULT,
		.text = {
			"This message will fade in."
		},
		.flags = MESSAGE_FLAGS_FADEIN
	};

	run(&msg);
}

static void
fadeout(void)
{
	struct message msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.h = MH,
		.delay = MESSAGE_DELAY_DEFAULT,
		.text = {
			"This message will fade out."
		},
		.flags = MESSAGE_FLAGS_FADEOUT
	};

	run(&msg);
}

static void
fade(void)
{
	struct message msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.h = MH,
		.delay = MESSAGE_DELAY_DEFAULT,
		.text = {
			"This message will fade in and out."
		},
		.flags = MESSAGE_FLAGS_FADEIN | MESSAGE_FLAGS_FADEOUT
	};

	run(&msg);
}

static void
question(void)
{
	struct message msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.h = MH,
		.text = {
			"Okay, I've understood.",
			"Nevermind, I'll do it again."
		},
		.flags = MESSAGE_FLAGS_QUESTION
	};

	run(&msg);
}

static void
smallbottom(void)
{
	const unsigned int w = window.w / 4;
	const unsigned int h = 50;
	const int x = (window.w / 2) - (w / 2);
	const int y = (window.h - h - 10);

	struct message msg = {
		.x = x,
		.y = y,
		.w = w,
		.h = h,
		.delay = MESSAGE_DELAY_DEFAULT,
		.flags = MESSAGE_FLAGS_FADEIN | MESSAGE_FLAGS_FADEOUT,
		.text = {
			"This one is small here."
		}
	};

	run(&msg);
}

static void
custom(void)
{
	struct theme theme;
	struct message msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.h = MH,
		.text = {
			"This one will destroy your eyes.",
			"Because it use a terrible custom theme."
		},
		.theme = &theme
	};

	/* Borrow default theme and change its frame drawing. */
	memcpy(&theme, theme_default(), sizeof (theme));
	theme.draw_frame = my_draw_frame;
	theme.colors[THEME_COLOR_NORMAL] = 0x0000ffff;

	run(&msg);
}

int
main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	init();
	basic();
	fadein();
	fadeout();
	fade();
	automatic();
	question();
	smallbottom();
	custom();
	quit();
}
