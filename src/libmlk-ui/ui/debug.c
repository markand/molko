/*
 * debug.c -- debugging interfaces
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

#include <assert.h>
#include <stdio.h>

#include <mlk/core/texture.h>

#include "debug.h"
#include "theme.h"

struct debug_options debug_options = {
#if !defined(NDEBUG)
	.enable = 1
#endif
};

void
debugf(struct debug_report *report, const char *fmt, ...)
{
	assert(report);
	assert(fmt);

	if (!debug_options.enable)
		return;

	va_list ap;

	va_start(ap, fmt);
	debugva(report, fmt, ap);
	va_end(ap);
}

void
debugva(struct debug_report *report, const char *fmt, va_list ap)
{
	assert(report);
	assert(fmt);

	if (!debug_options.enable)
		return;

	char line[DEBUG_LINE_MAX];
	const struct theme *theme;
	struct font *font;
	struct texture tex;
	int x, y;

	vsnprintf(line, sizeof (line), fmt, ap);

	theme = report->theme ? report->theme : theme_default();
	font = theme->fonts[THEME_FONT_DEBUG];

	if (font_render(font, &tex, line, theme->colors[THEME_COLOR_DEBUG]) < 0)
		return;

	x = theme->padding;
	y = (theme->padding * (report->count + 1)) + (tex.h * (report->count));
	report->count++;

	texture_draw(&tex, x, y);
	texture_finish(&tex);
}
