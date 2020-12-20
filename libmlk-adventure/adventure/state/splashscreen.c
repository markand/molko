/*
 * splashscreen.c -- splash screen state
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
#include <core/font.h>
#include <core/game.h>
#include <core/image.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/state.h>
#include <core/sys.h>
#include <core/texture.h>
#include <core/window.h>

#include <ui/align.h>

#include <adventure/molko.h>

#include "splashscreen.h"
#include "mainmenu.h"

#define DELAY 3000

struct self {
	struct state state;
	struct texture tex;
	int x;
	int y;
	unsigned int elapsed;
};

static void
start(struct state *state)
{
	struct self *self = state->data;
	struct font font;

	if (!font_open(&font, molko_path("fonts/cubic.ttf"), 80))
		panic();

	font.style = FONT_STYLE_ANTIALIASED;

	if (!font_render(&font, &self->tex, "malikania", 0x19332dff))
		panic();

	align(ALIGN_CENTER, &self->x, &self->y, self->tex.w, self->tex.h,
	    0, 0, window.w, window.h);
	font_finish(&font);
}

static void
update(struct state *state, unsigned int ticks)
{
	struct self *self = state->data;

	self->elapsed += ticks;

	if (self->elapsed >= DELAY)
		game_switch(state_mainmenu_new(), false);
}

static void
draw(struct state *state)
{
	struct self *self = state->data;

	painter_set_color(0xffffffff);
	painter_clear();
	texture_draw(&self->tex, self->x, self->y);
	painter_present();
}

static void
finish(struct state *state)
{
	struct self *self = state->data;

	texture_finish(&self->tex);

	free(self);
}

struct state *
state_splashscreen_new(void)
{
	struct self *self;

	self = alloc_new0(sizeof (*self));
	self->state.data = self;
	self->state.start = start;
	self->state.update = update;
	self->state.draw = draw;
	self->state.finish = finish;

	return &self->state;
}
