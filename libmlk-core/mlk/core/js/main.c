/*
 * main.c -- main Javascript loader
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

#include <errno.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

#include <core/alloc.h>
#include <core/core.h>
#include <core/error.h>
#include <core/event.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/window.h>

#include <ui/label.h>
#include <ui/ui.h>

#include <rpg/rpg.h>

#include "js.h"

static jmp_buf panic_buf;

static noreturn void
jump(void)
{
	longjmp(panic_buf, 1);
}

static noreturn void
usage(void)
{
	fprintf(stderr, "usage: molko-js main.js\n");
	exit(1);
}

static noreturn void
die(void)
{
	fprintf(stderr, "abort: %s\n", error());
	exit(1);
}

static void
init(void)
{
	if (!core_init() || !ui_init() || !rpg_init())
		panic();

	panic_handler = jump;
}

static void
draw_info(void)
{
	struct label label = {
		.text = "An unexpected error happened, game must stops.",
		.x = 10,
		.y = 10,
		.flags = LABEL_FLAGS_SHADOW
	};

	label_draw(&label);
}

static void
draw_error(void)
{
	struct label label = {
		.text = error(),
		.x = 10,
		.y = 40,
		.flags = LABEL_FLAGS_SHADOW
	};

	label_draw(&label);
}

static void
run_panic(void)
{
	panic_handler = die;

	for (;;) {
		union event ev;

		while (event_poll(&ev)) {
			switch (ev.type) {
			case EVENT_QUIT:
				return;
			default:
				break;
			}
		}

		painter_set_color(0x752438ff);
		painter_clear();
		draw_info();
		draw_error();
		painter_present();
	}
}

static void
run(const char *path)
{
	struct js js;

	if (setjmp(panic_buf) == 0) {
		if (!js_init(&js))
			panic();

		js_core_load(&js);

		if (!js_open(&js, path))
			panic();
	} else {
		/* TODO: change once we have window_ok function. */
		if (window.w && window.h)
			run_panic();
		else
			die();
	}
}

static void
quit(void)
{
	rpg_finish();
	ui_finish();
	core_finish();
}

int
main(int argc, char **argv)
{
	--argc;
	++argv;

	if (argc == 0)
		usage();

	init();
	run(argv[0]);
	quit();
}
