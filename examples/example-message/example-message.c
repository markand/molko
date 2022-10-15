/*
 * example-message.c -- show how to use messages
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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

#include <stddef.h>

#include <mlk/core/core.h>
#include <mlk/core/event.h>
#include <mlk/core/game.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/state.h>
#include <mlk/core/sys.h>
#include <mlk/core/util.h>
#include <mlk/core/window.h>

#include <mlk/ui/frame.h>
#include <mlk/ui/theme.h>
#include <mlk/ui/ui.h>

#include <rpg/message.h>
#include <rpg/rpg.h>

#define W       (1280)
#define H       (720)

#define MW      (W * 0.75)
#define MH      (H * 0.120)
#define MX      ((W / 2) - (MW / 2))
#define MY      (100)

static struct state *states[1];

static void
init(void)
{
	if (core_init("fr.malikania", "example-message") < 0 || ui_init() < 0 || rpg_init() < 0)
		panic();
	if (window_open("Example - Message", W, H) < 0)
		panic();
}

static void
quit(void)
{
	window_finish();
	rpg_finish();
	ui_finish();
	core_finish();
}

static void
handle(struct state *st, const union event *ev)
{
	switch (ev->type) {
	case EVENT_QUIT:
		game_quit();
		break;
	default:
		message_handle(st->data, ev);
		break;
	}
}

static void
update(struct state *st, unsigned int ticks)
{
	if (message_update(st->data, ticks))
		game_quit();
}

static void
draw(struct state *st)
{
	painter_set_color(0xffffffff);
	painter_clear();
	message_draw(st->data);
	painter_present();
}

static void
run(struct message *msg)
{
	struct state state = {
		.data = msg,
		.handle = handle,
		.update = update,
		.draw = draw
	};

	message_start(msg);

	game_init(states, UTIL_SIZE(states));
	game_push(&state);
	game_loop();
}

static void
my_draw_frame(const struct theme *th, const struct frame *f)
{
	(void)th;

	painter_set_color(0xff0000ff);
	painter_draw_rectangle(f->x, f->y, f->w, f->h);
}

static void
basic(void)
{
	const char * const text[] = {
		"This is a basic message.",
		"Vertical spacing is automatically computed.",
		"You need to press <Enter> to close it.",
	};
	struct message msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.spacing = 12,
		.lines = text,
		.linesz = 3
	};

	message_query(&msg, NULL, &msg.h);
	run(&msg);
}

static void
automatic(void)
{
	const char * const text[] = {
		"This is a an automatic message.",
		"It will disappear in a few seconds.",
		"You can still press <Enter> to close it quicker."
	};
	struct message msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.timeout = MESSAGE_TIMEOUT_DEFAULT,
		.lines = text,
		.linesz = 3,
		.flags = MESSAGE_FLAGS_AUTOMATIC
	};

	message_query(&msg, NULL, &msg.h);
	run(&msg);
}

static void
fadein(void)
{
	const char * const text[] = {
		"This message will fade in."
	};
	struct message msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.delay = MESSAGE_DELAY_DEFAULT,
		.lines = text,
		.linesz = 1,
		.flags = MESSAGE_FLAGS_FADEIN
	};

	message_query(&msg, NULL, &msg.h);
	run(&msg);
}

static void
fadeout(void)
{
	const char * const text[] = {
		"This message will fade out."
	};
	struct message msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.delay = MESSAGE_DELAY_DEFAULT,
		.lines = text,
		.linesz = 1,
		.flags = MESSAGE_FLAGS_FADEOUT
	};

	message_query(&msg, NULL, &msg.h);
	run(&msg);
}

static void
fade(void)
{
	const char * const text[] = {
		"This message will fade in and out."
	};
	struct message msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.delay = MESSAGE_DELAY_DEFAULT,
		.lines = text,
		.linesz = 1,
		.flags = MESSAGE_FLAGS_FADEIN | MESSAGE_FLAGS_FADEOUT
	};

	message_query(&msg, NULL, &msg.h);
	run(&msg);
}

static void
question(void)
{
	const char * const text[] = {
		"Okay, I've understood.",
		"Nevermind, I'll do it again."
	};
	struct message msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.lines = text,
		.linesz = 2,
		.flags = MESSAGE_FLAGS_QUESTION
	};

	message_query(&msg, NULL, &msg.h);
	run(&msg);
}

static void
smallbottom(void)
{
	const unsigned int w = window.w / 4;
	const unsigned int h = MH;
	const int x = (window.w / 2) - (w / 2);
	const int y = (window.h - h - 10);
	const char * const text[] = {
		"This one is small here."
	};

	struct message msg = {
		.x = x,
		.y = y,
		.w = w,
		.h = h,
		.delay = MESSAGE_DELAY_DEFAULT,
		.flags = MESSAGE_FLAGS_FADEIN | MESSAGE_FLAGS_FADEOUT,
		.lines = text,
		.linesz = 1
	};

	run(&msg);
}

static void
toosmallh(void)
{
	const char * const text[] = {
		"This one is too small in height and will emit a warning.",
		"Because this line will be incomplete."
	};
	struct message msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.h = 40,
		.lines = text,
		.linesz = 2
	};

	run(&msg);
}

static void
toosmallw(void)
{
	const char * const text[] = {
		"This one is too small in width."
	};
	struct message msg = {
		.x = MX,
		.y = MY,
		.w = 160,
		.h = MH,
		.lines = text,
		.linesz = 1
	};

	run(&msg);
}

static void
custom(void)
{
	const char * const text[] = {
		"This one will destroy your eyes.",
		"Because it use a terrible custom theme."
	};
	struct theme theme;
	struct message msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.h = MH,
		.lines = text,
		.linesz = 2,
		.theme = &theme
	};

	/* Borrow default theme and change its frame drawing. */
	theme_shallow(&theme, NULL);
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
	toosmallh();
	toosmallw();
	custom();
	quit();
}
