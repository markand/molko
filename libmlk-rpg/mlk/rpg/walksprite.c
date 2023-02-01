/*
 * walksprite.c -- sprite designed for walking entities
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

#include <assert.h>
#include <string.h>

#include <mlk/core/sprite.h>

#include "walksprite.h"

void
walksprite_init(struct walksprite *ws, struct sprite *sprite, unsigned int delay)
{
	assert(ws);
	assert(sprite);

	memset(ws, 0, sizeof (*ws));
	ws->sprite = sprite;
	ws->delay = delay;
}

void
walksprite_reset(struct walksprite *ws)
{
	assert(ws);

	ws->index = 0;
}

void
walksprite_update(struct walksprite *ws, unsigned int ticks)
{
	assert(ws);

	ws->elapsed += ticks;

	if (ws->elapsed >= ws->delay) {
		ws->index += 1;

		if (ws->index >= ws->sprite->ncols)
			ws->index = 0;

		ws->elapsed = 0;
	}
}

void
walksprite_draw(const struct walksprite *ws, unsigned int orientation, int x, int y)
{
	assert(ws);
	assert(orientation < 8);

	sprite_draw(ws->sprite, orientation, ws->index, x, y);
}
