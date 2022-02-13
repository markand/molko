/*
 * battle-state-closing.c -- battle state (closing)
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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
#include <core/music.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/texture.h>
#include <core/window.h>

#include "battle-state-closing.h"
#include "battle.h"

struct self {
	struct battle_state_closing data;
	struct battle_state state;
};

static int
update(struct battle_state *st, struct battle *bt, unsigned int ticks)
{
	(void)bt;

	return battle_state_closing_update(st->data, ticks);
}

static void
draw(const struct battle_state *st, const struct battle *bt)
{
	(void)bt;

	battle_state_closing_draw(st->data);
}

static void
finish(struct battle_state *st, struct battle *bt)
{
	(void)bt;

	battle_state_closing_finish(st->data);
	free(st->data);
}

void
battle_state_closing_init(struct battle_state_closing *cls)
{
	assert(cls);

	if (texture_new(&cls->texture, window.w, window.h) < 0)
		panic();

	PAINTER_BEGIN(&cls->texture);
	texture_set_blend_mode(&cls->texture, TEXTURE_BLEND_BLEND);
	painter_set_color(0x000000ff);
	painter_clear();
	painter_draw_rectangle(0, 0, window.w, window.h);
	PAINTER_END();
	
	texture_set_alpha_mod(&cls->texture, 0);
}

int
battle_state_closing_update(struct battle_state_closing *cls, unsigned int ticks)
{
	assert(cls);

	cls->elapsed += ticks;

	/* TODO: ??? */
	if (cls->elapsed > 8) {
		cls->elapsed = 0;

		if (cls->alpha == 255) {
			/* TODO: since OpenAL, no notion of global music. */
#if 0
			music_stop(0);
#endif
			return 1;
		}

		cls->alpha += 5;
		texture_set_alpha_mod(&cls->texture, cls->alpha);
	}

	return 0;
}

void
battle_state_closing_draw(const struct battle_state_closing *cls)
{
	assert(cls);

	texture_draw(&cls->texture, 0, 0);
}

void
battle_state_closing_finish(struct battle_state_closing *cls)
{
	assert(cls);

	texture_finish(&cls->texture);
	memset(cls, 0, sizeof (*cls));
}

void
battle_state_closing(struct battle *bt)
{
	assert(bt);

	struct self *self;

	self = alloc_new0(sizeof (*self));
	self->state.data = self;
	self->state.update = update;
	self->state.draw = draw;
	self->state.finish = finish;

	battle_state_closing_init(&self->data);
	battle_switch(bt, &self->state);
}
