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
 *
 * See also \a image.h for usage of textures.
 */

/**
 * \brief Texture object.
 *
 * This object is not publicly defined because it contains
 * implementation-defined data.
 */
struct texture;

/**
 * Simple texture drawing.
 *
 * \pre tex != NULL
 * \param tex the texture
 * \param x the X coordinate
 * \param y the Y coordinate
 */
void
texture_draw(struct texture *tex, int x, int y);

/**
 * Advanced texture drawing.
 *
 * \pre tex != NULL
 * \param tex the texture
 * \param src_x the source rectangle X coordinate
 * \param src_y the source rectangle Y coordinate
 * \param src_width the source rectangle width
 * \param src_height the source rectangle height
 * \param dst_x the destination rectangle X coordinate
 * \param dst_y the destination rectangle Y coordinate
 * \param dst_width the destination rectangle width
 * \param dst_height the destination rectangle height
 * \param angle the angle
 */
void
texture_draw_ex(struct texture *tex,
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
texture_close(struct texture *tex);

#endif /* !MOLKO_TEXTURE_H */
