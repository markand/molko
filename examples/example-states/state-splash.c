/*
 * state-splash.c -- minimal splash screen
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
#include <mlk/core/font.h>
#include <mlk/core/game.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/state.h>
#include <mlk/core/texture.h>
#include <mlk/core/util.h>
#include <mlk/core/window.h>

#include <mlk/ui/align.h>

#include <assets/fonts/zenda.h>

#include "state-menu.h"
#include "state-splash.h"

#define TITLE        "malikania"
#define SIZE         80
#define BG           0xf4f4f4ff
#define FG           0x29366fff
#define DELAY        1000

#define SPLASH(self) MLK_CONTAINER_OF(self, struct splash, state)

struct splash {
	unsigned int elapsed;
	struct mlk_texture texture;
	int x;
	int y;
	struct mlk_state state;
};

static void
state_splash__start(struct mlk_state *self)
{
	struct splash *splash = SPLASH(self);
	struct mlk_font font;

	if (mlk_font_openmem(&font, assets_fonts_zenda, sizeof (assets_fonts_zenda), SIZE) < 0)
		mlk_panic();

	font.style = MLK_FONT_STYLE_ANTIALIASED;

	if (mlk_font_render(&font, &splash->texture, TITLE, FG) < 0)
		mlk_panic();

	mlk_align(MLK_ALIGN_CENTER, 
	          &splash->x,
	          &splash->y,
	          splash->texture.w,
	          splash->texture.h,
	          0,
	          0,
	          mlk_window.w,
	          mlk_window.h
	);

	mlk_font_finish(&font);
}

static void
state_splash__update(struct mlk_state *self, unsigned int ticks)
{
	struct splash *splash = SPLASH(self);

	splash->elapsed += ticks;

	if (splash->elapsed >= DELAY)
		mlk_game_push(state_menu_new());
}

static void
state_splash__draw(struct mlk_state *self)
{
	struct splash *splash = SPLASH(self);

	mlk_painter_set_color(BG);
	mlk_painter_clear();
	mlk_texture_draw(&splash->texture, splash->x, splash->y);
	mlk_painter_present();
}

static void
state_splash__finish(struct mlk_state *self)
{
	struct splash *splash = SPLASH(self);

	mlk_texture_finish(&splash->texture);
}

struct mlk_state *
state_splash_new(void)
{
	struct splash *splash;

	splash = mlk_alloc_new0(1, sizeof (*splash));
	splash->state.name = "splash";
	splash->state.start = state_splash__start;
	splash->state.update = state_splash__update;
	splash->state.draw = state_splash__draw;
	splash->state.finish = state_splash__finish;

	return &splash->state;
}
