/*
 * state-play.c -- very funny game
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

#include <mlk/core/alloc.h>
#include <mlk/core/event.h>
#include <mlk/core/game.h>
#include <mlk/core/image.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/state.h>
#include <mlk/core/texture.h>
#include <mlk/core/util.h>

#include <mlk/ui/align.h>
#include <mlk/ui/button.h>

#include <assets/images/dvd.h>

#include "state-menu.h"
#include "state-play.h"

#define BG         0x99c2dbff
#define BUTTON_H   (mlk_window.h / 20)
#define BUTTON_W   (mlk_window.w / 10)
#define LOGO       assets_images_dvd
#define LOGO_SPEED 100

#define PLAY(self) MLK_UTIL_CONTAINER_OF(self, struct play, state)

struct play {
	struct mlk_button button_leave;
	struct mlk_texture logo;
	int x;
	int y;
	int dx;
	int dy;
	struct mlk_state state;
};

static void
state_play__start(struct mlk_state *self)
{
	struct play *play = PLAY(self);

	if (mlk_image_openmem(&play->logo, LOGO, sizeof (LOGO)) < 0)
		mlk_panic();

	mlk_align(MLK_ALIGN_CENTER,
	          &play->x,
	          &play->y,
	          play->logo.w,
	          play->logo.h,
	          0,
	          0,
	          mlk_window.w,
	          mlk_window.h
	);

	play->button_leave.text = "menu";
	play->button_leave.w = BUTTON_W;
	play->button_leave.h = BUTTON_H;

	mlk_align(MLK_ALIGN_BOTTOM,
	          &play->button_leave.x,
	          &play->button_leave.y,
	          play->button_leave.w,
	          play->button_leave.h,
	          0,
	          0,
	          mlk_window.w,
	          mlk_window.h - 20
	);

	play->dx = play->dy = 1;
}

static void
state_play__handle(struct mlk_state *self, const union mlk_event *event)
{
	struct play *play = PLAY(self);

	if (mlk_button_handle(&play->button_leave, event))
		mlk_game_pop();

	switch (event->type) {
	case MLK_EVENT_QUIT:
		mlk_game_quit();
		break;
	case MLK_EVENT_KEYUP:
		switch (event->key.key) {
		case MLK_KEY_ESCAPE:
			mlk_game_pop();
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
state_play__update(struct mlk_state *self, unsigned int ticks)
{
	struct play *play = PLAY(self);

	play->x += play->dx * (LOGO_SPEED * ticks / 1000);
	play->y += play->dy * (LOGO_SPEED * ticks / 1000);

	if (play->x < 0) {
		play->dx = 1;
		play->x = 0;
	} else if (play->x + play->logo.w >= mlk_window.w) {
		play->dx = -1;
		play->x = mlk_window.w - play->logo.w;
	}

	if (play->y < 0) {
		play->dy = 1;
		play->y = 0;
	} else if (play->y + play->logo.h >= mlk_window.h) {
		play->dy = -1;
		play->y = mlk_window.h - play->logo.h;
	}

	mlk_button_update(&play->button_leave, ticks);
}

static void
state_play__draw(struct mlk_state *self)
{
	struct play *play = PLAY(self);

	mlk_painter_set_color(BG);
	mlk_painter_clear();
	mlk_texture_draw(&play->logo, play->x, play->y);
	mlk_button_draw(&play->button_leave);
	mlk_painter_present();
}

static void
state_play__finish(struct mlk_state *self)
{
	struct play *play = PLAY(self);

	mlk_texture_finish(&play->logo);
}

struct mlk_state *
state_play_new(void)
{
	struct play *play;

	play = mlk_alloc_new0(1, sizeof (*play));
	play->state.name = "play";
	play->state.start = state_play__start;
	play->state.handle = state_play__handle;
	play->state.update = state_play__update;
	play->state.draw = state_play__draw;
	play->state.finish = state_play__finish;

	return &play->state;
}
