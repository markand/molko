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

#include <stdlib.h>

#include <core/animation.h>
#include <core/clock.h>
#include <core/event.h>
#include <core/drawable.h>
#include <core/key.h>
#include <core/label.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/sys.h>
#include <core/image.h>
#include <core/sprite.h>
#include <core/texture.h>
#include <core/theme.h>
#include <core/util.h>
#include <core/window.h>

#include <assets/sprites/explosion.h>

#define W 1280
#define H 720

static struct label help = {
	.text = "Keys: <Esc> to reset. Click anywhere to spawn a drawable.",
	.x = 10,
	.y = 10,
	.w = W,
	.h = H,
	.align = LABEL_ALIGN_TOP_LEFT,
	.flags = LABEL_FLAGS_SHADOW
};

#if 0
// TODO: for the moment only animations are supported.
static unsigned int which_selection;
static char which_text[128];
static struct label which = {
	.text = buf,
	.x = 10,
	.y = 40,
	.w = W,
	.h = 32,
	.flags = LABEL_NO_HCENTER,
	.color = 0x4f8fbaff
};
#endif

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
	if (!sys_init() ||
	    !window_init("Example - Drawable", W, H) ||
	    !theme_init())
		panic();

	/* 0: Explosion animation. */
	if (!image_openmem(&explosion_tex, sprites_explosion, sizeof (sprites_explosion)))
		panic();

	sprite_init(&explosion_sprite, &explosion_tex, 256, 256);
}

static void
explosion_finish(struct drawable *dw)
{
	free(dw->data);
}

static void
spawn(int x, int y)
{
	struct explosion *expl = emalloc(sizeof (struct explosion));

	animation_init(&expl->anim, &explosion_sprite, 100);
	animation_drawable(&expl->anim, &expl->dw, x, y);

	/*
	 * This work because the drawable->data field expects a struct animation
	 * pointer which is the first member of struct explosion.
	 *
	 * Thus this "poor man inheritance" trick works perfectly in our case
	 * and we simply need to free the whole explosion struct afterwards.
	 */
	expl->dw.finish = explosion_finish;

	drawable_stack_add(&stack, &expl->dw);
}

static void
run(void)
{
	struct clock clock = {0};

	clock_start(&clock);

	for (;;) {
		union event ev;
		unsigned int elapsed = clock_elapsed(&clock);

		clock_start(&clock);

		while (event_poll(&ev)) {
			switch (ev.type) {
			case EVENT_KEYDOWN:
				switch (ev.key.key) {
				case KEY_ESCAPE:
					drawable_stack_finish(&stack);
					break;
				default:
					break;
				}
				break;
			case EVENT_CLICKDOWN:
				spawn(ev.click.x, ev.click.y);
				break;
			case EVENT_QUIT:
				return;
			default:
				break;
			}
		}

		drawable_stack_update(&stack, elapsed);
		painter_set_color(0xebede9ff);
		painter_clear();
		label_draw(&help);
		drawable_stack_draw(&stack);
		painter_present();

		if ((elapsed = clock_elapsed(&clock)) < 20)
			delay(20 - elapsed);
	}
}

static void
quit(void)
{
	
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
