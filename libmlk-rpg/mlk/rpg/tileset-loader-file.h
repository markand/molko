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
 * This convenient tileset loader loads tilesets from file and its associative
 * resources relative to the tileset file directory.
 *
 * It also allocate memory for individual element and as such must be kept
 * valid until the tileset is no longer used. If this behavior is not wanted,
 * the allocator functions can be changed.
 */

#include <mlk/util/util.h>

struct mlk_animation;
struct mlk_sprite;
struct mlk_texture;
struct mlk_tileset_animation;
struct mlk_tileset_collision;
struct mlk_tileset_loader;

/**
 * \struct mlk_tileset_loader_file
 * \brief Tileset file loader structure
 */
struct mlk_tileset_loader_file {
	/**
	 * (read-only)
	 *
	 * Computed tileset file directory.
	 */
	char directory[MLK_PATH_MAX];

	/** \cond MLK_PRIVATE_DECLS */
	struct mlk_texture **textures;
	struct mlk_sprite **sprites;
	struct mlk_animation **animations;
	struct mlk_tileset_collision *tilecollisions;
	struct mlk_tileset_animation *tileanimations;
	/** \endcond MLK_PRIVATE_DECLS */
};

/**
 * Fill the abstract loader with appropriate implementation.
 *
 * All loader member functions will be set and ::mlk_tileset_loader::data will
 * be set to file loader.
 *
 * The file and loader structure are zero'ed before being initialized.
 *
 * \pre file != NULL
 * \pre loader != NULL
 * \pre filename != NULL
 * \param file the file loader
 * \param loader the abstract loader interface
 * \param filename path to the tileset file
 */
void
mlk_tileset_loader_file_init(struct mlk_tileset_loader_file *file,
                             struct mlk_tileset_loader *loader,
                             const char *filename);

/**
 * Cleanup allocated resources by this file loader.
 *
 * \pre file != NULL
 * \param file the file loader
 */
void
mlk_tileset_loader_file_finish(struct mlk_tileset_loader_file *file);

#endif /* !MLK_RPG_TILESET_LOADER_FILE_H */
