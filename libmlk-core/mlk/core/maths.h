/*
 * maths.h -- basic maths
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

#ifndef MLK_CORE_MATHS_H
#define MLK_CORE_MATHS_H

/**
 * \file maths.h
 * \brief Basic maths
 */

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Returns non-zero if the coordinates x, y inside the bounding rectangle.
 *
 * \param x the x coordinate
 * \param y the y coordinate
 * \param px rectangle x coordinate
 * \param py rectangle y coordinate
 * \param pw rectangle width
 * \param ph rectangle height
 * \return non-zero if x, y is within pw, ph, px, py
 */
int
mlk_maths_is_boxed(int x, int y, int px, int py, unsigned int pw, unsigned int ph);

/**
 * Scale a value within two ranges.
 *
 * \param in the value to scale
 * \param old_min old minimum
 * \param old_max old maximum
 * \param new_min new minimum
 * \param new_max new maximum
 * \return scaled value
 */
float
mlk_maths_scale(float in, float old_min, float old_max, float new_min, float new_max);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_MATHS_H */
