/*
 * tileset-loader-file.h -- tileset file loader implementation
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

#ifndef MLK_RPG_TILESET_LOADER_FILE_H
#define MLK_RPG_TILESET_LOADER_FILE_H

/**
 * \file mlk/rpg/tileset-loader-file.h
 * \brief Tileset file loader implementation
 *
 * This convenient tileset loader loads a tileset from file and its associative
 * resources relative to the tileset file directory.
 *
 * It also allocate memory for individual element and as such must be kept
 * valid until the tileset is no longer used. If this behavior is not wanted,
 * the allocator functions can be changed.
 */

#include "tileset-loader.h"

struct mlk_tileset_collision;
struct mlk_tileset_animation;
struct mlk__loader_file;

struct mlk_tileset_loader_file {
	/**
	 * (read-write)
	 *
	 * Underlying tileset loader.
	 */
	struct mlk_tileset_loader iface;
	
	/** \cond MLK_PRIVATE_DECLS */
	struct mlk_tileset_collision *tilecollisions;
	struct mlk_tileset_animation *tileanimations;
	struct mlk__loader_file *lf;
	/** \endcond MLK_PRIVATE_DECLS */
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Initialize the loader with internal functions and internal data to allocate
 * and find appropriate resources relative to the tileset filename.
 *
 * After loading the tileset with this underlying loader, it should be kept
 * until the tileset is no longer used.
 *
 * \pre file != NULL
 * \pre filename != NULL
 * \param file the abstract loader interface
 * \param filename path to the tileset file
 * \return 0 on success or -1 on error
 */
int
mlk_tileset_loader_file_init(struct mlk_tileset_loader_file *file, const char *filename);

/**
 * Cleanup allocated resources by this file loader.
 *
 * \pre file != NULL
 * \param file the file loader
 */
void
mlk_tileset_loader_file_finish(struct mlk_tileset_loader_file *file);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_RPG_TILESET_LOADER_FILE_H */
