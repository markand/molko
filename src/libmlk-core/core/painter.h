/*
 * painter.h -- basic drawing routines
 *
 * Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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

#ifndef MOLKO_CORE_PAINTER_H
#define MOLKO_CORE_PAINTER_H

#include "core.h"

struct texture;

CORE_BEGIN_DECLS

struct texture *
painter_get_target(void);

void
painter_set_target(struct texture *);

unsigned long
painter_get_color(void);

void
painter_set_color(unsigned long);

void
painter_draw_line(int, int, int, int);

void
painter_draw_point(int, int);

void
painter_draw_rectangle(int, int, unsigned int, unsigned int);

void
painter_draw_circle(int, int, int);

void
painter_clear(void);

void
painter_present(void);

CORE_END_DECLS

#define PAINTER_BEGIN(tex)                                              \
do {                                                                    \
        struct texture *__current_texture__;                            \
                                                                        \
        __current_texture__ = painter_get_target();                     \
        painter_set_target((tex))

#define PAINTER_END()                                                   \
        painter_set_target(__current_texture__);                        \
} while (0)

#endif /* !MOLKO_CORE_PAINTER_H */
