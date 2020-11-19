/*
 * tileset-file.h -- tileset file loader
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

#ifndef MOLKO_RPG_TILESET_FILE_H
#define MOLKO_RPG_TILESET_FILE_H

#include <stdbool.h>
#include <stddef.h>

#include <core/alloc.h>
#include <core/sprite.h>
#include <core/texture.h>

struct tileset;
struct tileset_tiledef;

struct tileset_file {
	struct alloc_pool tiledefs;
	struct alloc_pool anims[2];
	struct texture image;                   /*!< (*) Owned image file. */
	struct sprite sprite;                   /*!< (*) Owned sprite. */
};

bool
tileset_file_open(struct tileset_file *tf, struct tileset *tileset, const char *path);

void
tileset_file_finish(struct tileset_file *tf);

#endif /* !MOLKO_RPG_TILESET_FILE_H */
