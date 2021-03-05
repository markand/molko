/*
 * font.h -- basic font management
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

#ifndef MOLKO_CORE_FONT_H
#define MOLKO_CORE_FONT_H

#include <stdbool.h>
#include <stddef.h>

#include "core.h"

struct texture;

enum font_style {
	FONT_STYLE_ANTIALIASED,
	FONT_STYLE_NONE
};

struct font {
	enum font_style style;
	void *handle;
};

CORE_BEGIN_DECLS

bool
font_open(struct font *font, const char *path, unsigned int size);

bool
font_openmem(struct font *font, const void *buffer, size_t buflen, unsigned int size);

bool
font_ok(const struct font *font);

bool
font_render(struct font *font, struct texture *tex, const char *text, unsigned int color);

unsigned int
font_height(const struct font *font);

bool
font_query(const struct font *font, const char *text, unsigned int *w, unsigned int *h);

void
font_finish(struct font *font);

CORE_END_DECLS

#endif /* !MOLKO_CORE_FONT_H */
