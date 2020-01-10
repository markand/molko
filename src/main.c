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

#include "animation.h"
#include "clock.h"
#include "event.h"
#include "font.h"
#include "image.h"
#include "message.h"
#include "sprite.h"
#include "sys.h"
#include "texture.h"
#include "window.h"

int
main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	struct font *font;

	sys_init();
	window_init("Molko's Adventure", 1280, 720);

	if (!(font = font_openf("assets/fonts/DejaVuSansCondensed.ttf", 12)))
		exit(1);

	struct message welcome = {
		.text = {
			"Welcome to this adventure Molko.",
			"I'm pretty proud of you.",
			"Now get of my house."
		},
		.font = font
	};
	struct clock clock;

	for (;;) {
		uint64_t elapsed = clock_elapsed(&clock);

		clock_start(&clock);

		for (union event ev; event_poll(&ev); ) {
			switch (ev.type) {
			case EVENT_QUIT:
				return 0;
			case EVENT_MOUSE:
				printf("mouse moved to %d, %d, state: %d\n", ev.mouse.x, ev.mouse.y, ev.mouse.buttons);
				break;
			case EVENT_CLICKDOWN:
				printf("mouse click on %d, %d, which: %d\n", ev.click.x, ev.click.y, ev.click.button);
				break;
			default:
				break;
			}
		}

		window_set_color(0x667788ff);
		window_clear();
		message_update(&welcome, elapsed);
		message_draw(&welcome);
		window_present();
	}

	sys_close();

	return 0;
}
