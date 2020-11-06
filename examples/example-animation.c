/*
 * example-animation.c -- example on how to use animations
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

#include <core/animation.h>
#include <core/clock.h>
#include <core/core.h>
#include <core/event.h>
#include <core/image.h>
#include <core/sys.h>
#include <core/window.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/sprite.h>
#include <core/texture.h>
#include <core/util.h>

#include <ui/label.h>
#include <ui/ui.h>

#include <assets/sprites/numbers.h>

#define W 1280
#define H 720

static struct texture numbers;

static struct label label = {
	.text = "Keys: <Space> start or reset the animation.",
	.x = 10,
	.y = 10,
	.flags = LABEL_FLAGS_SHADOW
};

static void
init(void)
{
	if (!core_init() || !ui_init())
		panic();
	if (!window_open("Example - Animation", W, H))
		panic();
	if (!image_openmem(&numbers, sprites_numbers, sizeof (sprites_numbers)))
		panic();
}

static void
run(void)
{
	struct clock clock = {0};
	struct animation animation;
	struct sprite sprite;
	bool completed = true;

	clock_start(&clock);
	sprite_init(&sprite, &numbers, 48, 48);
	animation_init(&animation, &sprite, 1000);

	for (;;) {
		union event ev;
		unsigned int elapsed = clock_elapsed(&clock);

		clock_start(&clock);

		while (event_poll(&ev)) {
			switch (ev.type) {
			case EVENT_KEYDOWN:
				switch (ev.key.key) {
				case KEY_SPACE:
					animation_start(&animation);
					completed = animation_completed(&animation);
					break;
				default:
					break;
				}
				break;
			case EVENT_QUIT:
				return;
			default:
				break;
			}
		}

		if (!completed)
			completed = animation_update(&animation, elapsed);

		painter_set_color(0x4f8fbaff);
		painter_clear();
		label_draw(&label);

		if (!completed)
			animation_draw(&animation, (window.w - sprite.cellw) / 2, (window.h - sprite.cellh) / 2);

		painter_present();

		if ((elapsed = clock_elapsed(&clock)) < 20)
			delay(20 - elapsed);
	}
}

static void
quit(void)
{
	window_finish();
	ui_finish();
	core_finish();
}

int
main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	init();
	run();
	quit();
}

