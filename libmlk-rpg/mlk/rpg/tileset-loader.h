/*
 * tileset-loader.h -- abstract tileset loader
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

#ifndef MLK_RPG_TILESET_LOADER_H
#define MLK_RPG_TILESET_LOADER_H

/**
 * \file mlk/rpg/tileset-loader.h
 * \brief Abstract tileset loader
 *
 * This module provides a generic way to open tilesets. It uses a callback
 * system whenever an action has to be taken by the user. by itself, this
 * module does not alloate nor owns any data.
 *
 * It is designed in mind that the loader knows how to decode a tileset data
 * format file but has no indication on how it should allocate, arrange and
 * find tileset images and other resources.
 *
 * See tileset-file.h for an implementation of this module using files.
 */

#include <mlk/util/sysconfig.h>

#include <stddef.h>

struct mlk_animation;
struct mlk_sprite;
struct mlk_texture;
struct mlk_tileset;
struct mlk_tileset_animations;
struct mlk_tileset_collision;

/**
 * \struct mlk_tileset_loader
 * \brief Abstract loader structure
 *
 * All function pointers must be set.
 */
struct mlk_tileset_loader {
	/**
	 * (read-write, borrowed, optional)
	 *
	 * Arbitrary user data for callbacks.
	 */
	void *data;

	/**
	 * (read-write)
	 *
	 * Open a texture from the given ident name.
	 *
	 * \param self this loader
	 * \param ident the texture name (or path)
	 * \return a borrowed texture or NULL on failure
	 */
	struct mlk_texture * (*init_texture)(struct mlk_tileset_loader *self,
	                                     const char *ident);

	/**
	 * (read-write)
	 *
	 * Return a sprite that the loader needs.
	 *
	 * \param self this loader
	 * \return a unused sprite
	 * \return a borrowed sprite or NULL on failure
	 */
	struct mlk_sprite * (*init_sprite)(struct mlk_tileset_loader *self);

	/**
	 * (read-write)
	 *
	 * Return an animation that the loader needs.
	 *
	 * \param self this loader
	 * \return a unused animation
	 * \return a borrowed animation or NULL on failure
	 */
	struct mlk_animation * (*init_animation)(struct mlk_tileset_loader *self);

	/**
	 * (read-write)
	 *
	 * Expand the collision array by one element.
	 *
	 * \param self this loader
	 * \param array the old array (can be NULL) to reallocate
	 * \param arraysz the new array size (usually +1 than before)
	 * \return a unused animation
	 */
	struct mlk_tileset_collision * (*expand_collisions)(struct mlk_tileset_loader *self,
	                                                    struct mlk_tileset_collision *array,
	                                                    size_t arraysz);

	/**
	 * (read-write)
	 *
	 * Expand the animation array by one element.
	 *
	 * \param self this loader
	 * \param array the old array (can be NULL) to reallocate
	 * \param arraysz the new array size (usually +1 than before)
	 * \return a unused animation
	 */
	struct mlk_tileset_animation * (*expand_animations)(struct mlk_tileset_loader *self,
	                                                    struct mlk_tileset_animation *array,
	                                                    size_t arraysz);

	/** \cond MLK_PRIVATE_DECLS */
	unsigned int tilewidth;
	unsigned int tileheight;
	/** \endcond MLK_PRIVATE_DECLS */
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Open a tileset from a filesystem path.
 *
 * \pre loader != NULL
 * \pre tileset != NULL
 * \pre data != NULL
 * \param loader the loader
 * \param tileset the tileset destination
 * \param path the path to the tileset file
 * \return 0 on success or an error code on failure
 */
int
mlk_tileset_loader_open(struct mlk_tileset_loader *loader,
                        struct mlk_tileset *tileset,
                        const char *path);

#if defined(MLK_HAVE_FMEMOPEN)

/**
 * Open a tileset from a const binary data.
 *
 * The binary data must be kept alive until the tileset loader is no longer
 * used.
 *
 * \pre loader != NULL
 * \pre tileset != NULL
 * \pre data != NULL
 * \param loader the loader
 * \param tileset the tileset destination
 * \param data the tileset content
 * \param datasz the tileset content length
 * \return 0 on success or an error code on failure
 * \note This function is available only if MLK_HAVE_FMEMOPEN is defined.
 */
int
mlk_tileset_loader_openmem(struct mlk_tileset_loader *loader,
                           struct mlk_tileset *tileset,
                           const void *data,
                           size_t datasz);

#endif

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_RPG_TILESET_LOADER_H */
