/*
 * font.h -- basic font management
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

#ifndef MLK_CORE_FONT_H
#define MLK_CORE_FONT_H

#include <stddef.h>

#include "core.h"

struct texture;

enum mlk_font_style {
	MLK_FONT_STYLE_ANTIALIASED,
	MLK_FONT_STYLE_NONE,
	MLK_FONT_STYLE_LAST
};

struct mlk_font {
	enum mlk_font_style style;
	void *handle;
};

CORE_BEGIN_DECLS

int
mlk_font_open(struct mlk_font *, const char *, unsigned int);

int
mlk_font_openmem(struct mlk_font *, const void *, size_t, unsigned int);

int
mlk_font_ok(const struct mlk_font *);

int
mlk_font_render(struct mlk_font *, struct texture *, const char *, unsigned long);

unsigned int
mlk_font_height(const struct mlk_font *);

int
mlk_font_query(const struct mlk_font *, const char *, unsigned int *, unsigned int *);

void
mlk_font_finish(struct mlk_font *);

CORE_END_DECLS

#endif /* !MLK_CORE_FONT_H */
