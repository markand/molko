/*
 * example-trace.c -- example on how to use custom trace handlers
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

#include <core/core.h>
#include <core/event.h>
#include <core/game.h>
#include <core/sys.h>
#include <core/window.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/state.h>
#include <core/trace.h>
#include <core/util.h>

#include <ui/theme.h>
#include <ui/ui.h>

#include "trace_hud.h"

#define W 1280
#define H 720

static void
init(void)
{
	if (core_init("fr.malikania", "example-trace") < 0 || ui_init() < 0)
		panic();
	if (window_open("Example - Trace", W, H) < 0)
		panic();

	trace_handler = trace_hud_handler;
}

static void
handle(struct state *st, const union event *ev)
{
	(void)st;

	switch (ev->type) {
	case EVENT_KEYDOWN:
		switch (ev->key.key) {
		case KEY_ESCAPE:
			trace_hud_clear();
			break;
		default:
			tracef("keydown pressed: %d", ev->key.key);
			break;
		}
		break;
	case EVENT_CLICKDOWN:
		tracef("click at %d,%d", ev->click.x, ev->click.y);
		break;
	case EVENT_QUIT:
		game_quit();
		break;
	default:
		break;
	}
}

static void
update(struct state *st, unsigned int ticks)
{
	(void)st;

	trace_hud_update(ticks);
}

static void
draw(struct state *st)
{
	(void)st;

	painter_set_color(0x4f8fbaff);
	painter_clear();
	trace_hud_draw();
	painter_present();
}

static void
run(void)
{
	struct state state = {
		.handle = handle,
		.update = update,
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

