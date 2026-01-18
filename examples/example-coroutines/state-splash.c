/*
 * state-splash.c -- minimal splash screen
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
#include <mlk/core/font.h>
#include <mlk/core/game.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/texture.h>
#include <mlk/core/util.h>
#include <mlk/core/window.h>

#include <mlk/ui/align.h>

#include <assets/fonts/zenda.h>

#include "app.h"
#include "state-menu.h"
#include "state-splash.h"

#define TITLE        "malikania"
#define SIZE         80
#define BG           0xf4f4f4ff
#define FG           0x29366fff
#define DELAY        1000

static struct {
	unsigned int elapsed;
	struct mlk_texture texture;
	int x;
	int y;
} splash;

static void
state_splash_start(void)
{
	struct mlk_font font;

	if (mlk_font_openmem(&font, assets_fonts_zenda, sizeof (assets_fonts_zenda), SIZE) < 0)
		mlk_panic();

	font.style = MLK_FONT_STYLE_ANTIALIASED;

	if (mlk_font_render(&font, &splash.texture, TITLE, FG) < 0)
		mlk_panic();

	mlk_align(MLK_ALIGN_CENTER,
	          &splash.x,
	          &splash.y,
	          splash.texture.w,
	          splash.texture.h,
	          0,
	          0,
	          mlk_window.w,
	          mlk_window.h
	);

	mlk_font_finish(&font);
}

static void
state_splash_update(unsigned int ticks)
{
	splash.elapsed += ticks;

	if (splash.elapsed >= DELAY)
		app_switch(&state_menu);
}

static void
state_splash_draw(void)
{
	mlk_painter_set_color(BG);
	mlk_painter_clear();
	mlk_texture_draw(&splash.texture, splash.x, splash.y);
	mlk_painter_present();
}

const struct mlk_game_ops state_splash = {
	.start  = state_splash_start,
	.update = state_splash_update,
	.draw   = state_splash_draw
};
