/*
 * example-gridmenu.c -- show how to use grid menu
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
#include <mlk/ui/frame.h>
#include <mlk/ui/gridmenu.h>
#include <mlk/ui/label.h>
#include <mlk/ui/style.h>
#include <mlk/ui/ui.h>

#include <mlk/example/example.h>
#include <mlk/example/glower.h>
#include <mlk/example/registry.h>

static const char * const items[] = {
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

static struct mlk_gridmenu menu = {
	.w = 300,
	.h = 100,
	.nrows = 3,
	.ncols = 2,
	.items = items,
	.itemsz = MLK_UTIL_SIZE(items)
};
static struct mlk_glower menu_glower = {
	.start = 0x00bfa3ff,
	.end = 0x006b6dff,
	.delay = 20
};

static void
update_color(unsigned int ticks)
{
	mlk_glower_update(&menu_glower, ticks);
	mlk_gridmenu_style->color_selected = menu_glower.color;
}

static void
init(void)
{
	if (mlk_example_init("example-gridmenu") < 0)
		mlk_panic();

	/* Copy style. */
	mlk_glower_init(&menu_glower);
}

static void
handle(const union mlk_event *ev)
{
	switch (ev->type) {
	case MLK_EVENT_QUIT:
		mlk_game_quit();
		break;
	default:
		if (mlk_gridmenu_handle(&menu, ev))
			mlk_tracef("selected index: %zu (%s)", menu.selected, menu.items[menu.selected]);
		break;
	}
}

static void
update(unsigned int ticks)
{
	update_color(ticks);
	mlk_gridmenu_update(&menu, ticks);
}

static void
draw(void)
{
	mlk_painter_set_color(MLK_EXAMPLE_BG);
	mlk_painter_clear();
	mlk_gridmenu_draw(&menu);
	mlk_painter_present();
}

static void
run(void)
{
	struct mlk_game_ops ops = {
		.handle = handle,
		.update = update,
		.draw = draw,
	};

	mlk_gridmenu_start(&menu);
	mlk_align(MLK_ALIGN_CENTER, &menu.x, &menu.y, menu.w, menu.h, 0, 0, mlk_window.w, mlk_window.h);

	mlk_game_init(&ops);
	mlk_game_loop();
}

static void
quit(void)
{
	mlk_example_finish();
}

int
main(int, char **)
{
	init();
	run();
	quit();
}
