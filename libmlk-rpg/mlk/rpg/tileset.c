/*
 * tileset.c -- map tileset definition
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
#include <stdlib.h>

#include <mlk/core/animation.h>
#include <mlk/core/sprite.h>

#include "tileset.h"

static inline int
anim_cmp(const void *d1, const void *d2)
{
	const struct tileset_animation *mtd1 = d1;
	const struct tileset_animation *mtd2 = d2;

	if (mtd1->id < mtd2->id)
		return -1;
	if (mtd1->id > mtd2->id)
		return 1;

	return 0;
}

static inline const struct tileset_animation *
find(const struct tileset *ts, unsigned int r, unsigned int c)
{
	const struct tileset_animation key = {
		.id = c + (r * ts->sprite->ncols)
	};

	return bsearch(&key, ts->anims, ts->animsz, sizeof (key), anim_cmp);
}

int
tileset_ok(const struct tileset *ts)
{
	return ts && mlk_sprite_ok(ts->sprite);
}

void
tileset_start(struct tileset *ts)
{
	for (size_t i = 0; i < ts->animsz; ++i) {
		struct tileset_animation *ta = &ts->anims[i];

		if (ta->animation)
			mlk_animation_start(ta->animation);
	}
}

void
tileset_update(struct tileset *ts, unsigned int ticks)
{
	for (size_t i = 0; i < ts->animsz; ++i) {
		struct tileset_animation *ta = &ts->anims[i];

		if (!ta->animation)
			continue;

		if (mlk_animation_update(ta->animation, ticks))
			mlk_animation_start(ta->animation);
	}
}

void
tileset_draw(const struct tileset *ts, unsigned int r, unsigned int c, int x, int y)
{
	assert(ts);

	const struct tileset_animation *ta;

	if ((ta = find(ts, r, c)))
		mlk_animation_draw(ta->animation, x, y);
	else
		mlk_sprite_draw(ts->sprite, r, c, x, y);
}
