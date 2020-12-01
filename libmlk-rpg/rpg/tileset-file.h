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

/**
 * \file tileset-file.h
 * \brief Tileset file loader.
 */

#include <stdbool.h>
#include <stddef.h>

#include <core/alloc.h>
#include <core/sprite.h>
#include <core/texture.h>

struct tileset;
struct tileset_tiledef;

/**
 * \brief Context for loading tileset from files
 *
 * This structure own animations, tile definitions and sprites that are
 * associated with the tileset. By this mean, the structure must be kept until
 * the tileset is no longer used.
 *
 * Structure members should not be modified directly but only one exposed in
 * the final tileset destination.
 */
struct tileset_file {
	struct alloc_pool tiledefs;             /*!< (*) Tile definitions. */
	struct alloc_pool anims[2];             /*!< (*) Animations data. */
	struct texture image;                   /*!< (*) Image file. */
	struct sprite sprite;                   /*!< (*) Sprite. */
};

/**
 * Try to load a tileset from a file.
 *
 * This function will load the tileset file along with optional animations in
 * the same directory.
 *
 * \pre tf != NULL
 * \pre tileset != NULL
 * \pre path != NULL
 * \param tf the context file
 * \param tileset the destination tileset
 * \param path path to the tileset
 * \return False on errors.
 */
bool
tileset_file_open(struct tileset_file *tf, struct tileset *tileset, const char *path);

/**
 * Close all resources allocated from the tileset file context.
 *
 * \warning The tileset that was created must not be used anymore.
 * \pre tf != NULL
 * \param tf the tileset to clear
 */
void
tileset_file_finish(struct tileset_file *tf);

#endif /* !MOLKO_RPG_TILESET_FILE_H */