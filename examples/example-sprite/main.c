/*
 * example-sprite.c -- example on how to use sprites
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

#include <stdio.h>

#include <core/core.h>
#include <core/event.h>
#include <core/game.h>
#include <core/image.h>
#include <core/key.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/image.h>
#include <core/sprite.h>
#include <core/state.h>
#include <core/sys.h>
#include <core/texture.h>
#include <core/util.h>
#include <core/window.h>

#include <ui/align.h>
#include <ui/label.h>
#include <ui/ui.h>

#define W       1280
#define H       720
#define PATH(r) util_pathf("%s/mlk-adventure/%s", sys_dir(SYS_DIR_DATA), r)
#define HEADER "Keys: <Left>/<Right> and <Up/Down> to select a column/row. Current: %u, %u (total %u/%u)"

static char msg[512];
static struct texture texture;
static struct sprite sprite;
static unsigned int row, column;

static struct label help = {
	.x = 10,
	.y = 10,
	.text = msg,
	.flags = LABEL_FLAGS_SHADOW
};

static void
changed(void)
{
	snprintf(msg, sizeof (msg), HEADER, column, row, sprite.ncols, sprite.nrows);
}

static void
init(void)
{
	if (core_init("fr.malikania", "sprite") < 0 || ui_init() < 0)
		panic();
	if (window_open("Example - Sprite", W, H) < 0)
		panic();
	if (image_open(&texture, PATH("sprites/people.png")) < 0)
		panic();

	sprite_init(&sprite, &texture, 48, 48);
}

static void
handle(struct state *st, const union event *ev)
{
	(void)st;

	switch (ev->type) {
	case EVENT_KEYDOWN:
		switch (ev->key.key) {
		case KEY_LEFT:
			if (column > 0)
				column--;
			break;
		case KEY_RIGHT:
			if (column + 1 < sprite.ncols)
				column++;
			break;
		case KEY_UP:
			if (row > 0)
				row--;
			break;
		case KEY_DOWN:
			if (row + 1 < sprite.nrows)
				row++;
			break;
		default:
			break;
		}

		changed();
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

	int x, y;

	painter_set_color(0xebede9ff);
	painter_clear();
	align(ALIGN_CENTER, &x, &y, sprite.cellw, sprite.cellh, 0, 0, W, H);
	sprite_draw(&sprite, row, column, x, y);
	label_draw(&help);
	painter_present();
}

static void
run(void)
{
	struct state state = {
		.handle = handle,
		.draw = draw
	};

	changed();

	game_push(&state);
	game_loop();
}

static void
quit(void)
{
	texture_finish(&texture);
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
