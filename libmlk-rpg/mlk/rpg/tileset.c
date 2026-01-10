/*
 * tileset.c -- map tileset definition
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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

#include <mlk/core/animation.h>
#include <mlk/core/sprite.h>

#include "tileset.h"

static inline int
animation_cmp(const void *d1, const void *d2)
{
	const struct mlk_tileset_animation *a1 = d1;
	const struct mlk_tileset_animation *a2 = d2;

	if (a1->id < a2->id)
		return -1;
	if (a1->id > a2->id)
		return 1;

	return 0;
}

static inline const struct mlk_tileset_animation *
find(const struct mlk_tileset *tileset, unsigned int r, unsigned int c)
{
	const struct mlk_tileset_animation key = {
		.id = c + (r * tileset->sprite->ncols)
	};

	return bsearch(&key, tileset->animations, tileset->animationsz, sizeof (key), animation_cmp);
}

int
mlk_tileset_ok(const struct mlk_tileset *tileset)
{
	return tileset && mlk_sprite_ok(tileset->sprite);
}

void
mlk_tileset_start(struct mlk_tileset *tileset)
{
	assert(mlk_tileset_ok(tileset));

	struct mlk_tileset_animation *ta;

	for (size_t i = 0; i < tileset->animationsz; ++i) {
		ta = &tileset->animations[i];

		if (ta->animation)
			mlk_animation_start(ta->animation);
	}
}

void
mlk_tileset_update(struct mlk_tileset *tileset, unsigned int ticks)
{
	assert(mlk_tileset_ok(tileset));

	struct mlk_tileset_animation *ta;

	for (size_t i = 0; i < tileset->animationsz; ++i) {
		ta = &tileset->animations[i];

		if (!ta->animation)
			continue;

		/* Reset in case it ended, we loop animations. */
		if (mlk_animation_update(ta->animation, ticks))
			mlk_animation_start(ta->animation);
	}
}

int
mlk_tileset_draw(const struct mlk_tileset *tileset, unsigned int r, unsigned int c, int x, int y)
{
	assert(mlk_tileset_ok(tileset));

	const struct mlk_tileset_animation *ta;
	int ret;

	if ((ta = find(tileset, r, c)))
		ret = mlk_animation_draw(ta->animation, x, y);
	else
		ret = mlk_sprite_draw(tileset->sprite, r, c, x, y);

	return ret;
}
