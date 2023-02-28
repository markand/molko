/*
 * example-gridmenu.c -- show how to use grid menu
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
#include <mlk/core/err.h>
#include <mlk/core/event.h>
#include <mlk/core/game.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/state.h>
#include <mlk/core/sys.h>
#include <mlk/core/trace.h>
#include <mlk/core/util.h>
#include <mlk/core/window.h>

#include <mlk/ui/align.h>
#include <mlk/ui/gridmenu.h>
#include <mlk/ui/label.h>
#include <mlk/ui/theme.h>
#include <mlk/ui/ui.h>

#include <mlk/example/example.h>
#include <mlk/example/registry.h>

static struct mlk_state *states[1];

static void
init(void)
{
	int err;

	if ((err = mlk_example_init("example-gridmenu")) < 0)
		mlk_panicf("mlk_example_init: %s", mlk_err_string(err));
}

static void
quit(void)
{
	mlk_window_finish();
	mlk_ui_finish();
	mlk_core_finish();
}

static void
handle(struct mlk_state *st, const union mlk_event *ev)
{
	struct mlk_gridmenu *menu = st->data;

	switch (ev->type) {
	case MLK_EVENT_QUIT:
		mlk_game_quit();
		break;
	default:
		if (mlk_gridmenu_handle(st->data, ev))
			mlk_tracef("selected index: %zu (%s)", menu->selected, menu->items[menu->selected]);
		break;
	}
}

static void
draw(struct mlk_state *st)
{
	mlk_painter_set_color(0x4f8fbaff);
	mlk_painter_clear();
	mlk_gridmenu_draw(st->data);
	mlk_painter_present();
}

static void
run(void)
{
	const char * const items[] = {
	    "Feu mineur",
	    "Feu majeur",
	    "Feu septième",
	    "Glace mineure",
	    "Glace majeure",
	    "Glace septième",
	    "Foudre mineure",
	    "Foudre majeure",
	    "Foudre septième",
	    "Choc mineur",
	    "Choc majeur",
	    "Choc septième",
	    "Portée",
	    "Escapade",
	    "Destruction",
	    "Résurrection",
	    "Double tour"
	};

	struct mlk_gridmenu menu = {0};
	struct mlk_state state = {
		.data = &menu,
		.handle = handle,
		.draw = draw,
	};

	mlk_gridmenu_init(&menu, 3, 2, items, MLK_UTIL_SIZE(items));
	mlk_gridmenu_resize(&menu, 0, 0, 300, 100);

	mlk_align(MLK_ALIGN_CENTER, &menu.x, &menu.y, menu.w, menu.h, 0, 0, mlk_window.w, mlk_window.h);

	mlk_game_init(states, MLK_UTIL_SIZE(states));
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
