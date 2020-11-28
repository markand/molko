/*
 * battle-state-closing.c -- battle state (closing)
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

#include <core/alloc.h>
#include <core/music.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/texture.h>
#include <core/window.h>

#include "battle.h"
#include "battle-state-closing.h"

struct closing {
	struct battle_state self;
	struct texture texture;
	unsigned int alpha;
	unsigned int elapsed;
};

static bool
update(struct battle_state *st, struct battle *bt, unsigned int ticks)
{
	(void)bt;

	struct closing *closing = st->data;

	closing->elapsed += ticks;

	if (closing->elapsed > 8) {
		closing->elapsed = 0;

		if (closing->alpha == 255) {
			music_stop(0);
			return true;
		}

		closing->alpha += 5;
		texture_set_alpha_mod(&closing->texture, closing->alpha);
	}

	return false;
}

static void
draw(const struct battle_state *st, const struct battle *bt)
{
	(void)bt;

	const struct closing *closing = st->data;

	texture_draw(&closing->texture, 0, 0);
}

static void
finish(struct battle_state *st, struct battle *bt)
{
	(void)bt;

	struct closing *closing = st->data;

	texture_finish(&closing->texture);
	free(closing);
}

void
battle_state_closing(struct battle *bt)
{
	assert(bt);

	struct closing *closing;

	if (!(closing = alloc_new0(sizeof (*closing))) ||
	    !texture_new(&closing->texture, window.w, window.h))
		panic();

	PAINTER_BEGIN(&closing->texture);
	texture_set_blend_mode(&closing->texture, TEXTURE_BLEND_BLEND);
	painter_set_color(0x000000ff);
	painter_clear();
	painter_draw_rectangle(0, 0, window.w, window.h);
	PAINTER_END();

	texture_set_alpha_mod(&closing->texture, 0);

	closing->self.data = closing;
	closing->self.update = update;
	closing->self.draw = draw;
	closing->self.finish = finish;

	battle_switch(bt, &closing->self);
}
