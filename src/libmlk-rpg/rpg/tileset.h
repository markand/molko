/*
 * tileset.h -- map tileset definition
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

#ifndef MLK_RPG_TILESET_H
#define MLK_RPG_TILESET_H

#include <stddef.h>

#include <core/core.h>

struct sprite;

struct tileset_tiledef {
	unsigned short id;
	short x;
	short y;
	unsigned short w;
	unsigned short h;
};

struct tileset_animation {
	unsigned short id;
	struct animation *animation;
};

struct tileset {
	struct tileset_tiledef *tiledefs;
	size_t tiledefsz;
	struct tileset_animation *anims;
	size_t animsz;
	struct sprite *sprite;
};

CORE_BEGIN_DECLS

int
tileset_ok(const struct tileset *);

void
tileset_start(struct tileset *);

void
tileset_update(struct tileset *, unsigned int);

void
tileset_draw(const struct tileset *, unsigned int, unsigned int, int, int);

CORE_END_DECLS

#endif /* !MLK_RPG_TILESET_H */
