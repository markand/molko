/*
 * sprite.h -- image sprites
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

#ifndef MOLKO_SPRITE_H
#define MOLKO_SPRITE_H

/**
 * \file sprite.h
 * \brief Image sprites.
 * \ingroup drawing
 *
 * The sprite is a module to help rendering a large image that is split into
 * individual parts. This improves memory usage as several images are loaded
 * in a unique one instead of individual parts.
 *
 * Example of sprite.
 *
 * ```
 * +---+---+---+
 * | 0 | 1 | 2 |
 * +---+---+---+
 * | 3 | 4 | 5 |
 * +---+---+---+
 * ```
 *
 * If an image is designed like this grid, it contains three columns and 2 rows.
 *
 * \note The image may not contain space, margins or padding within each cell.
 */

#include <stdbool.h>

struct texture;

/**
 * \brief Sprite structure.
 */
struct sprite {
	struct texture *texture;        /*!< (+&) Texture to access. */
	unsigned int cellw;             /*!< (-) Width per cell. */
	unsigned int cellh;             /*!< (-) Height per cell. */
	unsigned int nrows;             /*!< (-) Number of rows. */
	unsigned int ncols;             /*!< (-) Number of columns. */
};

/**
 * Initialize a sprite.
 *
 * The sprite does not take ownership of texture and must be valid until the
 * sprite is no longer used.
 *
 * This function is only provided as convenience, user may initialize the
 * sprite by itself if wanted.
 *
 * The fields `nrows' and `ncols' will be determined automatically from the
 * texture size.
 *
 * \pre sprite != NULL
 * \pre tex != NULL && texture_ok(tex)
 * \param sprite the sprite to initialize
 * \param tex the texture
 * \param cellw the width per cell in pixels
 * \param cellh the height per cell in pixels
 */
void
sprite_init(struct sprite *sprite,
            struct texture *tex,
            unsigned int cellw,
            unsigned int cellh);

/**
 * Tells if the sprite has a texture and isn't null sized.
 *
 * \param sprite the sprite to check (may be NULL)
 * \return True if it is initialized correctly.
 */
bool
sprite_ok(const struct sprite *sprite);

/**
 * Draw the sprite component from row `r' and column `c'.
 *
 * \pre r < sprite->nrows
 * \pre c < sprite->ncols
 * \pre sprite != NULL
 * \param sprite the sprite to draw
 * \param r the row number
 * \param c the column number
 * \param x the X destination
 * \param y the Y destination
 * \return False in case of rendering error.
 */
bool
sprite_draw(struct sprite *sprite, unsigned int r, unsigned int c, int x, int y);

#endif /* !MOLKO_SPRITE_H */
