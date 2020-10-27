/*
 * mainmenu.c -- game main menu
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

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <core/alloc.h>
#include <core/event.h>
#include <core/font.h>
#include <core/game.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/state.h>
#include <core/texture.h>
#include <core/util.h>
#include <core/window.h>

#include <ui/align.h>
#include <ui/label.h>
#include <ui/theme.h>

#include <adventure/assets/fonts/teutonic.h>
#include <adventure/assets/fonts/pirata-one.h>

#include "mainmenu.h"

struct mainmenu {
	struct {
		struct texture tex;
		int x;
		int y;
	} texts[4];

	unsigned int itemsel;           /* Selected item. */
};

static void
new(void)
{
	/* TODO: implement here. */
}

static void
resume(void)
{
	/* TODO: implement here. */
}

static void
quit(void)
{
	game_quit();
}

static void
perform(struct mainmenu *main)
{
	assert(main->itemsel < 3);

	static void (*handlers[])(void) = {
		[0] = new,
		[1] = resume,
		[2] = quit
	};

	handlers[main->itemsel]();
}

static void
init_title(struct mainmenu *main, struct font *font)
{
	if (!font_render(font, &main->texts[3].tex, "Molko's Adventure", 0x000000ff))
		panic();
	
	/* Align header. */
	align(ALIGN_CENTER, &main->texts[3].x, NULL, main->texts[3].tex.w, main->texts[3].tex.h,
	    0, 0, window.w, window.h);

	main->texts[3].y = main->texts[3].x;
}

static void
init_items(struct mainmenu *main, struct font *font)
{
	if (!font_render(font, &main->texts[0].tex, "New", 0x000000ff) ||
	    !font_render(font, &main->texts[1].tex, "Continue", 0x000000ff) ||
	    !font_render(font, &main->texts[2].tex, "Quit", 0x000000ff))
		panic();

	main->texts[0].x = (window.w / 2) - (main->texts[0].tex.w / 2);
	main->texts[0].y = window.h * 0.75;

	main->texts[1].x = main->texts[0].x;
	main->texts[1].y = main->texts[0].y + main->texts[0].tex.h;

	main->texts[2].x = main->texts[0].x;
	main->texts[2].y = main->texts[1].y + main->texts[1].tex.h;
}

static void
start(struct state *state)
{
	struct mainmenu *main;
	struct font fonts[2];

	/* Allocate the main menu data. */
	main = (state->data = alloc_zero(1, sizeof (*main)));

	if (!font_openmem(&fonts[0], fonts_teutonic, sizeof (fonts_teutonic), 130) ||
	    !font_openmem(&fonts[1], fonts_pirata_one, sizeof (fonts_pirata_one), 30))
		panic();

	fonts[0].style = fonts[1].style = FONT_STYLE_ANTIALIASED;

	init_title(main, &fonts[0]);
	init_items(main, &fonts[1]);

	font_finish(&fonts[0]);
	font_finish(&fonts[1]);
}

static void
handle(struct state *state, const union event *event)
{
	struct mainmenu *main = state->data;

	switch (event->type) {
	case EVENT_KEYDOWN:
		switch (event->key.key) {
		case KEY_UP:
			main->itemsel = main->itemsel == 0 ? 2 : main->itemsel - 1;
			break;
		case KEY_DOWN:
			main->itemsel = (main->itemsel + 1) % 3;
			break;
		case KEY_ENTER:
			perform(main);
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
draw(struct state *state)
{
	struct mainmenu *main = state->data;

	painter_set_color(0xffffffff);
	painter_clear();

	for (size_t i = 0; i < NELEM(main->texts); ++i)
		texture_draw(&main->texts[i].tex, main->texts[i].x, main->texts[i].y);

	/* TODO: a sword here. */
	painter_set_color(0x000000ff);
	painter_draw_rectangle(
	    main->texts[main->itemsel].x - 30,
	    main->texts[main->itemsel].y + 11, 15, 15);
}

static void
finish(struct state *state)
{
	struct mainmenu *main = state->data;

	if (!main)
		return;

	for (size_t i = 0; i < NELEM(main->texts); ++i)
		texture_finish(&main->texts[i].tex);

	free(main);
	memset(state, 0, sizeof (*state));
}

void
mainmenu_state(struct state *state)
{
	assert(state);

	memset(state, 0, sizeof (*state));
	state->start = start;
	state->handle = handle;
	state->draw = draw;
	state->finish = finish;
}
