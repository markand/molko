/*
 * teleport.c -- teleport contact
 *
 * Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <core/action.h>
#include <core/alloc.h>
#include <core/maths.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/texture.h>
#include <core/window.h>

#include <rpg/map.h>

#include <adventure/molko.h>

#include "teleport.h"

static void
draw(struct action *act)
{
	struct teleport *tp = act->data;

	texture_set_blend_mode(&tp->overlay, TEXTURE_BLEND_BLEND);
	texture_set_alpha_mod(&tp->overlay, tp->alpha);

	PAINTER_BEGIN(&tp->overlay);
	painter_set_color(0x000000ff);
	painter_clear();
	PAINTER_END();

	texture_draw(&tp->overlay, 0, 0);
}

static int
update_fadeout(struct action *act, unsigned int ticks)
{
	struct teleport *tp = act->data;

	tp->elapsed += ticks;

	if (tp->elapsed >= 10) {
		if (tp->alpha >= 255) {
			molko_teleport(tp->destination, tp->origin_x, tp->origin_y);
			return 1;
		}

		tp->elapsed = 0;
		tp->alpha += 5;
	}

	return 0;
}

static int
update_touch(struct action *act, unsigned int ticks)
{
	(void)ticks;

	struct teleport *tp  = act->data;
	const int x          = tp->x - tp->map->player_sprite->cellw;
	const int y          = tp->y - tp->map->player_sprite->cellh;
	const unsigned int w = tp->w + tp->map->player_sprite->cellw;
	const unsigned int h = tp->h + tp->map->player_sprite->cellh;

	if (maths_is_boxed(x, y, w, h, tp->map->player_x, tp->map->player_y)) {
		/* Stop movement and disable input. */
		tp->map->player_movement = 0;
		game.inhibit = INHIBIT_STATE_INPUT;

		/*
		 * We change our update function and add a draw function that
		 * fade the screen out.
		 */
		if (texture_new(&tp->overlay, window.w, window.h) < 0)
			panic();

		act->update = update_fadeout;
		act->draw = draw;
	}

	return 0;
}

static void
finish(struct action *act)
{
	struct teleport *tp = act->data;

	texture_finish(&tp->overlay);

	free(act->data);
}

struct action *
teleport_action(struct teleport *tp)
{
	assert(tp);

	tp->action.data = tp;
	tp->action.update = update_touch;
	tp->action.finish = finish;

	return &tp->action;
}
