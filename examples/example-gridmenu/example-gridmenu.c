/*
 * example-gridmenu.c -- show how to use grid menu
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/state.h>
#include <mlk/core/sys.h>
#include <mlk/core/trace.h>
#include <mlk/core/util.h>
#include <mlk/core/window.h>

#include <ui/align.h>
#include <ui/gridmenu.h>
#include <ui/label.h>
#include <ui/theme.h>
#include <ui/ui.h>

#define W       (1280)
#define H       (720)

static struct state *states[1];

static void
init(void)
{
	if (core_init("fr.malikania", "example-gridmenu") < 0 || ui_init() < 0)
		panic();
	if (window_open("Example - Grid menu", W, H) < 0)
		panic();
}

static void
quit(void)
{
	window_finish();
	ui_finish();
	core_finish();
}

static void
handle(struct state *st, const union event *ev)
{
	struct gridmenu *menu = st->data;

	switch (ev->type) {
	case EVENT_QUIT:
		game_quit();
		break;
	default:
		if (gridmenu_handle(st->data, ev))
			tracef("selected index: %zu (%s)", menu->selected, menu->items[menu->selected]);
		break;
	}
}

static void
draw(struct state *st)
{
	painter_set_color(0x4f8fbaff);
	painter_clear();
	gridmenu_draw(st->data);
	painter_present();
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

	struct gridmenu menu = {0};
	struct state state = {
		.data = &menu,
		.handle = handle,
		.draw = draw,
	};

	gridmenu_init(&menu, 3, 2, items, UTIL_SIZE(items));
	gridmenu_resize(&menu, 0, 0, 300, 100);

	align(ALIGN_CENTER, &menu.x, &menu.y, menu.w, menu.h, 0, 0, W, H);

	game_init(states, UTIL_SIZE(states));
	game_push(&state);
	game_loop();
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
