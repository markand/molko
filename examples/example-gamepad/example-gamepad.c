/*
 * example-label.c -- show how to use label and alignments
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

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <mlk/util/util.h>

#include <mlk/core/core.h>
#include <mlk/core/err.h>
#include <mlk/core/event.h>
#include <mlk/core/game.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/sys.h>
#include <mlk/core/trace.h>
#include <mlk/core/util.h>
#include <mlk/core/window.h>

#include <mlk/ui/align.h>
#include <mlk/ui/label.h>
#include <mlk/ui/ui.h>

#include <mlk/example/example.h>
#include <mlk/example/trace-hud.h>

#define GAMEPAD_NAME_MAX        64
#define GAMEPAD_MAX             16

struct gamepad {
	struct mlk_label label;
	char name[GAMEPAD_NAME_MAX];
	int index;
};

/* descriptors. */
static struct {
	struct gamepad list[GAMEPAD_MAX];
	int len;
	int cur;
} info = {
	.cur = -1
};

static struct mlk_label label = {
	.text = "Keys: <Space> to scan devices, <Up>/<Down> to select one"
};
static struct mlk_label_style label_selected = {
	.color = 0x518822ff,
};

/* native instance. */
static struct mlk_gamepad gamepad;

static void
use(int n)
{
	struct gamepad *g = &info.list[n];

	if (mlk_gamepad_open(&gamepad, g->index) < 0) {
		mlk_tracef("unable to open %s: %s", g->name, mlk_err());
	} else {
		mlk_tracef("selected %s", g->name);
		info.cur = n;
	}
}

static void
scan(void)
{
	struct mlk_gamepad_iter iter;
	struct gamepad *pad;
	unsigned int h;

	info.len = 0;
	info.cur = -1;

	if (mlk_gamepad_iter_begin(&iter) < 0)
		mlk_tracef("unable to iterate gamepads: %s", mlk_err());
	else {
		for (int i = 1; mlk_gamepad_iter_next(&iter); ++i) {
			/* Add information. */
			pad = &info.list[info.len++];
			pad->index = iter.index;
			snprintf(pad->name, sizeof (pad->name), "%d: %s", iter.index, iter.name);

			/* Create a label. */
			pad->label.text = pad->name;
			mlk_label_query(&pad->label, NULL, &h);
			pad->label.x = 10;
			pad->label.y = mlk_window.h - (10 * i) - (h * i);
		}
	}

	/* Select first one for convenience if not already opened. */
	if (info.cur == -1 && info.len)
		use(0);
}

static void
choose(int delta)
{
	int next = -1;

	if (info.cur == -1) {
		/* No gamepad selected yet. */
		if (info.len)
			next = 0;
	} else {
		if (delta < 0 && info.cur > 0)
			next = info.cur - 1;
		else if (delta > 0 && info.cur + 1 < info.len)
			next = info.cur + 1;
	}

	if (next != -1)
		use(next);
}

static void
init(void)
{
	unsigned int w, h;

	if (mlk_example_init("example-gamepad") < 0)
		mlk_panic();

	mlk_trace_handler = mlk_trace_hud_handler;

	/* Initial scan for systems that support that. */
	scan();

	/* Put help text into the center. */
	mlk_label_query(&label, &w, &h);
	mlk_align(MLK_ALIGN_CENTER, &label.x, &label.y, w, h,
	    0, 0, mlk_window.w, mlk_window.h);
}

static void
handle_button(const struct mlk_event_button *pad)
{
	const char *which = pad->type == MLK_EVENT_BUTTONDOWN ? "pressed" : "released";

	mlk_tracef("gamepad button %d %s", pad->button, which);
}

static void
handle_device(const struct mlk_event_gamepad *dev)
{
	puts("OK");
	if (dev->type == MLK_EVENT_GAMEPAD_DETACH) {
		if (info.cur == dev->index) {
			mlk_gamepad_finish(&gamepad);
			info.cur = -1;
		}

		mlk_tracef("device %d detached", dev->index);
	} else
		mlk_tracef("device %d attached", dev->index);

	scan();
}

static void
handle(const union mlk_event *ev)
{
	switch (ev->type) {
	case MLK_EVENT_QUIT:
		mlk_game_quit();
		break;
	case MLK_EVENT_KEYDOWN:
		switch (ev->key.key) {
		case MLK_KEY_SPACE:
			scan();
			break;
		case MLK_KEY_UP:
			choose(-1);
			break;
		case MLK_KEY_DOWN:
			choose(1);
			break;
		default:
			break;
		}
		break;
	case MLK_EVENT_BUTTONDOWN:
	case MLK_EVENT_BUTTONUP:
		handle_button(&ev->button);
		break;
	case MLK_EVENT_GAMEPAD_ATTACH:
	case MLK_EVENT_GAMEPAD_DETACH:
		handle_device(&ev->gamepad);
		break;
	default:
		break;
	}
}

static void
update(unsigned int ticks)
{
	mlk_trace_hud_update(ticks);
}

static void
draw(void)
{
	struct mlk_label *l;

	mlk_painter_set_color(0xdcd4ffff);
	mlk_painter_clear();
	mlk_trace_hud_draw();
	mlk_label_draw(&label);

	for (int i = 0; i < info.len; ++i) {
		l = &info.list[i].label;
		l->style = i == info.cur ? &label_selected : NULL;

		mlk_label_draw(&info.list[i].label);
	}

	mlk_painter_present();
}

static void
run(void)
{
	struct mlk_game_ops ops = {
		.handle = handle,
		.update = update,
		.draw = draw
	};

	mlk_game_init(&ops);
	mlk_game_loop();
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
}
