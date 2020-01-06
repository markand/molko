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
 * Initialize window.
 *
 * \pre title != NULL
 * \param title the window title
 * \param width the desired width
 * \param height the desired height
 * \return true on success
 */
bool
window_init(const char *title, unsigned width, unsigned height);

/**
 * Get the current drawing color.
 *
 * \return the color in RRGGBBAA format
 */
uint32_t
window_get_color(void);

/**
 * Set the rendering drawing color.
 *
 * \param color in RRGGBBAA format
 */
void
window_set_color(uint32_t color);

/**
 * Draw a line.
 *
 * \param x1 first X coordinate
 * \param y1 first Y coordinate
 * \param x2 second X coordinate
 * \param y2 second Y coordinate
 */
void
window_draw_line(int x1, int y1, int x2, int y2);

/**
 * Draw a pixel point.
 *
 * \param x the X coordinate
 * \param y the Y coordinate
 */
void
window_draw_point(int x, int y);

/**
 * Draw a rectangle
 *
 * \param fill set to true to fill the rectangle
 * \param x the X coordinate
 * \param y the Y coordinate
 * \param w the rectangle width
 * \param h the rectangle height
 */
void
window_draw_rectangle(bool fill, int x, int y, unsigned w, unsigned h);

/**
 * Clear the window.
 */
void
window_clear(void);

/**
 * Present the window, only call this function one time in the main loop.
 */
void
window_present(void);

/**
 * Close the window and destroy associated resources, do not use pointer
 * afterwards.
 */
void
window_close(void);

#endif /* !MOLKO_WINDOW_H */
