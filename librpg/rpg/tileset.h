/*
 * tileset.h -- map tileset definition
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

#ifndef MOLKO_RPG_TILESET_H
#define MOLKO_RPG_TILESET_H

#include <stdbool.h>
#include <stddef.h>

struct sprite;

/**
 * \brief Describe a tile property in a tileset.
 *
 * It can contains an animation and a collision mask.
 */
struct tileset_tiledef {
	unsigned short id;                      /*!< (+) Tile index. */
	short x;                                /*!< (+) Collision region starts in y. */
	short y;                                /*!< (+) Collision region starts in y. */
	unsigned short w;                       /*!< (+) Collision width. */
	unsigned short h;                       /*!< (+) Collision height. */
};

struct tileset_animation {
	unsigned short id;                      /* (*) Tile index. */
	struct animation *animation;            /* (+&?) Animation. */
};

/**
 * \brief Tileset definition.
 */
struct tileset {
	struct tileset_tiledef *tiledefs;       /*!< (+&?) Per tile properties (must be sorted by id). */
	size_t tiledefsz;                       /*!< (+) Number of tile properties. */
	struct tileset_animation *anims;        /*!< (+&?) Per tile animations (must be sorted by id). */
	size_t animsz;                          /*!< (+) Number of tile animations. */
	struct sprite *sprite;                  /*!< (+&) Sprite to generate the terrain. */
};

bool
tileset_ok(const struct tileset *ts);

void
tileset_start(struct tileset *ts);

void
tileset_update(struct tileset *ts, unsigned int ticks);

void
tileset_draw(const struct tileset *ts, unsigned int r, unsigned int c, int x, int y);

#endif /* !MOLKO_RPG_TILESET_H */
