/*
 * color.h -- basic color routines
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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

#ifndef MLK_CORE_COLOR_H
#define MLK_CORE_COLOR_H

/**
 * \file color.h
 * \brief Basic color routines
 *
 * This modules provides conversion from RGBA and hexadecimal colors.
 *
 * Every function that requires a color will take an `unsigned long` as
 * argument whic is expressed as `0xRRGGBBAA`. Sometimes it's however handy to
 * get an individual color component to adjust its value and this can be done
 * using the convenient macro provided by this module.
 *
 * Example
 *
 * ```c
 * unsigned long color = 0xaabbccdd;
 * unsigned int r = MLK_COLOR_R(color); // 0xaa
 * unsigned int g = MLK_COLOR_G(color); // 0xbb
 * unsigned int b = MLK_COLOR_B(color); // 0xcc
 * unsigned int a = MLK_COLOR_A(color); // 0xdd
 * ```
 */

/**
 * Obtain the red component of the color c.
 *
 * \param c the color
 * \return the red component
 */
#define MLK_COLOR_R(c) \
        ((c >> 24) & 0xff)

/**
 * Obtain the green component of the color c.
 *
 * \param c the color
 * \return the green component
 */
#define MLK_COLOR_G(c) \
        ((c >> 16) & 0xff)

/**
 * Obtain the blue component of the color c.
 *
 * \param c the color
 * \return the green component
 */
#define MLK_COLOR_B(c) \
        ((c >> 8) & 0xff)

/**
 * Obtain the alpha component of the color c.
 *
 * \param c the color
 * \return the alpha component
 */
#define MLK_COLOR_A(c) \
        ((c) & 0xff)

/**
 * Create a hexadecimal color from individual red, green, blue and alpha color
 * components.
 *
 * \param r the red component
 * \param g the green component
 * \param b the blue component
 * \param a the alpha component
 * \return the hexadecimal color
 */
#define MLK_COLOR_HEX(r, g, b, a)       \
        (((r << 24) & 0xff000000) |     \
         ((g << 16) & 0x00ff0000) |     \
         ((b << 8)  & 0x0000ff00) |     \
         ((a)       & 0x000000ff))

unsigned long
mlk_color_darken(unsigned long color, float perc);

#endif /* !MLK_CORE_COLOR_H */
