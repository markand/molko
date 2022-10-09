/*
 * example-font.c -- show how to use fonts
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
#include <core/font.h>
#include <core/game.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/state.h>
#include <core/sys.h>
#include <core/texture.h>
#include <core/util.h>
#include <core/window.h>

#include <ui/theme.h>
#include <ui/ui.h>

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

static struct state *states[1];
static int ci = 0;
static enum font_style style = FONT_STYLE_ANTIALIASED;

static void
init(void)
{
	if (core_init("fr.malikania", "example-font") < 0 || ui_init() < 0)
		panic();
	if (window_open("Example - Font", W, H) < 0)
		panic();
}

static void
handle(struct state *st, const union event *ev)
{
	(void)st;

	switch (ev->type) {
	case EVENT_KEYDOWN:
		switch (ev->key.key) {
		case KEY_LEFT:
			if (ci > 0)
				ci--;
			break;
		case KEY_RIGHT:
			if ((size_t)ci < UTIL_SIZE(colors))
				ci++;
			break;
		case KEY_SPACE:
			if (style == FONT_STYLE_ANTIALIASED)
				style = FONT_STYLE_NONE;
			else
				style = FONT_STYLE_ANTIALIASED;

			theme_default()->fonts[THEME_FONT_INTERFACE]->style = style;
		default:
			break;
		}
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

	struct font *font = theme_default()->fonts[THEME_FONT_INTERFACE];
	struct texture tex;

	painter_set_color(0xffffffff);
	painter_clear();

	if (font_render(font, &tex, "Example of text. Use <Left>/<Right> to change color and <Space> to toggle antialiasing.", colors[ci]) < 0)
		panic();

	texture_draw(&tex, 10, 10);
	painter_present();
	texture_finish(&tex);
}

static void
run(void)
{
	struct state state = {
		.handle = handle,
		.draw = draw
	};

	game_init(states, UTIL_SIZE(states));
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

	return 0;
}
