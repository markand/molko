/*
 * example-animation.c -- example on how to use animations
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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
#include <mlk/core/texture.h>
#include <mlk/core/util.h>

#include <mlk/ui/label.h>
#include <mlk/ui/ui.h>

#include <mlk/example/example.h>
#include <mlk/example/registry.h>

static struct mlk_label label = {
	.text = "Keys: <Space> standard animation, <l> loop animation.",
	.x = 10,
	.y = 10,
};

static struct mlk_animation explosion = {
	.sprite = &mlk_registry_sprites[MLK_REGISTRY_TEXTURE_EXPLOSION],
	.delay  = 25
};
static struct mlk_animation loop = {
	.sprite = &mlk_registry_sprites[MLK_REGISTRY_TEXTURE_CAT_RUNNING],
	.delay  = 1000 / 16,
	.flags  = MLK_ANIMATION_FLAGS_LOOP
};
static struct mlk_animation *animation;

static void
init(void)
{
	if (mlk_example_init("example-animation") < 0)
		mlk_panic();
}

static void
handle(const union mlk_event *ev)
{
	int changed = 0;

	switch (ev->type) {
	case MLK_EVENT_KEYDOWN:
		switch (ev->key.key) {
		case MLK_KEY_SPACE:
			animation = &explosion;
			changed = 1;
			break;
		case MLK_KEY_L:
			animation = &loop;
			changed = 1;
			break;
		default:
			break;
		}

		if (animation && changed)
			mlk_animation_start(animation);
		break;
	case MLK_EVENT_QUIT:
		mlk_game_quit();
		break;
	default:
		break;
	}
}

static void
update(unsigned int ticks)
{
	if (animation && mlk_animation_update(animation, ticks))
		animation = NULL;
}

static void
draw(void)
{
	unsigned int cellw, cellh;

	mlk_painter_set_color(MLK_EXAMPLE_BG);
	mlk_painter_clear();
	mlk_label_draw(&label);

	if (animation) {
		cellw = animation->sprite->cellw;
		cellh = animation->sprite->cellh;
		mlk_animation_draw(animation, (mlk_window.w - cellw) / 2, (mlk_window.h - cellh) / 2);
	}

	mlk_painter_present();
}

static void
run(void)
{
	struct mlk_game_ops ops = {
		.handle = handle,
		.update = update,
		.draw = draw
	};

	mlk_game_init(&ops);
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
