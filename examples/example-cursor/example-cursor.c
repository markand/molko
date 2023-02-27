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

#define W 1280
#define H 720

static struct state *states[1];
static char help_text[128];
static enum window_cursor cursor = WINDOW_CURSOR_ARROW;

static struct label help = {
	.x = 10,
	.y = 10,
	.text = help_text,
	.flags = LABEL_FLAGS_SHADOW
};

static void
init(void)
{
	if (mlk_core_init("fr.malikania", "example-cursor") < 0 || ui_init() < 0)
		panic();
	if (window_open("Example - Cursor", W, H) < 0)
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
handle(struct state *st, const union mlk_event *ev)
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
			if (cursor + 1 < WINDOW_CURSOR_LAST)
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
draw(struct state *st)
{
	(void)st;

	mlk_painter_set_color(0xebede9ff);
	mlk_painter_clear();
	label_draw(&help);
	mlk_painter_present();
}

static void
run(void)
{
	struct state state = {
		.handle = handle,
		.draw = draw
	};

	change(cursor);

	mlk_game_init(states, UTIL_SIZE(states));
	mlk_game_push(&state);
	mlk_game_loop();
}

static void
quit(void)
{
	window_finish();
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
