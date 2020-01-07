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
#include "font.h"
#include "clock.h"
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

	struct texture *logo, *label;
	struct sprite sprite;
	struct clock clock;
	struct font *font;
	struct animation animation;

	window_init("Molko's Adventure", 640, 480);
	window_set_color(0x667788ff);

	clock_start(&clock);

	logo = image_openf("E:\\dev\\molko\\explosion.png");
	font = font_openf("E:\\dev\\molko\\DejaVuSans.ttf", 10);

	if (!logo || !font) {
		printf("%s\n", SDL_GetError());
		exit(1);
	}

	label = font_render(font, "Hello World", 0xffffffff);
	sprite_init(&sprite, logo, 256, 256);
	animation_init(&animation, &sprite, 20);

	setvbuf(stdout, NULL, _IONBF, 0);

	while (!animation_is_complete(&animation)) {
		uint64_t ticks = clock_elapsed(&clock);

		clock_start(&clock);

		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
			case SDL_QUIT:
				return 0;
			}
		}

		animation_update(&animation, ticks);
		window_clear();
		texture_draw(label, 30, 30);
		animation_draw(&animation, 10, 10);
		window_present();
		SDL_Delay(50);
	}

#if 0
	window_set_color(0xffffffff);
	window_draw_line(50, 50, 100, 100);
	window_draw_point(60, 60);
	window_draw_rectangle(true, 20, 20, 70, 10);
	logo = image_openf("E:\\Charactervector.png");
	sprite_init(&sprite, logo, 65, 100);
	sprite_draw(&sprite, 1, 2, 400, 400);
#endif

	return 0;
}
