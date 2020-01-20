/*
 * main.c -- Molko's Adventure
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

#include "clock.h"
#include "error.h"
#include "event.h"
#include "game.h"
#include "image.h"
#include "map.h"
#include "map_state.h"
#include "splashscreen.h"
#include "sprite.h"
#include "sys.h"
#include "util.h"
#include "window.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

static void
init(void)
{
	struct texture *image;

	if (!sys_init())
		error_fatal();
	if (!window_init("Molko's Adventure", WINDOW_WIDTH, WINDOW_HEIGHT))
		error_fatal();

	image = image_openf(sys_datapath("sprites/test-walk.png"));

	if (!image)
		error_fatal();
	if (!map_open(&map_state_data.map.map, sys_datapath("maps/test.map")))
		error_fatal();

	sprite_init(&map_state_data.player.sprite, image, 48, 48);
	game_switch(&map_state);

	/* Default state is splash screen */
	//game_switch(&splashscreen_state);
}

static void
run(void)
{
	union event ev;
	struct clock clock;

	clock_start(&clock);

	for (;;) {
		unsigned int elapsed = clock_elapsed(&clock);

		clock_start(&clock);

		while (event_poll(&ev)) {
			/* TODO: this must be handled by states. */
			if (ev.type == EVENT_QUIT)
				return;

			game_handle(&ev);
		}

		game_update(elapsed);
		game_draw();

		if ((elapsed = clock_elapsed(&clock)) < 20)
			delay(20 - elapsed);
	}
}

static void
close(void)
{
	window_close();
	sys_close();
}

int
main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	init();
	run();
	close();

	return 0;
}
