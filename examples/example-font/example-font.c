/*
 * example-font.c -- show how to use fonts
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
#include <mlk/core/event.h>
#include <mlk/core/font.h>
#include <mlk/core/game.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/state.h>
#include <mlk/core/sys.h>
#include <mlk/core/texture.h>
#include <mlk/core/util.h>
#include <mlk/core/window.h>

#include <mlk/ui/theme.h>
#include <mlk/ui/ui.h>

#define W       (1280)
#define H       (720)

/* Friendly taken from: https://lospec.com/palette-list/apollo */
static const unsigned long colors[] = {
	0x3c5e8bff,     /* Blue. */
	0x468232ff,     /* Green. */
	0xad7757ff,     /* Light brown. */
	0x884b2bff,     /* Brown. */
	0x752438ff,     /* Red. */
	0x7a367bff,     /* Magenta. */
	0x151d28ff,     /* Dark */
	0xc7cfccff,     /* Christian Grey. */
};

static struct mlk_state *states[1];
static int ci = 0;
static enum mlk_font_style style = MLK_FONT_STYLE_ANTIALIASED;

static void
init(void)
{
	if (mlk_core_init("fr.malikania", "example-font") < 0 || ui_init() < 0)
		mlk_panic();
	if (window_open("Example - Font", W, H) < 0)
		mlk_panic();
}

static void
handle(struct mlk_state *st, const union mlk_event *ev)
{
	(void)st;

	switch (ev->type) {
	case MLK_EVENT_KEYDOWN:
		switch (ev->key.key) {
		case MLK_KEY_LEFT:
			if (ci > 0)
				ci--;
			break;
		case MLK_KEY_RIGHT:
			if ((size_t)ci < UTIL_SIZE(colors))
				ci++;
			break;
		case MLK_KEY_SPACE:
			if (style == MLK_FONT_STYLE_ANTIALIASED)
				style = MLK_FONT_STYLE_NONE;
			else
				style = MLK_FONT_STYLE_ANTIALIASED;

			theme_default()->fonts[THEME_FONT_INTERFACE]->style = style;
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

	struct mlk_font *font = theme_default()->fonts[THEME_FONT_INTERFACE];
	struct mlk_texture tex;

	mlk_painter_set_color(0xffffffff);
	mlk_painter_clear();

	if (mlk_font_render(font, &tex, "Example of text. Use <Left>/<Right> to change color and <Space> to toggle antialiasing.", colors[ci]) < 0)
		mlk_panic();

	mlk_texture_draw(&tex, 10, 10);
	mlk_painter_present();
	mlk_texture_finish(&tex);
}

static void
run(void)
{
	struct mlk_state state = {
		.handle = handle,
		.draw = draw
	};

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

	return 0;
}
