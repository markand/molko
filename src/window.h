/*
 * window.h -- basic window management
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

#ifndef MOLKO_WINDOW_H
#define MOLKO_WINDOW_H

/**
 * \file window.h
 * \brief Basic window management.
 */

#include <stdbool.h>
#include <stdint.h>

/**
 * \brief Window opaque object.
 */
struct window;

/**
 * Initialize window.
 *
 * \pre title != NULL
 * \param title the window title
 * \param width the desired width
 * \param height the desired height
 * \return the window or NULL on error
 */
struct window *
window_init(const char *title, unsigned width, unsigned height);

/**
 * Get the current drawing color.
 *
 * \pre window != NULL
 * \param window the window
 * \return the color in RRGGBBAA format
 */
uint32_t
window_get_color(const struct window *window);

/**
 * Set the rendering drawing color.
 *
 * \pre window != NULL
 * \param window the window
 * \param color in RRGGBBAA format
 */
void
window_set_color(struct window *window, uint32_t color);

/**
 * Draw a line.
 *
 * \pre window != NULL
 * \param window the window
 * \param x1 first X coordinate
 * \param y1 first Y coordinate
 * \param x2 second X coordinate
 * \param y2 second Y coordinate
 */
void
window_draw_line(struct window *window, int x1, int y1, int x2, int y2);

/**
 * Draw a pixel point.
 *
 * \pre window != NULL
 * \param window the window
 * \param x the X coordinate
 * \param y the Y coordinate
 */
void
window_draw_point(struct window *window, int x, int y);

/**
 * Draw a rectangle
 *
 * \pre window != NULL
 * \param window the window
 * \param fill set to true to fill the rectangle
 * \param x the X coordinate
 * \param y the Y coordinate
 * \param w the rectangle width
 * \param h the rectangle height
 */
void
window_draw_rectangle(struct window *window,
                      bool fill,
                      int x,
                      int y,
                      unsigned w,
                      unsigned h);

/**
 * Clear the window.
 *
 * \pre window != NULL
 * \param window the window
 */
void
window_clear(struct window *window);

/**
 * Present the window, only call this function one time in the main loop.
 *
 * \pre window != NULL
 * \param window the window
 */
void
window_present(struct window *window);

/**
 * Close the window and destroy associated resources, do not use pointer
 * afterwards.
 *
 * \pre window != NULL
 * \param window the window
 */
void
window_close(struct window *window);

#endif /* !MOLKO_WINDOW_H */
