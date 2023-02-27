/*
 * example-action.c -- example on how to use automatic actions
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
#include <mlk/core/game.h>
#include <mlk/core/maths.h>
#include <mlk/core/panic.h>
#include <mlk/core/painter.h>
#include <mlk/core/state.h>
#include <mlk/core/sys.h>
#include <mlk/core/util.h>
#include <mlk/core/window.h>

#include <mlk/ui/align.h>
#include <mlk/ui/button.h>
#include <mlk/ui/checkbox.h>
#include <mlk/ui/frame.h>
#include <mlk/ui/label.h>
#include <mlk/ui/theme.h>
#include <mlk/ui/ui.h>

#define W               (1280)
#define H               (720)

#define FRAME_ORIGIN_X  (10)
#define FRAME_ORIGIN_Y  (10)
#define FRAME_WIDTH     (400)
#define FRAME_HEIGHT    (200)

#define HEADER_HEIGHT   (32)

#define ELEMENT_HEIGHT  (20)

/*
 * We design a basic UI like this.
 *
 *                    FRAME_WIDTH
 * +---------------------------------------------+--
 * | Title                                       | | HEADER_HEIGHT
 * +---------------------------------------------+--
 * | [x] Auto save                               |
 * |                                             |
 * |                                             |
 * |                                    [ Quit ] |
 * +---------------------------------------------+
 */
static struct {
	struct {
		int active;
		int x;
		int y;
	} motion;

	struct {
		struct frame frame;
	} panel;

	struct {
		struct label label;
	} header;

	struct {
		struct checkbox cb;
		struct label label;
	} autosave;

	struct {
		struct button button;
	} quit;
} ui = {
	.panel = {
		.frame = {
			.x = FRAME_ORIGIN_X,
			.y = FRAME_ORIGIN_Y,
			.w = FRAME_WIDTH,
			.h = FRAME_HEIGHT
		}
	},
	.header = {
		.label = {
			.text = "Preferences",
			.x = FRAME_ORIGIN_X,
			.y = FRAME_ORIGIN_Y,
			.flags = LABEL_FLAGS_SHADOW,
		}
	},
	.autosave = {
		.cb = {
			.w = ELEMENT_HEIGHT,
			.h = ELEMENT_HEIGHT
		},
		.label = {
			.text = "Auto save game",
			.flags = LABEL_FLAGS_SHADOW,
		}
	},
	.quit = {
		.button = {
			.text = "Quit",
			.h = ELEMENT_HEIGHT
		}
	}
};

static struct mlk_state *states[1];

static void
init(void)
{
	if (mlk_core_init("fr.malikania", "example-ui") < 0 || ui_init() < 0)
		mlk_panic();
	if (window_open("Example - UI", W, H) < 0)
		mlk_panic();
}

static void
resize_header(void)
{
	struct frame *f = &ui.panel.frame;
	struct label *l = &ui.header.label;
	unsigned int w, h;

	/* Header. */
	label_query(l, &w, &h);
	align(ALIGN_LEFT, &l->x, &l->y, w, h, f->x, f->y, f->w, HEADER_HEIGHT);

	l->x += theme_default()->padding;
}

static void
resize_autosave(void)
{
	unsigned int padding = theme_default()->padding;
	struct frame *f = &ui.panel.frame;
	struct checkbox *c = &ui.autosave.cb;
	struct label *l = &ui.autosave.label;

	c->x = f->x + padding;
	c->y = f->y + HEADER_HEIGHT + padding;

	l->x = c->x + c->w + padding;
	l->y = c->y;
}

static void
resize_button(void)
{
	unsigned int padding = theme_default()->padding;
	struct frame *f = &ui.panel.frame;
	struct button *b = &ui.quit.button;

	/* Button. */
	b->w = f->w / 4;

	align(ALIGN_BOTTOM_RIGHT, &b->x, &b->y, b->w, b->h,
	    f->x, f->y, f->w, f->h);

	b->x -= padding;
	b->y -= padding;
}

static void
resize(void)
{
	resize_header();
	resize_autosave();
	resize_button();
}

static int
headerclick(int x, int y)
{
	return mlk_maths_is_boxed(
	    ui.panel.frame.x,
	    ui.panel.frame.y,
	    ui.panel.frame.w,
	    HEADER_HEIGHT,
	    x,
	    y
	);
}

static void
handle(struct mlk_state *st, const union mlk_event *ev)
{
	(void)st;

	switch (ev->type) {
	case MLK_EVENT_QUIT:
		mlk_game_quit();
		break;
	case MLK_EVENT_MOUSE:
		if (ui.motion.active) {
			ui.panel.frame.x += ev->mouse.x - ui.motion.x;
			ui.panel.frame.y += ev->mouse.y - ui.motion.y;
			ui.motion.x = ev->mouse.x;
			ui.motion.y = ev->mouse.y;
			resize();
		}
		break;
	case MLK_EVENT_CLICKDOWN:
		if (headerclick(ev->click.x, ev->click.y)) {
			ui.motion.active = 1;
			ui.motion.x = ev->click.x;
			ui.motion.y = ev->click.y;
			window_set_cursor(WINDOW_CURSOR_SIZE);
		}
		break;
	case MLK_EVENT_CLICKUP:
		ui.motion.active = 0;
		window_set_cursor(WINDOW_CURSOR_ARROW);
		break;
	default:
		break;
	}

	checkbox_handle(&ui.autosave.cb, ev);

	if (button_handle(&ui.quit.button, ev))
		mlk_game_quit();
}

static void
draw(struct mlk_state *st)
{
	(void)st;

	mlk_painter_set_color(0xffffffff);
	mlk_painter_clear();
	frame_draw(&ui.panel.frame);
	label_draw(&ui.header.label);
	checkbox_draw(&ui.autosave.cb);
	label_draw(&ui.autosave.label);
	button_draw(&ui.quit.button);
	mlk_painter_present();
}

static void
run(void)
{
	struct mlk_state state = {
		.handle = handle,
		.draw = draw
	};

	resize();

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
