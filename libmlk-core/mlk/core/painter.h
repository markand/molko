/*
 * painter.h -- basic drawing routines
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

#ifndef MLK_CORE_PAINTER_H
#define MLK_CORE_PAINTER_H

#include "core.h"

struct texture;

MLK_CORE_BEGIN_DECLS

struct texture *
mlk_painter_get_target(void);

void
mlk_painter_set_target(struct texture *);

unsigned long
mlk_painter_get_color(void);

void
mlk_painter_set_color(unsigned long);

void
mlk_painter_draw_line(int, int, int, int);

void
mlk_painter_draw_point(int, int);

void
mlk_painter_draw_rectangle(int, int, unsigned int, unsigned int);

void
mlk_painter_draw_circle(int, int, int);

void
mlk_painter_clear(void);

void
mlk_painter_present(void);

MLK_CORE_END_DECLS

#define MLK_PAINTER_BEGIN(tex)                                          \
do {                                                                    \
        struct texture *__current_texture__;                            \
                                                                        \
        __current_texture__ = mlk_painter_get_target();                 \
        mlk_painter_set_target((tex))

#define MLK_PAINTER_END()                                               \
        mlk_painter_set_target(__current_texture__);                    \
} while (0)

#endif /* !MLK_CORE_PAINTER_H */
