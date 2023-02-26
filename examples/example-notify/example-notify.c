/*
 * example-notify -- show how notifications work
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

#include <assert.h>

#include <mlk/core/core.h>
#include <mlk/core/event.h>
#include <mlk/core/game.h>
#include <mlk/core/image.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/state.h>
#include <mlk/core/texture.h>
#include <mlk/core/util.h>
#include <mlk/core/window.h>

#include <mlk/ui/notify.h>
#include <mlk/ui/label.h>
#include <mlk/ui/ui.h>

/* Sword by Icongeek26 (https://www.flaticon.com). */
#include <assets/images/sword.h>

#define W       1280
#define H       720

static struct label help = {
	.text = "Keys: <Space> to generate a notification.",
	.x = 10,
	.y = 10,
	.flags = LABEL_FLAGS_SHADOW
};
static struct texture icon;
static struct state *states[1];

static void
init(void)
{
	if (core_init("fr.malikania", "example-notify") < 0 || ui_init() < 0)
		panic();
	if (window_open("Example - Notify", W, H) < 0)
		panic();
	if (mlk_image_openmem(&icon, assets_images_sword, sizeof (assets_images_sword)) < 0)
		panic();
}

static void
handle(struct state *st, const union mlk_event *ev)
{
	(void)st;

	switch (ev->type) {
	case MLK_EVENT_QUIT:
		mlk_game_quit();
		break;
	case MLK_EVENT_KEYDOWN:
		if (ev->key.key == MLK_KEY_SPACE)
			notify(&icon, "Quest", "Quest finished.");
		break;
	default:
		break;
	}
}

static void
update(struct state *st, unsigned int ticks)
{
	(void)st;

	notify_update(ticks);
}

static void
draw(struct state *st)
{
	(void)st;

	mlk_painter_set_color(0xffffffff);
	mlk_painter_clear();
	label_draw(&help);
	notify_draw();
	mlk_painter_present();
}

static void
run(void)
{
	struct state state = {
		.handle = handle,
		.update = update,
		.draw = draw
	};

	mlk_game_init(states, UTIL_SIZE(states));
	mlk_game_push(&state);
	mlk_game_loop();
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

	return 0;
}
