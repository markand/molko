/*
 * example-debug.c -- example on how to use debug interface
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

#include <mlk/core/core.h>
#include <mlk/core/err.h>
#include <mlk/core/event.h>
#include <mlk/core/game.h>
#include <mlk/core/window.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/state.h>
#include <mlk/core/util.h>

#include <mlk/ui/debug.h>
#include <mlk/ui/theme.h>
#include <mlk/ui/ui.h>

#include <mlk/example/example.h>

static struct mlk_state *states[1];
static int mouse_x;
static int mouse_y;

static void
init(void)
{
	int err;

	if ((err = mlk_example_init("example-debug")) < 0)
		mlk_panicf("mlk_example_init: %s", mlk_err_string(err));

	mlk_debug_options.enable = 1;
}

static void
handle(struct mlk_state *st, const union mlk_event *ev)
{
	(void)st;

	switch (ev->type) {
	case MLK_EVENT_MOUSE:
		mouse_x = ev->mouse.x;
		mouse_y = ev->mouse.y;
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

	struct mlk_debug_report report = {0};

	mlk_painter_set_color(0x4f8fbaff);
	mlk_painter_clear();
	mlk_debugf(&report, "Game running.");
	mlk_debugf(&report, "mouse: %d, %d", mouse_x, mouse_y);
	mlk_painter_present();
}

static void
run(void)
{
	struct mlk_state state = {
		.handle = handle,
		.draw = draw
	};

	mlk_game_init(states, MLK_UTIL_SIZE(states));
	mlk_game_push(&state);
	mlk_game_loop();
}

static void
quit(void)
{
	mlk_window_finish();
	ui_finish();
	mlk_core_finish();
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

