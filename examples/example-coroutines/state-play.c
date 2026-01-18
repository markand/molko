/*
 * state-play.c -- very funny game
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

#include <mlk/core/alloc.h>
#include <mlk/core/event.h>
#include <mlk/core/game.h>
#include <mlk/core/image.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/texture.h>
#include <mlk/core/util.h>

#include <mlk/ui/align.h>
#include <mlk/ui/button.h>

#include <assets/images/dvd.h>

#include "app.h"
#include "state-menu.h"
#include "state-play.h"

#define BG         0x99c2dbff
#define BUTTON_H   (mlk_window.h / 20)
#define BUTTON_W   (mlk_window.w / 10)
#define LOGO       assets_images_dvd
#define LOGO_SPEED 100

struct play {
	struct mlk_button button_leave;
	struct mlk_texture logo;
	int x;
	int y;
	int dx;
	int dy;
} play;

static void
state_play_start(void)
{
	if (mlk_image_openmem(&play.logo, LOGO, sizeof (LOGO)) < 0)
		mlk_panic();

	mlk_align(MLK_ALIGN_CENTER,
	          &play.x,
	          &play.y,
	          play.logo.w,
	          play.logo.h,
	          0,
	          0,
	          mlk_window.w,
	          mlk_window.h
	);

	play.button_leave.text = "menu";
	play.button_leave.w = BUTTON_W;
	play.button_leave.h = BUTTON_H;

	mlk_align(MLK_ALIGN_BOTTOM,
	          &play.button_leave.x,
	          &play.button_leave.y,
	          play.button_leave.w,
	          play.button_leave.h,
	          0,
	          0,
	          mlk_window.w,
	          mlk_window.h - 20
	);

	play.dx = play.dy = 1;
	state_play.start = NULL;
}

static void
state_play_handle(const union mlk_event *event)
{
	if (mlk_button_handle(&play.button_leave, event))
		app_switch(&state_menu);

	switch (event->type) {
	case MLK_EVENT_QUIT:
		mlk_game_quit();
		break;
	case MLK_EVENT_KEYUP:
		switch (event->key.key) {
		case MLK_KEY_ESCAPE:
			app_switch(&state_menu);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

static void
state_play_update(unsigned int ticks)
{
	play.x += play.dx * (LOGO_SPEED * ticks / 1000);
	play.y += play.dy * (LOGO_SPEED * ticks / 1000);

	if (play.x < 0) {
		play.dx = 1;
		play.x = 0;
	} else if (play.x + play.logo.w >= mlk_window.w) {
		play.dx = -1;
		play.x = mlk_window.w - play.logo.w;
	}

	if (play.y < 0) {
		play.dy = 1;
		play.y = 0;
	} else if (play.y + play.logo.h >= mlk_window.h) {
		play.dy = -1;
		play.y = mlk_window.h - play.logo.h;
	}

	mlk_button_update(&play.button_leave, ticks);
}

static void
state_play_draw(void)
{
	mlk_painter_set_color(BG);
	mlk_painter_clear();
	mlk_texture_draw(&play.logo, play.x, play.y);
	mlk_button_draw(&play.button_leave);
	mlk_painter_present();
}

struct mlk_game_ops state_play = {
	.start  = state_play_start,
	.handle = state_play_handle,
	.update = state_play_update,
	.draw   = state_play_draw
};
