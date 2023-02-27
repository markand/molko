/*
 * example-label.c -- show how to use label and alignments
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

#include <stddef.h>

#include <mlk/core/core.h>
#include <mlk/core/event.h>
#include <mlk/core/game.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/state.h>
#include <mlk/core/sys.h>
#include <mlk/core/util.h>
#include <mlk/core/window.h>

#include <mlk/ui/align.h>
#include <mlk/ui/label.h>
#include <mlk/ui/theme.h>
#include <mlk/ui/ui.h>

#define W       (1280)
#define H       (720)

struct {
	enum align align;
	struct label label;
} table[] = {
	{
		.align = ALIGN_TOP_LEFT,
		.label = {
			.text = "Top left"
		}
	},
	{
		.align = ALIGN_TOP,
		.label = {
			.text = "Top",
		}
	},
	{
		.align = ALIGN_TOP_RIGHT,
		.label = {
			.text = "Top right",
		}
	},
	{
		.align = ALIGN_RIGHT,
		.label = {
			.text = "Right",
		}
	},
	{
		.align = ALIGN_BOTTOM_RIGHT,
		.label = {
			.text = "Bottom right",
		}
	},
	{
		.align = ALIGN_BOTTOM,
		.label = {
			.text = "Bottom",
		}
	},
	{
		.align = ALIGN_BOTTOM_LEFT,
		.label = {
			.text = "Bottom left",
		}
	},
	{
		.align = ALIGN_LEFT,
		.label = {
			.text = "Left",
		}
	},
	{
		.align = ALIGN_CENTER,
		.label = {
			.text = "The world is Malikania.",
			.flags = LABEL_FLAGS_SHADOW
		}
	}
};

static struct label mlabel = {
	.text = "This one follows your mouse and is not aligned."
};

static struct state *states[1];

static void
init(void)
{
	if (mlk_core_init("fr.malikania", "example-label") < 0 || ui_init() < 0)
		mlk_panic();
	if (window_open("Example - Label", W, H) < 0)
		mlk_panic();

	for (size_t i = 0; i < UTIL_SIZE(table); ++i) {
		struct label *l = &table[i].label;
		unsigned int w, h;

		label_query(l, &w, &h);
		align(table[i].align, &l->x, &l->y, w, h, 0, 0, W, H);
	}
}

static void
quit(void)
{
	window_finish();
	ui_finish();
	mlk_core_finish();
}

static void
handle(struct state *st, const union mlk_event *ev)
{
	(void)st;

	switch (ev->type) {
	case MLK_EVENT_MOUSE:
		mlabel.x = ev->mouse.x;
		mlabel.y = ev->mouse.y;
		break;
	case MLK_EVENT_QUIT:
		mlk_game_quit();
		break;
	default:
		break;
	}
}

static void
draw(struct state *st)
{
	(void)st;

	mlk_painter_set_color(0x4f8fbaff);
	mlk_painter_clear();

	for (size_t i = 0; i < UTIL_SIZE(table); ++i)
		label_draw(&table[i].label);

	label_draw(&mlabel);
	mlk_painter_present();
}

static void
run(void)
{
	struct state state = {
		.handle = handle,
		.draw = draw
	};

	mlk_game_init(states, UTIL_SIZE(states));
	mlk_game_push(&state);
	mlk_game_loop();
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
