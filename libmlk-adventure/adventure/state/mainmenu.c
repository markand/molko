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
#include <core/image.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/state.h>
#include <core/texture.h>
#include <core/util.h>
#include <core/window.h>

#include <ui/align.h>
#include <ui/label.h>
#include <ui/theme.h>

#include <assets/fonts/teutonic.h>
#include <assets/fonts/pirata-one.h>

#include "adventure_p.h"
#include "mainmenu.h"
#include "molko.h"

struct self {
	struct state state;

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
	molko_teleport("assets/maps/map-world.map", -1, -1);
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
perform(struct self *self)
{
	assert(self->itemsel < 3);

	static void (*handlers[])(void) = {
		[0] = new,
		[1] = resume,
		[2] = quit
	};

	handlers[self->itemsel]();
}

static void
init_title(struct self *self, struct font *font)
{
	if (!font_render(font, &self->texts[3].tex, "Molko's Adventure", 0x000000ff))
		panic();
	
	/* Align header. */
	align(ALIGN_CENTER, &self->texts[3].x, NULL, self->texts[3].tex.w, self->texts[3].tex.h,
	    0, 0, window.w, window.h);

	self->texts[3].y = self->texts[3].x;
}

static void
init_items(struct self *self, struct font *font)
{
	if (!font_render(font, &self->texts[0].tex, _("New"), 0x000000ff) ||
	    !font_render(font, &self->texts[1].tex, _("Continue"), 0x000000ff) ||
	    !font_render(font, &self->texts[2].tex, _("Quit"), 0x000000ff))
		panic();

	self->texts[0].x = (window.w / 2) - (self->texts[0].tex.w / 2);
	self->texts[0].y = window.h * 0.75;

	self->texts[1].x = self->texts[0].x;
	self->texts[1].y = self->texts[0].y + self->texts[0].tex.h;

	self->texts[2].x = self->texts[0].x;
	self->texts[2].y = self->texts[1].y + self->texts[1].tex.h;
}

static void
start(struct state *state)
{
	struct self *self = state->data;
	struct font fonts[2];

	if (!font_openmem(&fonts[0], fonts_teutonic, sizeof (fonts_teutonic), 130) ||
	    !font_openmem(&fonts[1], fonts_pirata_one, sizeof (fonts_pirata_one), 30))
		panic();

	fonts[0].style = fonts[1].style = FONT_STYLE_ANTIALIASED;

	init_title(self, &fonts[0]);
	init_items(self, &fonts[1]);

	font_finish(&fonts[0]);
	font_finish(&fonts[1]);
}

static void
handle(struct state *state, const union event *event)
{
	struct self *self = state->data;

	switch (event->type) {
	case EVENT_KEYDOWN:
		switch (event->key.key) {
		case KEY_UP:
			self->itemsel = self->itemsel == 0 ? 2 : self->itemsel - 1;
			break;
		case KEY_DOWN:
			self->itemsel = (self->itemsel + 1) % 3;
			break;
		case KEY_ENTER:
			perform(self);
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
	struct self *self = state->data;

	painter_set_color(0xffffffff);
	painter_clear();

	for (size_t i = 0; i < NELEM(self->texts); ++i)
		texture_draw(&self->texts[i].tex, self->texts[i].x, self->texts[i].y);

	/* TODO: a sword here. */
	painter_set_color(0x000000ff);
	painter_draw_rectangle(
	    self->texts[self->itemsel].x - 30,
	    self->texts[self->itemsel].y + 11, 15, 15);
	painter_present();
}

static void
finish(struct state *state)
{
	struct self *self = state->data;

	for (size_t i = 0; i < NELEM(self->texts); ++i)
		texture_finish(&self->texts[i].tex);

	free(self);
}

struct state *
mainmenu_state_new(void)
{
	struct self *self;

	self = alloc_new0(sizeof (*self));
	self->state.data = self;
	self->state.start = start;
	self->state.handle = handle;
	self->state.draw = draw;
	self->state.finish = finish;

	return &self->state;
}
