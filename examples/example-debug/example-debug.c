/*
 * example-debug.c -- example on how to use debug interface
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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
#include <mlk/core/texture.h>
#include <mlk/core/util.h>

#include <mlk/ui/align.h>
#include <mlk/ui/debug.h>
#include <mlk/ui/label.h>
#include <mlk/ui/ui.h>

#include <mlk/example/example.h>

static enum mlk_align align;
static int mouse_x;
static int mouse_y;
static struct mlk_label help = {
	.text = "Keys: <Left>/<Right> arrows keys to change debug queue position."
};

/*
 * Example of debug style drawing function to adjust position on the screen.
 */
static void
debug_draw(const char *line, unsigned int n)
{
	int x, y;
	struct mlk_texture tex;
	struct mlk_font *font;
	unsigned long color;
	unsigned int pad;

	font = &mlk_ui_fonts[MLK_UI_FONT_INTERFACE];
	color = mlk_debug_style.color;
	pad = mlk_debug_style.padding;

	if (mlk_font_render(font, &tex, line, color) < 0)
		mlk_panic();

	/* Compute x position according to the user choice and text length. */
	mlk_align(align, &x, NULL, tex.w, tex.h, pad, 0, mlk_window.w - (pad * 2), 0);

	/* Compute y according to padding, n and text height. */
	y = (pad * (n + 1)) + (tex.h * n);

	mlk_texture_draw(&tex, x, y);
	mlk_texture_finish(&tex);
}

static void
init(void)
{
	unsigned int w, h;

	if (mlk_example_init("example-debug") < 0)
		mlk_panic();

	mlk_label_query(&help, &w, &h);
	mlk_align(MLK_ALIGN_CENTER, &help.x, &help.y, w, h, 0, 0, mlk_window.w, mlk_window.h);

	mlk_debug_options.enable = 1;
	mlk_debug_style.draw = debug_draw;
}

static void
handle(struct mlk_state *, const union mlk_event *ev)
{
	switch (ev->type) {
	case MLK_EVENT_MOUSE:
		mouse_x = ev->mouse.x;
		mouse_y = ev->mouse.y;
		break;
	case MLK_EVENT_KEYDOWN:
		switch (ev->key.key) {
		case MLK_KEY_LEFT:
			align = MLK_ALIGN_TOP_LEFT;
			break;
		case MLK_KEY_RIGHT:
			align = MLK_ALIGN_TOP_RIGHT;
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
draw(struct mlk_state *)
{
	struct mlk_debug_report report = {};

	mlk_painter_set_color(MLK_EXAMPLE_BG);
	mlk_painter_clear();
	mlk_label_draw(&help);
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

	mlk_game_init();
	mlk_game_loop(&state);
}

static void
quit(void)
{
	mlk_example_finish();
}

int
main(int, char **)
{
	init();
	run();
	quit();
}
