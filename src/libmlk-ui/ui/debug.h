/*
 * debug.h -- debugging interfaces
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

#ifndef MLK_UI_DEBUG_H
#define MLK_UI_DEBUG_H

#include <stdarg.h>

#include <core/core.h>
#include <core/font.h>

#define DEBUG_LINE_MAX 1024

struct theme;

struct debug_options {
	int enable;
};

struct debug_report {
	const struct theme *theme;
	unsigned int count;
};

CORE_BEGIN_DECLS

extern struct debug_options debug_options;

void
debugf(struct debug_report *, const char *, ...);


void
debugva(struct debug_report *, const char *, va_list);

CORE_END_DECLS

#endif /* !MLK_UI_DEBUG_H */
