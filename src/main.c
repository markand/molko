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

#include "window.h"
#include "image.h"
#include "texture.h"
#include "sprite.h"

#include <SDL.h>

int
main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	struct texture *logo;
	struct sprite sprite;

	window_init("Molko's Adventure", 640, 480);
	window_set_color(0x667788ff);
	window_clear();
	window_set_color(0xffffffff);
	window_draw_line(50, 50, 100, 100);
	window_draw_point(60, 60);
	window_draw_rectangle(true, 20, 20, 70, 10);
	logo = image_openf("E:\\Charactervector.png");
	sprite_init(&sprite, logo, 65, 100);
	sprite_draw(&sprite, 1, 2, 400, 400);

	window_present();
	SDL_Delay(5000);

	return 0;
}
