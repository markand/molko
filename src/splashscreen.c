/*
 * splashscreen.c -- splash screen state
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

#include "error.h"
#include "font.h"
#include "painter.h"
#include "splashscreen.h"
#include "map_state.h"
#include "image.h"
#include "state.h"
#include "sys.h"
#include "texture.h"
#include "window.h"
#include "map.h"

#define DELAY 3000

static unsigned int elapsed;
static struct font *font;
static struct texture *text;
static int x;
static int y;

static void
enter(void)
{
	if (!(font = font_openf(sys_datapath("fonts/knights-quest.ttf"), 160)))
		error_fatal();
	if (!(text = font_render(font, "Molko's Adventure", 0x000000ff)))
		error_fatal();

	/* Compute position. */
	const unsigned int w = texture_width(text);
	const unsigned int h = texture_height(text);

	x = (window_width() / 2) - (w / 2);
	y = (window_height() / 2) - (h / 2) - 100;
}

static void
leave(void)
{
	font_close(font);
}

static void
handle(const union event *event)
{
	(void)event;
}

static void
update(unsigned int ticks)
{
	elapsed += ticks;

	/* TODO: change this once map is done. */
	if (elapsed >= DELAY) {
		/* TODO: this will be removed too. */
		static struct map map;
		static struct texture *player_texture;
		static struct sprite player_sprite;

		if (!map_open(&map, sys_datapath("maps/test.map")))
			error_fatal();
		if (!(player_texture = image_openf(sys_datapath("sprites/test-walk.png"))))
			error_fatal();

		sprite_init(&player_sprite, player_texture, 48, 48);
		//map_state_start(&map, &player_sprite);
	}
}

static void
draw(void)
{
	painter_set_color(0xffffffff);
	painter_clear();
	texture_draw(text, x, y);
	painter_present();
}

struct state splashscreen_state = {
	.enter = enter,
	.leave = leave,
	.handle = handle,
	.update = update,
	.draw = draw
};
