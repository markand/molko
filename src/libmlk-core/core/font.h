/*
 * font.h -- basic font management
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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
struct vfs_file;

enum font_style {
	FONT_STYLE_ANTIALIASED,
	FONT_STYLE_NONE,
	FONT_STYLE_LAST
};

struct font {
	enum font_style style;
	void *handle;
};

CORE_BEGIN_DECLS

int
font_open(struct font *, const char *, unsigned int);

int
font_openmem(struct font *, const void *, size_t, unsigned int);

int
font_openvfs(struct font *, struct vfs_file *, unsigned int);

int
font_ok(const struct font *);

int
font_render(struct font *, struct texture *, const char *, unsigned long);

unsigned int
font_height(const struct font *);

int
font_query(const struct font *, const char *, unsigned int *, unsigned int *);

void
font_finish(struct font *);

CORE_END_DECLS

#endif /* !MLK_CORE_FONT_H */
