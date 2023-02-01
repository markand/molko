/*
 * example-animation.c -- example on how to use animations
 *
 * Copyright (c) 2020-2023 David Demelier <markand@malikania.fr>
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

#include <mlk/core/animation.h>
#include <mlk/core/core.h>
#include <mlk/core/event.h>
#include <mlk/core/game.h>
#include <mlk/core/image.h>
#include <mlk/core/sys.h>
#include <mlk/core/window.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/sprite.h>
#include <mlk/core/state.h>
#include <mlk/core/texture.h>
#include <mlk/core/util.h>

#include <mlk/ui/label.h>
#include <mlk/ui/ui.h>

#include <assets/sprites/numbers.h>

#define W       1280
#define H       720

static struct label label = {
	.text = "Keys: <Space> start or reset the animation.",
	.x = 10,
	.y = 10,
	.flags = LABEL_FLAGS_SHADOW
};

static struct state *states[1];
static struct texture numbers;
static struct animation animation;
static struct sprite sprite;
static int completed = 1;

static void
init(void)
{
	if (core_init("fr.malikania", "example-animation") < 0 || ui_init() < 0)
		panic();
	if (window_open("Example - Animation", W, H) < 0)
		panic();
	if (image_openmem(&numbers, assets_numbers, sizeof (assets_numbers)) < 0)
		panic();
}

static void
handle(struct state *st, const union event *ev)
{
	(void)st;

	switch (ev->type) {
	case EVENT_KEYDOWN:
		switch (ev->key.key) {
		case KEY_SPACE:
			animation_start(&animation);
			completed = animation_completed(&animation);
			break;
		default:
			break;
		}
		break;
	case EVENT_QUIT:
		game_quit();
		break;
	default:
		break;
	}
}

static void
update(struct state *st, unsigned int ticks)
{
	(void)st;

	if (!completed)
		completed = animation_update(&animation, ticks);
}

static void
draw(struct state *st)
{
	(void)st;

	painter_set_color(0x4f8fbaff);
	painter_clear();
	label_draw(&label);

	if (!completed)
		animation_draw(&animation, (window.w - sprite.cellw) / 2, (window.h - sprite.cellh) / 2);

	painter_present();
}

static void
run(void)
{
	struct state state = {
		.handle = handle,
		.update = update,
		.draw = draw
	};

	sprite_init(&sprite, &numbers, 48, 48);
	animation_init(&animation, &sprite, 1000);

	game_init(states, UTIL_SIZE(states));
	game_push(&state);
	game_loop();
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

