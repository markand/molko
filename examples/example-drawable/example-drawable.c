/*
 * example-drawable.c -- example on how to use automatic drawables
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

#include <stdio.h>
#include <stdlib.h>

#include <mlk/core/alloc.h>
#include <mlk/core/animation.h>
#include <mlk/core/core.h>
#include <mlk/core/event.h>
#include <mlk/core/game.h>
#include <mlk/core/drawable.h>
#include <mlk/core/drawable-stack.h>
#include <mlk/core/key.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/sys.h>
#include <mlk/core/image.h>
#include <mlk/core/sprite.h>
#include <mlk/core/state.h>
#include <mlk/core/texture.h>
#include <mlk/core/util.h>
#include <mlk/core/window.h>

#include <mlk/ui/label.h>
#include <mlk/ui/ui.h>

#include <mlk/example/example.h>
#include <mlk/example/registry.h>

static struct mlk_label help = {
	.x = 10,
	.y = 10,
	.text = "Keys: <Esc> to reset. Click anywhere to spawn a drawable."
};

static struct mlk_state *states[1];
static struct mlk_drawable *drawables[64];
static struct mlk_drawable_stack stack;

/*
 * List of drawables for this example.
 * -----------------------------------------------------------------------------
 */

/* 0: Explosion animation. */
static struct mlk_texture *explosion_tex;
static struct mlk_sprite *explosion_sprite;

struct explosion {
	struct mlk_animation anim;
	struct mlk_drawable dw;
};

static void
init(void)
{
	int err;

	if ((err = mlk_example_init("example-drawable")) < 0)
		mlk_panicf("mlk_example_init: %s", mlk_err_string(err));

	explosion_tex = &mlk_registry_textures[MLK_REGISTRY_TEXTURE_EXPLOSION];
	explosion_sprite = &mlk_registry_sprites[MLK_REGISTRY_TEXTURE_EXPLOSION];
}

static int
explosion_update(struct mlk_drawable *dw, unsigned int ticks)
{
	struct explosion *ex = dw->data;

	return mlk_animation_update(&ex->anim, ticks);
}

static void
explosion_draw(struct mlk_drawable *dw)
{
	struct explosion *ex = dw->data;

	mlk_animation_draw(&ex->anim, ex->dw.x, ex->dw.y);
}

static void
explosion_finish(struct mlk_drawable *dw)
{
	mlk_alloc_free(dw->data);
}

static void
spawn(int x, int y)
{
	struct explosion *ex = mlk_alloc_new0(1, sizeof (*ex));

	mlk_animation_init(&ex->anim, explosion_sprite, 15);

	ex->dw.data = ex;
	ex->dw.x = x - (int)(explosion_sprite->cellw / 2);
	ex->dw.y = y - (int)(explosion_sprite->cellh / 2);
	ex->dw.update = explosion_update;
	ex->dw.draw = explosion_draw;
	ex->dw.finish = explosion_finish;

	mlk_drawable_stack_add(&stack, &ex->dw);
}

static void
handle(struct mlk_state *st, const union mlk_event *ev)
{
	(void)st;

	switch (ev->type) {
	case MLK_EVENT_KEYDOWN:
		switch (ev->key.key) {
		case MLK_KEY_ESCAPE:
			mlk_drawable_stack_finish(&stack);
			break;
		default:
			break;
		}
		break;
	case MLK_EVENT_CLICKDOWN:
		spawn(ev->click.x, ev->click.y);
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

	mlk_drawable_stack_update(&stack, ticks);
}

static void
draw(struct mlk_state *st)
{
	(void)st;

	mlk_painter_set_color(MLK_EXAMPLE_BG);
	mlk_painter_clear();
	mlk_label_draw(&help);
	mlk_drawable_stack_draw(&stack);
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

	mlk_drawable_stack_init(&stack, drawables, MLK_UTIL_SIZE(drawables));

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
