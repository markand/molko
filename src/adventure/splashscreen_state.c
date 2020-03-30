/*
 * splashscreen_state.c -- splash screen state
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

#include <font.h>
#include <game.h>
#include <image.h>
#include <map.h>
#include <map_state.h>
#include <painter.h>
#include <panic.h>
#include <state.h>
#include <sys.h>
#include <texture.h>
#include <window.h>

#include "splashscreen_state.h"
#include "mainmenu_state.h"

#define DELAY 2000

struct splashscreen_state_data splashscreen_state_data;

static void
enter(void)
{
	struct font font = {
		.color = 0x000000ff
	};

	if (!(font_open(&font, sys_datapath("fonts/teutonic1.ttf"), 130)))
		panic();
	if (!(font_render(&font, &splashscreen_state_data.text, "Molko's Adventure")))
		panic();

	/* Compute position. */
	const unsigned int w = splashscreen_state_data.text.w;
	const unsigned int h = splashscreen_state_data.text.h;

	splashscreen_state_data.x = (window.w / 2) - (w / 2);
	splashscreen_state_data.y = (window.h / 2) - (h / 2);

	font_finish(&font);
}

static void
leave(void)
{
	/* We don't delete the texture because it is used by mainmenu_state. */
}

static void
handle(const union event *event)
{
	(void)event;
}

static void
update(unsigned int ticks)
{
	splashscreen_state_data.elapsed += ticks;

	if (splashscreen_state_data.elapsed >= DELAY)
		game_switch(&mainmenu_state, false);
}

static void
draw(void)
{
	painter_set_color(0xffffffff);
	painter_clear();
	texture_draw(&splashscreen_state_data.text,
		splashscreen_state_data.x,
		splashscreen_state_data.y);
}

struct state splashscreen_state = {
	.enter = enter,
	.leave = leave,
	.handle = handle,
	.update = update,
	.draw = draw
};
