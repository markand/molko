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

#include "animation.h"
#include "clock.h"
#include "event.h"
#include "font.h"
#include "image.h"
#include "sprite.h"
#include "texture.h"
#include "window.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

int
main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);

	window_init("Molko's Adventure", 640, 480);
	window_set_color(0x667788ff);

	for (;;) {
		union event ev;

		while (event_poll(&ev)) {
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

		window_clear();
		window_present();
		SDL_Delay(50);
	}

	return 0;
}
