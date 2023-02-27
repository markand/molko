/*
 * color.h -- basic color routines
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

#ifndef MLK_CORE_COLOR_H
#define MLK_CORE_COLOR_H

#define MLK_COLOR_R(c)                  ((c >> 24) & 0xff)
#define MLK_COLOR_G(c)                  ((c >> 16) & 0xff)
#define MLK_COLOR_B(c)                  ((c >> 8) & 0xff)
#define MLK_COLOR_A(c)                  ((c) & 0xff)

#define MLK_COLOR_HEX(r, g, b, a)       \
        (((r << 24) & 0xff000000) |     \
         ((g << 16) & 0x00ff0000) |     \
         ((b << 8)  & 0x0000ff00) |     \
         ((a)       & 0x000000ff))

#endif /* !MLK_CORE_COLOR_H */
