/*
 * texture.h -- basic texture management
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

#ifndef MOLKO_TEXTURE_H
#define MOLKO_TEXTURE_H

/**
 * \file texture.h
 * \brief Basic texture management.
 * \ingroup drawing
 *
 * See also \a image.h for usage of textures.
 */

#include <stdbool.h>

/**
 * \brief Texture object.
 */
struct texture {
	unsigned int w;         /*!< (-) Texture width. */
	unsigned int h;         /*!< (-) Texture height. */
	void *handle;           /*!< (*) Native handle. */
};

/**
 * Create a new texture.
 *
 * \pre tex != NULL
 * \param tex the texture to initialize
 * \param w the width
 * \param h the height
 * \return False on error.
 */
bool
texture_new(struct texture *tex, unsigned int w, unsigned int h);

/**
 * Check if the texture is valid.
 *
 * This function simply checks if the texture is initialized and has non-null
 * dimensions.
 *
 * \param tex the texture to check (may be NULL)
 * \return True if the texture is properly initialized.
 */
bool
texture_ok(const struct texture *tex);

/**
 * Simple texture drawing.
 *
 * \pre tex != NULL
 * \param tex the texture
 * \param x the X coordinate
 * \param y the Y coordinate
 * \return False in case of rendering error.
 */
bool
texture_draw(struct texture *tex, int x, int y);

/**
 * Advanced texture drawing.
 *
 * \pre tex != NULL
 * \param tex the texture
 * \param src_x the source rectangle X coordinate
 * \param src_y the source rectangle Y coordinate
 * \param src_w the source rectangle width
 * \param src_h the source rectangle height
 * \param dst_x the destination rectangle X coordinate
 * \param dst_y the destination rectangle Y coordinate
 * \param dst_w the destination rectangle width
 * \param dst_h the destination rectangle height
 * \param angle the angle
 * \return False in case of rendering error.
 */
bool
texture_scale(struct texture *tex,
              int src_x,
              int src_y,
              unsigned src_w,
              unsigned src_h,
              int dst_x,
              int dst_y,
              unsigned dst_w,
              unsigned dst_h,
              double angle);

/**
 * Close the texture, do not use afterwards.
 *
 * \pre tex != NULL
 * \param tex the texture
 */
void
texture_finish(struct texture *tex);

#endif /* !MOLKO_TEXTURE_H */
