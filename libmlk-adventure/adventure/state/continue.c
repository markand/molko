/*
 * continue.c -- select save to continue the game
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

#include <core/alloc.h>
#include <core/event.h>
#include <core/game.h>
#include <core/painter.h>
#include <core/state.h>

#include <adventure/dialog/save.h>

#include "mainmenu.h"
#include "continue.h"

struct self {
	struct state state;
	struct dialog_save dialog;
};

static void
start(struct state *state)
{
	struct self *self = state->data;

	dialog_save_init(&self->dialog);
}

static void
handle(struct state *state, const union event *ev)
{
	struct self *self = state->data;
	int selected = 0;

	switch (ev->type) {
	case EVENT_QUIT:
		game_quit();
		break;
	case EVENT_KEYDOWN:
		if (ev->key.key == KEY_ESCAPE)
			game_push(state_mainmenu_new());
		else
			selected = dialog_save_handle(&self->dialog, ev);
		break;
	default:
		selected = dialog_save_handle(&self->dialog, ev);
		break;
	}

	if (selected)
		game_quit();
}

static void
update(struct state *state, unsigned int ticks)
{
	struct self *self = state->data;

	dialog_save_update(&self->dialog, ticks);
}

static void
draw(struct state *state)
{
	struct self *self = state->data;

	painter_set_color(0xffffffff);
	painter_clear();
	dialog_save_draw(&self->dialog);
	painter_present();
}

static void
finish(struct state *state)
{
	struct self *self = state->data;

	dialog_save_finish(&self->dialog);
}

struct state *
state_continue_new(void)
{
	struct self *self;

	self = alloc_new0(sizeof (*self));
	self->state.data = self;
	self->state.start = start;
	self->state.handle = handle;
	self->state.update = update;
	self->state.draw = draw;
	self->state.finish = finish;

	return &self->state;
}
