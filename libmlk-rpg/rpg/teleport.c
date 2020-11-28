/*
 * teleport.c -- animate and teleport
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
#include <string.h>

#include <core/action.h>
#include <core/game.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/window.h>

#include "map.h"
#include "teleport.h"

static bool
update(struct action *act, unsigned int ticks)
{
	return teleport_update(act->data, ticks);
}

static void
draw(struct action *act)
{
	teleport_draw(act->data);
}

static void
finish(struct action *act)
{
	teleport_finish(act->data);
}

void
teleport_start(struct teleport *tp)
{
	assert(tp);

	if (!texture_new(&tp->fade, window.w, window.h))
		panic();

	texture_set_blend_mode(&tp->fade, TEXTURE_BLEND_BLEND);

	PAINTER_BEGIN(&tp->fade);
	painter_set_color(0x000000ff);
	painter_clear();
	PAINTER_END();

	tp->elapsed = tp->alpha = 0;
}

bool
teleport_update(struct teleport *tp, unsigned int ticks)
{
	assert(tp);

	tp->elapsed += ticks;

	if (tp->elapsed >= 10) {
		if (tp->alpha >= 255) {
			game_switch(tp->state, false);
			return true;
		}

		tp->elapsed = 0;
		tp->alpha += 5;
	}

	return false;
}

void
teleport_draw(struct teleport *tp)
{
	texture_set_alpha_mod(&tp->fade, tp->alpha);
	texture_draw(&tp->fade, 0, 0);
}

void
teleport_finish(struct teleport *tp)
{
	assert(tp);

	texture_finish(&tp->fade);
}

void
teleport_action(struct teleport *tp, struct action *act)
{
	assert(tp && tp->state);
	assert(act);

	memset(act, 0, sizeof (*act));
	act->data = tp;
	act->update = update;
	act->draw = draw;
	act->finish = finish;

	teleport_start(tp);
}
