/*
 * align.h -- user interface alignment
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

#ifndef MOLKO_UI_ALIGN_H
#define MOLKO_UI_ALIGN_H

/**
 * \file align.h
 * \brief User interface alignment.
 */

/**
 * \brief Label alignment in bounding box.
 *
 * The alignment is described as following:
 *
 * ```
 * +---------------------+
 * | 2        3        4 |
 * |                     |
 * | 9        1        5 |
 * |                     |
 * | 8        7        6 |
 * +---------------------+
 * ```
 *
 * The default value of 0 (LABEL_ALIGN_NONE) means the alignment isn't used.
 */
enum align {
	ALIGN_NONE,             /*!< No alignment. */
	ALIGN_CENTER,           /*!< Align to the center. */
	ALIGN_TOP_LEFT,         /*!< Top left. */
	ALIGN_TOP,              /*!< Top center (aligned horizontally). */
	ALIGN_TOP_RIGHT,        /*!< Top right. */
	ALIGN_RIGHT,            /*!< Right (aligned vertically). */
	ALIGN_BOTTOM_RIGHT,     /*!< Bottom right. */
	ALIGN_BOTTOM,           /*!< Bottom (aligned horizontally). */
	ALIGN_BOTTOM_LEFT,      /*!< Bottom left. */
	ALIGN_LEFT              /*!< Left (aligned vertically). */
};

/**
 * Align the given object relative to its parent region.
 *
 * The arguments are described as following:
 *
 * ```
 * Example of ALIGN_LEFT (centered vertically but placed on the left.)
 *
 *    px, py
 *      +----------pw----------+
 *      |                      |
 *      | x, y                 |
 *      | +---w---+            |
 *     ph h       |            |
 *      | +-------+            |
 *      |                      |
 *      |                      |
 *      +----------------------+
 * ```
 *
 * As a convenience, x and y are left untouched if align is ALIGN_NONE which
 * means you can set the coordinates to default values and still call the align
 * function without need to check.
 *
 * \param align the desired alignment
 * \param x the pointer to x coordinate to modify (may be NULL)
 * \param y the pointer yo y coordinate to modify (may be NULL)
 * \param w the object width
 * \param h the object height
 * \param px the parent region start
 * \param py the parent region start
 * \param pw the parent region width
 * \param ph the parent region height
 */
void
align(enum align align,
      int *x,
      int *y,
      unsigned int w,
      unsigned int h,
      int px,
      int py,
      unsigned int pw,
      unsigned int ph);

#endif /* !MOLKO_UI_ALIGN_H */
