/*
 * molko.c -- main structure for Molko's Adventure
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

#include <assert.h>
#include <stddef.h>
#include <setjmp.h>
#include <string.h>
#include <locale.h>

#include <core/clock.h>
#include <core/core.h>
#include <core/event.h>
#include <core/image.h>
#include <core/panic.h>
#include <core/sys.h>
#include <core/trace.h>
#include <core/translate.h>
#include <core/util.h>
#include <core/window.h>

#include <ui/ui.h>

#include <rpg/rpg.h>

#include <adventure/state/battle.h>
#include <adventure/state/panic.h>
#include <adventure/state/splashscreen.h>
#include <adventure/state/mainmenu.h>
#include <adventure/state/map.h>

#include "assets.h"
#include "molko.h"
#include "trace_hud.h"

#define WINDOW_WIDTH    1280
#define WINDOW_HEIGHT   720


#include "character/neth.h"
#include "item/potion.h"

static jmp_buf panic_buf;

struct molko molko;

static void
crash(void)
{
	longjmp(panic_buf, 1);
}

void
molko_init(void)
{
	setlocale(LC_ALL, "");

	if (!core_init("fr.malikania", "molko") || !ui_init() || !rpg_init())
		panic();

	translate_init("libmlk-adventure");

	if (!window_open("Molko's Adventure", WINDOW_WIDTH, WINDOW_HEIGHT))
		panic();

	/*
	 * From here, we can setup our panic state which requires a window
	 * to be running.
	 */

	/* Init unrecoverable panic state. */
	molko.panic = state_panic_new();
	panic_handler = crash;
	trace_handler = trace_hud_handler;

	/* Init other stuff. */
	assets_init();

	/* Start to splash. */
#if 0
	// TODO: put back this.
	game_switch(state_splashscreen_ne(), true);
#else
	game_switch(state_mainmenu_new(), true);
	molko.team.members[0] = &character_neth;
	molko.team.members[1] = &character_neth;
	inventory_add(&molko.inventory, &item_potion, 100);
	molko_teleport("maps/map-world.map", -1, -1);
#endif
}

void
molko_run(void)
{
	if (setjmp(panic_buf) == 0) {
		/* Initial game run. */
		game_loop();
	} else {
		/* Clear event queue to avoid accidental key presses. */
		for (union event ev; event_poll(&ev); )
			continue;

		game_switch(molko.panic, true);
		game_loop();
	}
}

void
molko_teleport(const char *map, int origin_x, int origin_y)
{
	molko.state = MOLKO_STATE_MAP;

	game_switch(state_map_new(map, origin_x, origin_y), false);
	game.inhibit = INHIBIT_NONE;
}

void
molko_fight(struct battle *bt)
{
	molko.state = MOLKO_STATE_BATTLE;

	game_switch(state_battle_new(bt), false);
}

const char *
molko_path(const char *file)
{
	assert(file);

	return util_pathf("%s/mlk-adventure/%s", sys_dir(SYS_DIR_DATA), file);
}

void
molko_finish(void)
{
	window_finish();
	rpg_finish();
	ui_finish();
	core_finish();
}
