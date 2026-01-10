/*
 * example-message.c -- show how to use messages
 *
 * Copyright (c) 2020-2023 David Demelier <markand@malikania.fr>
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
#include <mlk/ui/ui.h>

#include <mlk/rpg/message.h>
#include <mlk/rpg/rpg.h>

#include <mlk/example/example.h>

#define MW      (MLK_EXAMPLE_W * 0.75)
#define MH      (MLK_EXAMPLE_H * 0.120)
#define MX      ((MLK_EXAMPLE_W / 2) - (MW / 2))
#define MY      (100)

static void
init(void)
{
	if (mlk_example_init("example-message") < 0)
		mlk_panic();
}

static void
handle(struct mlk_state *st, const union mlk_event *ev)
{
	switch (ev->type) {
	case MLK_EVENT_QUIT:
		mlk_game_quit();
		break;
	default:
		mlk_message_handle(st->data, ev);
		break;
	}
}

static void
update(struct mlk_state *st, unsigned int ticks)
{
	if (mlk_message_update(st->data, ticks))
		mlk_game_quit();
}

static void
draw(struct mlk_state *st)
{
	mlk_painter_set_color(MLK_EXAMPLE_BG);
	mlk_painter_clear();
	mlk_message_draw(st->data);
	mlk_painter_present();
}

static void
run(struct mlk_message *msg)
{
	struct mlk_state state = {
		.data = msg,
		.handle = handle,
		.update = update,
		.draw = draw
	};

	mlk_message_start(msg);

	mlk_game_init();
	mlk_game_push(&state);
	mlk_game_loop(&state);
}

#if 0
static void
my_draw_frame(const struct mlk_theme *th, const struct mlk_frame *f)
{
	(void)th;

	mlk_painter_set_color(0xff0000ff);
	mlk_painter_draw_rectangle(f->x, f->y, f->w, f->h);
}
#endif

static void
basic(void)
{
	const char * const text[] = {
		"This is a basic message.",
		"Vertical spacing is automatically computed.",
		"You need to press <Enter> to close it.",
	};
	struct mlk_message msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.lines = text,
		.linesz = 3
	};

	mlk_message_query(&msg, NULL, &msg.h);
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
	struct mlk_message msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.lines = text,
		.linesz = 3,
		.flags = MLK_MESSAGE_FLAGS_AUTOMATIC
	};

	mlk_message_query(&msg, NULL, &msg.h);
	run(&msg);
}

static void
fadein(void)
{
	const char * const text[] = {
		"This message will fade in."
	};
	struct mlk_message msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.lines = text,
		.linesz = 1,
		.flags = MLK_MESSAGE_FLAGS_FADEIN
	};

	mlk_message_query(&msg, NULL, &msg.h);
	run(&msg);
}

static void
fadeout(void)
{
	const char * const text[] = {
		"This message will fade out."
	};
	struct mlk_message msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.lines = text,
		.linesz = 1,
		.flags = MLK_MESSAGE_FLAGS_FADEOUT
	};

	mlk_message_query(&msg, NULL, &msg.h);
	run(&msg);
}

static void
fade(void)
{
	const char * const text[] = {
		"This message will fade in and out."
	};
	struct mlk_message msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.lines = text,
		.linesz = 1,
		.flags = MLK_MESSAGE_FLAGS_FADEIN | MLK_MESSAGE_FLAGS_FADEOUT
	};

	mlk_message_query(&msg, NULL, &msg.h);
	run(&msg);
}

static void
question(void)
{
	const char * const text[] = {
		"Do you think you're brave enough to fight this Karen?",
		"Sure I am.",
		"No."
	};
	struct mlk_message msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.lines = text,
		.linesz = 3,
		.selectable = 0x6
	};

	mlk_message_query(&msg, NULL, &msg.h);
	run(&msg);
}

static void
smallbottom(void)
{
	const unsigned int w = mlk_window.w / 4;
	const unsigned int h = MH;
	const int x = (mlk_window.w / 2) - (w / 2);
	const int y = (mlk_window.h - h - 10);
	const char * const text[] = {
		"This one is small here."
	};

	struct mlk_message msg = {
		.x = x,
		.y = y,
		.w = w,
		.h = h,
		.flags = MLK_MESSAGE_FLAGS_FADEIN | MLK_MESSAGE_FLAGS_FADEOUT,
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
	struct mlk_message msg = {
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
	struct mlk_message msg = {
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
	struct mlk_message_style style = *mlk_message_style;
	struct mlk_message msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.lines = text,
		.linesz = 2,
		.style = &style
	};

	style.background = 0xf85d80ff;
	style.border = 0xd94a69ff;
	style.color = 0xffffffff;

	mlk_message_query(&msg, NULL, &msg.h);
	run(&msg);
}

static void
quit(void)
{
	mlk_example_finish();
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
