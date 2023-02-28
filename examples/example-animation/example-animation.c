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
#include <mlk/core/window.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/sprite.h>
#include <mlk/core/state.h>
#include <mlk/core/texture.h>
#include <mlk/core/util.h>

#include <mlk/ui/label.h>
#include <mlk/ui/ui.h>

#include <mlk/example/example.h>
#include <mlk/example/registry.h>

static struct mlk_label label = {
	.text = "Keys: <Space> start or reset the animation.",
	.x = 10,
	.y = 10,
	.flags = MLK_LABEL_FLAGS_SHADOW
};

static struct mlk_state *states[1];
static struct mlk_animation animation;
static int completed = 1;

static void
init(void)
{
	int err;

	if ((err = mlk_example_init("example-animation")) < 0)
		mlk_panicf("mlk_example_init: %s", mlk_err_string(err));
}

static void
handle(struct mlk_state *st, const union mlk_event *ev)
{
	(void)st;

	switch (ev->type) {
	case MLK_EVENT_KEYDOWN:
		switch (ev->key.key) {
		case MLK_KEY_SPACE:
			mlk_animation_start(&animation);
			completed = mlk_animation_completed(&animation);
			break;
		default:
			break;
		}
		break;
	case MLK_EVENT_QUIT:
		mlk_game_quit();
		break;
	default:
		break;
	}
}

static void
update(struct mlk_state *st, unsigned int ticks)
{
	(void)st;

	if (!completed)
		completed = mlk_animation_update(&animation, ticks);
}

static void
draw(struct mlk_state *st)
{
	(void)st;

	unsigned int cellw, cellh;

	cellw = registry_sprites[REGISTRY_TEXTURE_NUMBERS].cellw;
	cellh = registry_sprites[REGISTRY_TEXTURE_NUMBERS].cellh;

	mlk_painter_set_color(0x4f8fbaff);
	mlk_painter_clear();
	mlk_label_draw(&label);

	if (!completed)
		mlk_animation_draw(&animation, (mlk_window.w - cellw) / 2, (mlk_window.h - cellh) / 2);

	mlk_painter_present();
}

static void
run(void)
{
	struct mlk_state state = {
		.handle = handle,
		.update = update,
		.draw = draw
	};

	mlk_animation_init(&animation, &registry_sprites[REGISTRY_TEXTURE_NUMBERS], 1000);

	mlk_game_init(states, MLK_UTIL_SIZE(states));
	mlk_game_push(&state);
	mlk_game_loop();
}

static void
quit(void)
{
	mlk_example_finish();
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
