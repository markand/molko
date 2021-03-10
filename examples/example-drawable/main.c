/*
 * example-drawable.c -- example on how to use automatic drawables
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

#include <core/alloc.h>
#include <core/animation.h>
#include <core/core.h>
#include <core/event.h>
#include <core/game.h>
#include <core/drawable.h>
#include <core/key.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/sys.h>
#include <core/image.h>
#include <core/sprite.h>
#include <core/state.h>
#include <core/texture.h>
#include <core/util.h>
#include <core/window.h>

#include <ui/label.h>
#include <ui/theme.h>
#include <ui/ui.h>

#define W       1280
#define H       720
#define PATH(r) util_pathf("%s/mlk-adventure/%s", sys_dir(SYS_DIR_DATA), r)

static struct label help = {
	.x = 10,
	.y = 10,
	.text = "Keys: <Esc> to reset. Click anywhere to spawn a drawable.",
	.flags = LABEL_FLAGS_SHADOW
};

static struct drawable_stack stack;

/*
 * List of drawables for this example.
 * -----------------------------------------------------------------------------
 */

/* 0: Explosion animation. */
static struct texture explosion_tex;
static struct sprite explosion_sprite;

struct explosion {
	struct animation anim;
	struct drawable dw;
};

static void
init(void)
{
	if (core_init("fr.malikania", "drawable") < 0 || ui_init() < 0)
		panic();
	if (window_open("Example - Drawable", W, H) < 0)
		panic();

	/* 0: Explosion animation. */
	if (image_open(&explosion_tex, PATH("sprites/explosion.png")) < 0)
		panic();

	sprite_init(&explosion_sprite, &explosion_tex, 256, 256);
}

static int
explosion_update(struct drawable *dw, unsigned int ticks)
{
	struct explosion *ex = dw->data;

	return animation_update(&ex->anim, ticks);
}

static void
explosion_draw(struct drawable *dw)
{
	struct explosion *ex = dw->data;

	animation_draw(&ex->anim, ex->dw.x, ex->dw.y);
}

static void
explosion_finish(struct drawable *dw)
{
	free(dw->data);
}

static void
spawn(int x, int y)
{
	struct explosion *ex = alloc_new0(sizeof (*ex));

	animation_init(&ex->anim, &explosion_sprite, 15);

	ex->dw.data = ex;
	ex->dw.x = x - (int)(explosion_sprite.cellw / 2);
	ex->dw.y = y - (int)(explosion_sprite.cellh / 2);
	ex->dw.update = explosion_update;
	ex->dw.draw = explosion_draw;
	ex->dw.finish = explosion_finish;

	drawable_stack_add(&stack, &ex->dw);
}

static void
handle(struct state *st, const union event *ev)
{
	(void)st;

	switch (ev->type) {
	case EVENT_KEYDOWN:
		switch (ev->key.key) {
		case KEY_ESCAPE:
			drawable_stack_finish(&stack);
			break;
		default:
			break;
		}
		break;
	case EVENT_CLICKDOWN:
		spawn(ev->click.x, ev->click.y);
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

	drawable_stack_update(&stack, ticks);

}

static void
draw(struct state *st)
{
	(void)st;

	painter_set_color(0xebede9ff);
	painter_clear();
	label_draw(&help);
	drawable_stack_draw(&stack);
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

	game_switch(&state, 1);
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
