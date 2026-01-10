/*
 * sprite.h -- image sprites
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

#ifndef MLK_CORE_SPRITE_H
#define MLK_CORE_SPRITE_H

/**
 * \file mlk/core/sprite.h
 * \brief Image sprites
 *
 * The sprite is a module to help rendering a large image that is split into
 * individual parts. This improves memory usage as several images are loaded in
 * a unique one instead of individual parts.
 *
 * \note The image may not contain space, margins or padding within each cell.
 *
 * ## Usage
 *
 * To use a sprite, fill up the required fields in ::mlk_sprite and then call
 * ::mlk_sprite_init which will calculate the number of rows and columns
 * available in the image.
 */

struct mlk_texture;

/**
 * \struct mlk_sprite
 * \brief Sprite structure
 */
struct mlk_sprite {
	/**
	 * (read-write, borrowed)
	 *
	 * The texture to render.
	 */
	struct mlk_texture *texture;

	/**
	 * (read-write)
	 *
	 * The width of individual cell.
	 */
	unsigned int cellw;

	/**
	 * (read-write)
	 *
	 * The height of individual cell.
	 */
	unsigned int cellh;

	/**
	 * (read-only)
	 *
	 * Number of rows in the image.
	 *
	 * \note only available after calling ::mlk_sprite_init.
	 */
	unsigned int nrows;

	/**
	 * (read-only)
	 *
	 * Number of columns in the image.
	 *
	 * \note only available after calling ::mlk_sprite_init.
	 */
	unsigned int ncols;
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Initialize the sprite by computing the number of rows and columns.
 *
 * Pre-initialize the sprite with required fields before calling this function,
 * it is not required if you compute the number of rows and columns yourself.
 *
 * \pre mlk_sprite_ok(sprite)
 * \param sprite the sprite
 */
void
mlk_sprite_init(struct mlk_sprite *sprite);

/**
 * Indicates if the sprite is properly initialized.
 *
 * \param sprite the sprite to check
 * \return non-zero if structure is usable
 */
int
mlk_sprite_ok(const struct mlk_sprite *sprite);

/**
 * Draw an individual cell from the sprite at the given location.
 *
 * \pre mlk_sprite_ok(sprite)
 * \pre row < sprite->nrows
 * \pre column < sprite->ncols
 * \param sprite the sprite
 * \param row the row index
 * \param column the column index
 * \param x the x coordinate
 * \param y the y coordinate
 * \return 0 on success or any error propagated from ::mlk_texture_draw.
 */
int
mlk_sprite_draw(const struct mlk_sprite *sprite,
                unsigned int row,
                unsigned int column,
                int x,
                int y);

/**
 * Similar to ::mlk_sprite_draw but scale the cell into the bounding rectangle
 * destination.
 *
 * \pre mlk_sprite_ok(sprite)
 * \pre row < sprite->nrows
 * \pre column < sprite->ncols
 * \param sprite the sprite
 * \param row the row index
 * \param column the column index
 * \param x the x coordinate
 * \param y the y coordinate
 * \param w the rectangle width
 * \param h the rectangle height
 * \return 0 on success or any error propagated from ::mlk_texture_draw.
 */
int
mlk_sprite_scale(const struct mlk_sprite *sprite,
                 unsigned int row,
                 unsigned int column,
                 int x,
                 int y,
                 unsigned int w,
                 unsigned int h);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_SPRITE_H */
