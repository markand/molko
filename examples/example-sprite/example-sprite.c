/*
 * example-sprite.c -- example on how to use sprites
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
#include <mlk/core/image.h>
#include <mlk/core/key.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/image.h>
#include <mlk/core/sprite.h>
#include <mlk/core/state.h>
#include <mlk/core/sys.h>
#include <mlk/core/texture.h>
#include <mlk/core/util.h>
#include <mlk/core/window.h>

#include <mlk/ui/align.h>
#include <mlk/ui/label.h>
#include <mlk/ui/ui.h>

#include <assets/sprites/people.h>

#define W       1280
#define H       720
#define HEADER "Keys: <Left>/<Right> and <Up/Down> to select a column/row. Current: %u, %u (total %u/%u)"

static char msg[512];
static struct texture texture;
static struct mlk_sprite sprite;
static unsigned int row, column;
static struct state *states[1];

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
	if (mlk_core_init("fr.malikania", "example-sprite") < 0 || ui_init() < 0)
		mlk_panic();
	if (window_open("Example - Sprite", W, H) < 0)
		mlk_panic();
	if (mlk_image_openmem(&texture, assets_sprites_people, sizeof (assets_sprites_people)) < 0)
		mlk_panic();

	mlk_sprite_init(&sprite, &texture, 48, 48);
}

static void
handle(struct state *st, const union mlk_event *ev)
{
	(void)st;

	switch (ev->type) {
	case MLK_EVENT_KEYDOWN:
		switch (ev->key.key) {
		case MLK_KEY_LEFT:
			if (column > 0)
				column--;
			break;
		case MLK_KEY_RIGHT:
			if (column + 1 < sprite.ncols)
				column++;
			break;
		case MLK_KEY_UP:
			if (row > 0)
				row--;
			break;
		case MLK_KEY_DOWN:
			if (row + 1 < sprite.nrows)
				row++;
			break;
		default:
			break;
		}

		changed();
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

	int x, y;

	mlk_painter_set_color(0xebede9ff);
	mlk_painter_clear();
	align(ALIGN_CENTER, &x, &y, sprite.cellw, sprite.cellh, 0, 0, W, H);
	mlk_sprite_draw(&sprite, row, column, x, y);
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

	changed();

	mlk_game_init(states, UTIL_SIZE(states));
	mlk_game_push(&state);
	mlk_game_loop();
}

static void
quit(void)
{
	texture_finish(&texture);
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
