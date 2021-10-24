/*
 * example-debug.c -- example on how to use debug interface
 *
 * Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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

#include <core/core.h>
#include <core/event.h>
#include <core/game.h>
#include <core/window.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/state.h>
#include <core/util.h>

#include <ui/debug.h>
#include <ui/theme.h>
#include <ui/ui.h>

#define W 1280
#define H 720

static int mouse_x;
static int mouse_y;

static void
init(void)
{
	if (core_init("fr.malikania", "example-debug") < 0 || ui_init() < 0)
		panic();
	if (window_open("Example - Debug", W, H) < 0)
		panic();

	debug_options.enable = 1;
}

static void
handle(struct state *st, const union event *ev)
{
	(void)st;

	switch (ev->type) {
	case EVENT_MOUSE:
		mouse_x = ev->mouse.x;
		mouse_y = ev->mouse.y;
		break;
	case EVENT_QUIT:
		game_quit();
		break;
	default:
		break;
	}
}

static void
draw(struct state *st)
{
	(void)st;

	struct debug_report report = {0};

	painter_set_color(0x4f8fbaff);
	painter_clear();
	debugf(&report, "Game running.");
	debugf(&report, "mouse: %d, %d", mouse_x, mouse_y);
	painter_present();
}

static void
run(void)
{
	struct state state = {
		.handle = handle,
		.draw = draw
	};

	game_push(&state);
	game_loop();
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

