/*
 * example-action.c -- example on how to use automatic actions
 *
 * Copyright (c) 2020 David Demelier <markand@malikania.fr>
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

#include <core/action.h>
#include <core/clock.h>
#include <core/core.h>
#include <core/event.h>
#include <core/maths.h>
#include <core/panic.h>
#include <core/painter.h>
#include <core/sys.h>
#include <core/util.h>
#include <core/window.h>

#include <ui/align.h>
#include <ui/button.h>
#include <ui/checkbox.h>
#include <ui/frame.h>
#include <ui/label.h>
#include <ui/theme.h>
#include <ui/ui.h>

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
	struct action_stack st;

	struct {
		bool active;
		int x;
		int y;
	} motion;

	struct {
		struct frame frame;
		struct action act;
	} panel;

	struct {
		struct label label;
		struct action act;
	} header;

	struct {
		struct checkbox cb;
		struct action cb_act;
		struct label label;
		struct action label_act;
	} autosave;

	struct {
		struct button button;
		struct action act;
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

static void
init(void)
{
	if (!core_init() || !ui_init())
		panic();
	if (!window_open("Example - UI", W, H))
		panic();
}

static void
resize_header(void)
{
	struct frame *h = &ui.panel.frame;
	struct label *l = &ui.header.label;

	/* Header. */
	label_query(l);
	align(ALIGN_LEFT, &l->x, &l->y, l->w, l->h, h->x, h->y, h->w, HEADER_HEIGHT);

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

	l->w = f->w - c->w - padding;
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

static void
prepare(void)
{
	/* Frame. */
	frame_action(&ui.panel.frame, &ui.panel.act);

	/* Header title. */
	label_action(&ui.header.label, &ui.header.act);

	/* Button quit. */
	button_action(&ui.quit.button, &ui.quit.act);

	/* Autosave. */
	checkbox_action(&ui.autosave.cb, &ui.autosave.cb_act);
	label_action(&ui.autosave.label, &ui.autosave.label_act);

	/* Add all UI elements. */
	action_stack_add(&ui.st, &ui.panel.act);
	action_stack_add(&ui.st, &ui.header.act);
	action_stack_add(&ui.st, &ui.autosave.cb_act);
	action_stack_add(&ui.st, &ui.autosave.label_act);
	action_stack_add(&ui.st, &ui.quit.act);
}

static bool
headerclick(int x, int y)
{
	return maths_is_boxed(
	    ui.panel.frame.x,
	    ui.panel.frame.y,
	    ui.panel.frame.w,
	    HEADER_HEIGHT,
	    x,
	    y
	);
}

static void
run(void)
{
	struct clock clock = {0};

	clock_start(&clock);

	prepare();
	resize();

	while (ui.quit.button.state != BUTTON_STATE_ACTIVATED) {
		unsigned int elapsed = clock_elapsed(&clock);

		clock_start(&clock);

		for (union event ev; event_poll(&ev); ) {
			switch (ev.type) {
			case EVENT_QUIT:
				return;
			case EVENT_MOUSE:
				if (ui.motion.active) {
					ui.panel.frame.x += ev.mouse.x - ui.motion.x;
					ui.panel.frame.y += ev.mouse.y - ui.motion.y;
					ui.motion.x = ev.mouse.x;
					ui.motion.y = ev.mouse.y;
					resize();
				}
				break;
			case EVENT_CLICKDOWN:
				if (headerclick(ev.click.x, ev.click.y)) {
					ui.motion.active = true;
					ui.motion.x = ev.click.x;
					ui.motion.y = ev.click.y;
				}
				else
					action_stack_handle(&ui.st, &ev);
				break;
			case EVENT_CLICKUP:
				ui.motion.active = false;
				/* Fallthrough. */
			default:
				action_stack_handle(&ui.st, &ev);
				break;
			}
		}

		painter_set_color(0xffffffff);
		painter_clear();
		action_stack_update(&ui.st, elapsed);
		action_stack_draw(&ui.st);
		painter_present();

		if ((elapsed = clock_elapsed(&clock)) < 20)
			delay(20 - elapsed);
	}
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
