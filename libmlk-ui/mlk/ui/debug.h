/*
 * debug.h -- debugging interfaces
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

#ifndef MLK_UI_DEBUG_H
#define MLK_UI_DEBUG_H

#include <stdarg.h>

#include <mlk/core/core.h>
#include <mlk/core/font.h>

#define MLK_DEBUG_LINE_MAX 256

struct mlk_theme;

struct mlk_debug_options {
	int enable;
};

struct mlk_debug_report {
	const struct mlk_theme *theme;
	unsigned int count;
};

MLK_CORE_BEGIN_DECLS

extern struct mlk_debug_options mlk_debug_options;

void
mlk_debugf(struct mlk_debug_report *, const char *, ...);


void
mlk_debugva(struct mlk_debug_report *, const char *, va_list);

MLK_CORE_END_DECLS

#endif /* !MLK_UI_DEBUG_H */
