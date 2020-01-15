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

#include <stdio.h>
#include <stdlib.h>

#include "clock.h"
#include "error.h"
#include "event.h"
#include "painter.h"
#include "image.h"
#include "map.h"
#include "sys.h"
#include "walksprite.h"
#include "window.h"
#include "util.h"

int
main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

#if 0

	struct clock clock;
	struct map map;
	struct texture *guy;
	struct sprite sprite;
	struct walksprite ws;

	if (!sys_init())
		error_fatal();
	if (!window_init("Molko's Adventure", 1024, 576))
		error_fatal();
	if (!map_open(&map, "world.json"))
		error_fatal();
	if (!(guy = image_openf(sys_datapath("sprites/test-walk.png"))))
		error_fatal();

	sprite_init(&sprite, guy, 48, 48);
	walksprite_init(&ws, &sprite, 200);
	clock_start(&clock);
	map_repaint(&map);

	int guy_x = 10;
	int guy_y = 10;
	int guy_dx = 0;
	int guy_dy = 0;

	int dx = 0;
	int dy = 0;
	int orientation = 0;

	for (;;) {
		uint64_t elapsed = clock_elapsed(&clock);
		union event ev;

		clock_start(&clock);

		while (event_poll(&ev)) {
			switch (ev.type) {
			case EVENT_QUIT:
				return 0;
			case EVENT_KEYDOWN:
				switch (ev.key.key) {
				case KEY_UP:
					orientation = 0;
					dy = -5;
					break;
				case KEY_RIGHT:
					orientation = 2;
					dx = 5;
					break;
				case KEY_DOWN:
					orientation = 4;
					dy = 5;
					break;
				case KEY_LEFT:
					orientation = 6;
					dx = -5;
					break;
				default:
					break;
				}
				break;
			case EVENT_KEYUP:
				switch (ev.key.key) {
				case KEY_DOWN:
					dy = 0;
					break;
				case KEY_UP:
					dy = 0;
					break;
				case KEY_LEFT:
					dx = 0;
					break;
				case KEY_RIGHT:
					dx = 0;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}

		walksprite_update(&ws, elapsed);
		painter_clear();
		map_draw(&map);
		walksprite_draw(&ws, orientation, 100, 100);
		painter_present();
		delay(50);
	}
#endif

	return 0;
}
