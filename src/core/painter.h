/*
 * painter.h -- basic drawing routines
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

#ifndef MOLKO_PAINTER_H
#define MOLKO_PAINTER_H

/**
 * \file painter.h
 * \brief Basic drawing routines.
 * \ingroup drawing
 */

#include <stdbool.h>

struct texture;

/**
 * Give the current texture being used for rendering, maybe NULL if the
 * rendering is on root.
 *
 * \return Texture or NULL.
 */
struct texture *
painter_get_target(void);

/**
 * Set the rendering context to the given texture.
 *
 * Since this function change an internal global variable it is strongly
 * encouraged to store a local backup of the current texture using \a
 * painter_get_target and call this function with it afterwards.
 *
 * If texture is NULL, use default context aka the window.
 *
 * \param tex the texture
 * \see \ref PAINTER_BEGIN
 * \see \ref PAINTER_END
 */
void
painter_set_target(struct texture *tex);

/**
 * Get the current drawing color.
 *
 * \return the color in RRGGBBAA format
 */
unsigned long
painter_get_color(void);

/**
 * Set the rendering drawing color.
 *
 * \param color in RRGGBBAA format
 */
void
painter_set_color(unsigned long color);

/**
 * Draw a line.
 *
 * \param x1 first X coordinate
 * \param y1 first Y coordinate
 * \param x2 second X coordinate
 * \param y2 second Y coordinate
 */
void
painter_draw_line(int x1, int y1, int x2, int y2);

/**
 * Draw a pixel point.
 *
 * \param x the X coordinate
 * \param y the Y coordinate
 */
void
painter_draw_point(int x, int y);

/**
 * Draw a rectangle
 *
 * \param x the X coordinate
 * \param y the Y coordinate
 * \param w the rectangle width
 * \param h the rectangle height
 */
void
painter_draw_rectangle(int x, int y, unsigned int w, unsigned int h);

/**
 * Clear the window.
 */
void
painter_clear(void);

/**
 * Present the window, only call this function one time in the main loop.
 */
void
painter_present(void);

/**
 * Use this macro to start painting on the texture to store the current
 * rendering context and put it back afterwards.
 *
 * \pre tex != NULL
 * \param tex the texture to use
 * \see \ref PAINTER_END
 */
#define PAINTER_BEGIN(tex    )                                          \
do {                                                                    \
        struct texture *__current_texture__;                            \
                                                                        \
        __current_texture__ = painter_get_target();                     \
        painter_set_target((tex))

/**
 * Use this macro at the end of rendering into a given texture.
 *
 * \see \ref PAINTER_BEGIN
 */
#define PAINTER_END()                                                   \
        painter_set_target(__current_texture__);                        \
} while (0)

#endif /* !MOLKO_PAINTER_H */
