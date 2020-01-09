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
 */

#include <stdint.h>

struct texture;

/**
 * \brief Sprite structure.
 */
struct sprite {
	struct texture *texture;        /*!< Texture to access (RO) */
	uint16_t cellw;                 /*!< Width per cell (RW) */
	uint16_t cellh;                 /*!< Height per cell (RW) */
	uint16_t nrows;                 /*!< Number of rows (RW) */
	uint16_t ncols;                 /*!< Number of columns (RW) */
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
 * \pre tex != NULL
 * \param sprite the sprite to initialize
 * \param tex the texture
 * \param cellw the width per cell in pixels
 * \param cellh the height per cell in pixels
 */
void
sprite_init(struct sprite *sprite,
            struct texture *tex,
            uint16_t cellw,
            uint16_t cellh);

/**
 * Draw the sprite component from row `r' and column `c'.
 *
 * \pre sprite != NULL
 * \param sprite the sprite to draw
 * \param r the row number
 * \param c the column number
 * \param x the X destination
 * \param y the Y destination
 * \warning No bounds checking
 */
void
sprite_draw(struct sprite *sprite, uint16_t r, uint16_t c, int x, int y);

#endif /* !MOLKO_SPRITE_H */
