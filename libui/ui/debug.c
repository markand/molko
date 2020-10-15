/*
 * debug.c -- debugging interfaces
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

#include <assert.h>
#include <stdio.h>

#include <core/texture.h>

#include "debug.h"
#include "theme.h"

#define PADDING_X 5
#define PADDING_Y 5

struct debug_options debug_options = {
#if !defined(NDEBUG)
	.enable = true
#endif
};

void
debug_printf(struct debug_report *report, const char *fmt, ...)
{
	assert(report);
	assert(fmt);

	if (!debug_options.enable)
		return;

	va_list ap;

	va_start(ap, fmt);
	debug_vprintf(report, fmt, ap);
	va_end(ap);
}

void
debug_vprintf(struct debug_report *report, const char *fmt, va_list ap)
{
	assert(report);
	assert(fmt);

	if (!debug_options.enable)
		return;

	char line[DEBUG_LINE_MAX];
	struct theme *theme;
	struct font *font;
	struct texture tex;
	unsigned int gapy;

	vsnprintf(line, sizeof (line), fmt, ap);

	theme = report->theme ? report->theme : theme_default();
	font = theme->fonts[THEME_FONT_DEBUG];
	font->color = report->color;

	if (!font_render(font, &tex, line))
		return;

	gapy = (PADDING_Y * (report->count)) +
	    (font_height(font) * (report->count));
	report->count++;

	texture_draw(&tex, PADDING_X, gapy);
	texture_finish(&tex);
}
