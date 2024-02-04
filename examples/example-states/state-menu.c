/*
 * state-menu.c -- basic main menu
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
#include <mlk/core/font.h>
#include <mlk/core/game.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/state.h>
#include <mlk/core/util.h>

#include <mlk/ui/align.h>
#include <mlk/ui/button.h>
#include <mlk/ui/label.h>

#include <assets/fonts/breamcatcher.h>

#include "state-menu.h"
#include "state-play.h"

#define BG         0xdfededff
#define BUTTON_H   (mlk_window.h / 12)
#define BUTTON_W   (mlk_window.w / 6)
#define LABEL_FG   0x4d6a94ff
#define LABEL_FONT assets_fonts_breamcatcher
#define LABEL_SIZE 32
#define LABEL_TEXT "Sample Game"

#define MENU(self) MLK_CONTAINER_OF(self, struct menu, state)

struct menu {
	struct mlk_font font_header;
	struct mlk_label_style label_style_header;
	struct mlk_label label_header;
	struct mlk_button button_play;
	struct mlk_button button_quit;
	struct mlk_state state;
};

static void
state_menu__start(struct mlk_state *self)
{
	struct menu *menu = MENU(self);
	unsigned int w, h;

	/* Top header. */
	if (mlk_font_openmem(&menu->font_header, LABEL_FONT, sizeof (LABEL_FONT), LABEL_SIZE) < 0)
		mlk_panic();

	menu->font_header.style = MLK_FONT_STYLE_ANTIALIASED;
	menu->label_style_header.font = &menu->font_header;
	menu->label_style_header.color = LABEL_FG;
	menu->label_header.text = LABEL_TEXT;
	menu->label_header.style = &menu->label_style_header;
	mlk_label_query(&menu->label_header, &w, &h);
	mlk_align(MLK_ALIGN_TOP,
	          &menu->label_header.x,
	          &menu->label_header.y,
	          w,
	          h,
	          0,
	          10,
	          mlk_window.w,
	          0);

	/* Buttons. */
	menu->button_play.text = "Play";
	menu->button_play.w    = BUTTON_W;
	menu->button_play.h    = BUTTON_H;

	menu->button_quit.text = "Quit";
	menu->button_quit.w    = BUTTON_W;
	menu->button_quit.h    = BUTTON_H;

	mlk_align(MLK_ALIGN_CENTER,
	          &menu->button_play.x,
	          &menu->button_play.y,
	          menu->button_play.w,
	          menu->button_play.h,
	          0,
	          0,
	          mlk_window.w,
	          mlk_window.h
	);

	menu->button_quit.x = menu->button_play.x;
	menu->button_quit.y = menu->button_play.y + BUTTON_H + 20;
}

static void
state_menu__handle(struct mlk_state *self, const union mlk_event *event)
{
	struct menu *menu = MENU(self);

	switch (event->type) {
	case MLK_EVENT_QUIT:
		mlk_game_quit();
		break;
	default:
		if (mlk_button_handle(&menu->button_play, event))
			mlk_game_push(state_play_new());
		else if (mlk_button_handle(&menu->button_quit, event))
			mlk_game_quit();
		break;
	}
}

static void
state_menu__update(struct mlk_state *self, unsigned int ticks)
{
	struct menu *menu = MENU(self);

	mlk_button_update(&menu->button_play, ticks);
	mlk_button_update(&menu->button_quit, ticks);
}

static void
state_menu__draw(struct mlk_state *self)
{
	struct menu *menu = MENU(self);

	mlk_painter_set_color(BG);
	mlk_painter_clear();
	mlk_label_draw(&menu->label_header);
	mlk_button_draw(&menu->button_play);
	mlk_button_draw(&menu->button_quit);
	mlk_painter_present();
}

struct mlk_state *
state_menu_new(void)
{
	struct menu *menu;

	menu = mlk_alloc_new0(1, sizeof (*menu));
	menu->state.name = "menu";
	menu->state.start = state_menu__start;
	menu->state.handle = state_menu__handle;
	menu->state.update = state_menu__update;
	menu->state.draw = state_menu__draw;

	return &menu->state;
}
