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

#include <core/clock.h>
#include <core/event.h>
#include <core/panic.h>
#include <core/painter.h>
#include <core/sys.h>
#include <core/util.h>
#include <core/window.h>

#include <ui/button.h>
#include <ui/checkbox.h>
#include <ui/frame.h>
#include <ui/label.h>
#include <ui/theme.h>

#define W       (1280)
#define H       (720)

static struct frame frame = {
	.x = 10,
	.y = 10,
	.w = 400,
	.h = 200
};

static struct label title = {
	.text = "Preferences",
	.x = 10,
	.y = 10,
	.w = 400,
	.h = 200,
	.flags = LABEL_FLAGS_SHADOW,
	.align = ALIGN_TOP_LEFT
};

static struct {
	struct checkbox cb;
	struct label label;
} autosave = {
	.cb = {
		.x = 20,
		.y = 60,
		.w = 20,
		.h = 20
	},
	.label = {
		.text = "Auto save game",
		.x = 20 + 20,
		.y = 60,
		.w = 200,
		.h = 20,
		.align = ALIGN_LEFT,
		.flags = LABEL_FLAGS_SHADOW
	}
};

static struct button btquit = {
	.text = "Quit",
	.w = 50,
	.h = 20
};

static void
init(void)
{
	if (!sys_init() ||
	    !window_init("Example - UI", W, H) ||
	    !theme_init())
		panic();
}

static void
run(void)
{
	struct clock clock = {0};

	clock_start(&clock);

	for (;;) {
		unsigned int elapsed = clock_elapsed(&clock);

		clock_start(&clock);

		for (union event ev; event_poll(&ev); ) {
			switch (ev.type) {
			case EVENT_QUIT:
				return;
			default:
				checkbox_handle(&autosave.cb, &ev);
				button_handle(&btquit, &ev);

				if (btquit.state == BUTTON_STATE_ACTIVATED)
					return;

				break;
			}
		}

		/* Compute button position at runtime. */
		align(ALIGN_BOTTOM_RIGHT, &btquit.x, &btquit.y, btquit.w, btquit.h,
		    frame.x, frame.y, frame.w, frame.h);

		btquit.x -= theme_default()->padding;
		btquit.y -= theme_default()->padding;

		painter_set_color(0xffffffff);
		painter_clear();
		frame_draw(&frame);
		label_draw(&title);
		checkbox_draw(&autosave.cb);
		label_draw(&autosave.label);
		button_draw(&btquit);
		painter_present();

		if ((elapsed = clock_elapsed(&clock)) < 20)
			delay(20 - elapsed);
	}
}

static void
quit(void)
{
	theme_finish();
	window_finish();
	sys_finish();
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

