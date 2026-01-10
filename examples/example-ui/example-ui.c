/*
 * example-action.c -- example on how to use automatic actions
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

#include <mlk/core/clock.h>
#include <mlk/core/err.h>
#include <mlk/core/event.h>
#include <mlk/core/game.h>
#include <mlk/core/maths.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/state.h>
#include <mlk/core/sys.h>
#include <mlk/core/trace.h>
#include <mlk/core/util.h>
#include <mlk/core/window.h>

#include <mlk/ui/align.h>
#include <mlk/ui/button.h>
#include <mlk/ui/checkbox.h>
#include <mlk/ui/frame.h>
#include <mlk/ui/label.h>
#include <mlk/ui/notify.h>
#include <mlk/ui/style.h>
#include <mlk/ui/ui.h>

#include <mlk/example/example.h>
#include <mlk/example/glower.h>
#include <mlk/example/registry.h>

#include "button-glower.h"

#define FRAME_ORIGIN_X  (10)
#define FRAME_ORIGIN_Y  (10)
#define FRAME_WIDTH     (400)
#define FRAME_HEIGHT    (200)

#define HEADER_HEIGHT   (32)

#define ELEMENT_HEIGHT  (20)

#define PADDING         (10)

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
 * |                             [Hello][ Quit ] |
 * +---------------------------------------------+
 */
static struct {
	struct {
		int active;
		int x;
		int y;
	} motion;

	struct {
		struct mlk_frame frame;
	} panel;

	struct {
		struct mlk_label label;
	} header;

	struct {
		struct mlk_checkbox cb;
		struct mlk_label label;
	} autosave;

	struct {
		/* [Hello] with default style*/
		struct mlk_button hello;

		/* [Quit] with custom style color. */
		struct mlk_button_style quit_style;
		struct mlk_button quit;

		/*
		 * [Download free RAM] with custom style using a delegate.
		 */
		struct button_glower download_glower;
		struct mlk_button download;
	} buttons;
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
			.y = FRAME_ORIGIN_Y
		}
	},
	.autosave = {
		.cb = {
			.w = ELEMENT_HEIGHT,
			.h = ELEMENT_HEIGHT
		},
		.label = {
			.text = "Auto save game"
		}
	},
	.buttons = {
		.hello = {
			.text = "Hello",
			.h = ELEMENT_HEIGHT
		},
		.quit_style = {
			.background = 0x24aed6ff,
			.color = 0xffffffff
		},
		.quit = {
			.text = "Quit",
			.h = ELEMENT_HEIGHT,
			.style = &ui.buttons.quit_style
		},
		.download_glower = {
			.glower = {
				.start = BUTTON_STYLE_GLOW_COLOR_1,
				.end = BUTTON_STYLE_GLOW_COLOR_2,
				.delay = BUTTON_STYLE_GLOW_DELAY
			},
			.style = {
				.color = 0xffffffff,
				.border = BUTTON_STYLE_GLOW_COLOR_1,
				.background = BUTTON_STYLE_GLOW_COLOR_1,
				.border_size = 2
			}
		},
		.download = {
			.w = 180,
			.h = 32,
			.text = "!! Download free RAM !!",
		}
	}
};

static void
resize_header(void)
{
	struct mlk_frame *f = &ui.panel.frame;
	struct mlk_label *l = &ui.header.label;
	unsigned int w, h;

	/* Header. */
	mlk_label_query(l, &w, &h);
	mlk_align(MLK_ALIGN_LEFT, &l->x, &l->y, w, h, f->x, f->y, f->w, HEADER_HEIGHT);

	l->x += PADDING;
}

static void
resize_autosave(void)
{
	struct mlk_frame *f = &ui.panel.frame;
	struct mlk_checkbox *c = &ui.autosave.cb;
	struct mlk_label *l = &ui.autosave.label;

	c->x = f->x + PADDING;
	c->y = f->y + HEADER_HEIGHT + PADDING;

	l->x = c->x + c->w + PADDING;
	l->y = c->y;
}

static void
resize_button(void)
{
	struct mlk_frame *f = &ui.panel.frame;
	struct mlk_button *quit = &ui.buttons.quit;
	struct mlk_button *hello = &ui.buttons.hello;
	struct mlk_button *dl = &ui.buttons.download;

	/* Buttons takes 1/4 of the frame width. */
	quit->w = hello->w = f->w / 4;

	mlk_align(MLK_ALIGN_BOTTOM_RIGHT, &quit->x, &quit->y, quit->w, quit->h,
	    f->x, f->y, f->w, f->h);

	quit->x -= PADDING;
	quit->y -= PADDING;

	/* Hello is immediately left. */
	hello->x = quit->x - quit->w - PADDING;
	hello->y = quit->y;

	/* Download free ram is at the bottom center. */
	mlk_align(MLK_ALIGN_BOTTOM, &dl->x, &dl->y, dl->w, dl->h, 0, 0, mlk_window.w, mlk_window.h - 10);
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
	    x,
	    y,
	    ui.panel.frame.x,
	    ui.panel.frame.y,
	    ui.panel.frame.w,
	    HEADER_HEIGHT
	);
}

static void
handle(struct mlk_state *st, const union mlk_event *ev)
{
	(void)st;

	mlk_ui_handle(ev);

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
			mlk_window_set_cursor(MLK_WINDOW_CURSOR_SIZE);
		}
		break;
	case MLK_EVENT_CLICKUP:
		ui.motion.active = 0;
		mlk_window_set_cursor(MLK_WINDOW_CURSOR_ARROW);
		break;
	default:
		break;
	}

	mlk_checkbox_handle(&ui.autosave.cb, ev);

	if (mlk_button_handle(&ui.buttons.quit, ev))
		mlk_game_quit();
	if (mlk_button_handle(&ui.buttons.hello, ev))
		mlk_notify(
		    &mlk_registry_textures[MLK_REGISTRY_TEXTURE_SWORD],
		    "Hello",
		    "Hello world!"
		);
	if (mlk_button_handle(&ui.buttons.download, ev))
		mlk_notify(
		    &mlk_registry_textures[MLK_REGISTRY_TEXTURE_SWORD],
		    "Complete",
		    "16GB of RAM successfully downloaded!"
		);
}

static void
update(struct mlk_state *st, unsigned int ticks)
{
	(void)st;

	mlk_button_update(&ui.buttons.quit, ticks);
	mlk_button_update(&ui.buttons.hello, ticks);
	mlk_button_update(&ui.buttons.download, ticks);

	mlk_notify_update(ticks);
}

static void
draw(struct mlk_state *st)
{
	(void)st;

	mlk_painter_set_color(MLK_EXAMPLE_BG);
	mlk_painter_clear();
	mlk_frame_draw(&ui.panel.frame);
	mlk_label_draw(&ui.header.label);
	mlk_checkbox_draw(&ui.autosave.cb);
	mlk_label_draw(&ui.autosave.label);
	mlk_button_draw(&ui.buttons.hello);
	mlk_button_draw(&ui.buttons.quit);
	mlk_button_draw(&ui.buttons.download);
	mlk_notify_draw();
	mlk_painter_present();
}

static void
init(void)
{
	if (mlk_example_init("example-ui") < 0)
		mlk_panic();

	button_glower_init(&ui.buttons.download_glower, &ui.buttons.download);
}

static void
run(void)
{
	struct mlk_state state = {
		.handle = handle,
		.update = update,
		.draw = draw
	};

	resize();

	mlk_game_init();
	mlk_game_loop(&state);
}

static void
quit(void)
{
	mlk_example_finish();
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
