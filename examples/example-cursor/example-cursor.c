/*
 * example-cursor.c -- example on how to change cursor
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

#include <stdio.h>

#include <mlk/core/core.h>
#include <mlk/core/err.h>
#include <mlk/core/event.h>
#include <mlk/core/game.h>
#include <mlk/core/key.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/state.h>
#include <mlk/core/sys.h>
#include <mlk/core/util.h>
#include <mlk/core/window.h>

#include <mlk/ui/label.h>
#include <mlk/ui/ui.h>

#include <mlk/example/example.h>

static struct mlk_state *states[1];
static char help_text[128];
static enum mlk_window_cursor cursor = MLK_WINDOW_CURSOR_ARROW;

static struct mlk_label help = {
	.x = 10,
	.y = 10,
	.text = help_text
};

static void
init(void)
{
	int err;

	if ((err = mlk_example_init("example-cursor")) < 0)
		mlk_panicf("mlk_example_init: %s", mlk_err_string(err));
}

static void
change(enum mlk_window_cursor cursor)
{
	static const char *names[] = {
		[MLK_WINDOW_CURSOR_ARROW]       = "MLK_WINDOW_CURSOR_ARROW",
		[MLK_WINDOW_CURSOR_EDIT]        = "MLK_WINDOW_CURSOR_EDIT",
		[MLK_WINDOW_CURSOR_WAIT]        = "MLK_WINDOW_CURSOR_WAIT",
		[MLK_WINDOW_CURSOR_CROSSHAIR]   = "MLK_WINDOW_CURSOR_CROSSHAIR",
		[MLK_WINDOW_CURSOR_SIZE]        = "MLK_WINDOW_CURSOR_SIZE",
		[MLK_WINDOW_CURSOR_NO]          = "MLK_WINDOW_CURSOR_NO",
		[MLK_WINDOW_CURSOR_HAND]        = "MLK_WINDOW_CURSOR_HAND"
	};

	snprintf(help_text, sizeof (help_text), "Keys: <Left>/<Right> to change cursor. Current: %s", names[cursor]);
	mlk_window_set_cursor(cursor);
}

static void
handle(struct mlk_state *st, const union mlk_event *ev)
{
	(void)st;

	switch (ev->type) {
	case MLK_EVENT_KEYDOWN:
		switch (ev->key.key) {
		case MLK_KEY_LEFT:
			if (cursor > 0)
				change(--cursor);
			break;
		case MLK_KEY_RIGHT:
			if (cursor + 1 < MLK_WINDOW_CURSOR_LAST)
				change(++cursor);
			break;
		default:
			break;
		}

		break;
	case MLK_EVENT_QUIT:
		mlk_game_quit();
		break;
	default:
		break;
	}
}

static void
draw(struct mlk_state *st)
{
	(void)st;

	mlk_painter_set_color(MLK_EXAMPLE_BG);
	mlk_painter_clear();
	mlk_label_draw(&help);
	mlk_painter_present();
}

static void
run(void)
{
	struct mlk_state state = {
		.handle = handle,
		.draw = draw
	};

	change(cursor);

	mlk_game_init(states, MLK_UTIL_SIZE(states));
	mlk_game_push(&state);
	mlk_game_loop();
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
	run();
	quit();
}
