/*
 * example-label.c -- show how to use label and alignments
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
#include <core/painter.h>
#include <core/panic.h>
#include <core/sys.h>
#include <core/util.h>
#include <core/window.h>

#include <ui/label.h>
#include <ui/theme.h>

#define W       (1280)
#define H       (720)

static void
init(void)
{
	if (!sys_init() ||
	    !window_init("Example - Label", W, H) ||
	    !theme_init())
		panic();
}

static void
quit(void)
{
	theme_finish();
	window_finish();
	sys_finish();
}

static void
run(void)
{
	struct clock clock = {0};
	struct label labels[] = {
		{
			.x = 0,
			.y = 0,
			.w = W,
			.h = H,
			.text = "Top left",
			.align = LABEL_ALIGN_TOP_LEFT
		},
		{
			.x = 0,
			.y = 0,
			.w = W,
			.h = H,
			.text = "Top",
			.align = LABEL_ALIGN_TOP
		},
		{
			.x = 0,
			.y = 0,
			.w = W,
			.h = H,
			.text = "Top right",
			.align = LABEL_ALIGN_TOP_RIGHT
		},
		{
			.x = 0,
			.y = 0,
			.w = W,
			.h = H,
			.text = "Right",
			.align = LABEL_ALIGN_RIGHT
		},
		{
			.x = 0,
			.y = 0,
			.w = W,
			.h = H,
			.text = "Bottom right",
			.align = LABEL_ALIGN_BOTTOM_RIGHT
		},
		{
			.x = 0,
			.y = 0,
			.w = W,
			.h = H,
			.text = "Bottom",
			.align = LABEL_ALIGN_BOTTOM
		},
		{
			.x = 0,
			.y = 0,
			.w = W,
			.h = H,
			.text = "Bottom left",
			.align = LABEL_ALIGN_BOTTOM_LEFT
		},
		{
			.x = 0,
			.y = 0,
			.w = W,
			.h = H,
			.text = "Left",
			.align = LABEL_ALIGN_LEFT
		},
		{
			.x = 0,
			.y = 0,
			.w = W,
			.h = H,
			.text = "The world is Malikania.",
			.flags = LABEL_FLAGS_SHADOW,
			.align = LABEL_ALIGN_CENTER
		},
	};
	struct label mlabel = {
		.text = "This one follows your mouse and is not aligned."
	};

	clock_start(&clock);

	for (;;) {
		union event ev;
		unsigned int elapsed = clock_elapsed(&clock);

		clock_start(&clock);

		while (event_poll(&ev)) {
			switch (ev.type) {
			case EVENT_MOUSE:
				mlabel.x = ev.mouse.x;
				mlabel.y = ev.mouse.y;
				break;
			case EVENT_QUIT:
				return;
			default:
				break;
			}
		}

		painter_set_color(0x4f8fbaff);
		painter_clear();

		for (size_t i = 0; i < NELEM(labels); ++i)
			label_draw(&labels[i]);

		label_draw(&mlabel);
		painter_present();

		if ((elapsed = clock_elapsed(&clock)) < 20)
			delay(20 - elapsed);
	}
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

