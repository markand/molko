/*
 * trace_hud.c -- on screen hud
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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
#include <string.h>

#include <mlk/core/font.h>
#include <mlk/core/trace.h>
#include <mlk/core/window.h>

#include <mlk/ui/align.h>
#include <mlk/ui/ui.h>

#include "trace-hud.h"

#define PAD             (2)
#define LINES_MAX       (8)

static struct {
	char lines[LINES_MAX + 1][MLK_TRACE_LINE_MAX];
	unsigned int elapsed;
} data;

struct mlk_trace_hud mlk_trace_hud = {
	.timeout = MLK_TRACE_HUD_TIMEOUT_DEFAULT
};

void
mlk_trace_hud_handler(const char *str)
{
	assert(str);

	/* 1.Try to find an empty line. */
	for (size_t i = 0; i < LINES_MAX; ++i) {
		if (data.lines[i][0] == '\0') {
			snprintf(data.lines[i], sizeof (data.lines[i]), "%s", str);
			return;
		}
	}

	/* 2. All lines are full, put in last one and move other. */
	memmove(&data.lines[0], &data.lines[1], sizeof (data.lines[0]) * LINES_MAX - 1);
	snprintf(data.lines[LINES_MAX - 1], sizeof (data.lines[0]), "%s", str);

	/* 3. Reset elapsed time now. */
	data.elapsed = 0;
}

void
mlk_trace_hud_update(unsigned int ticks)
{
	data.elapsed += ticks;

	/*
	 * We have an empty line in the data.lines at LINES_MAX, so we simply so
	 * to move the whole array.
	 *
	 * [0] = "abc"
	 * [1] = "def"
	 * [2] = "xyz"
	 * [3] = "zef"
	 * [n] = "ldkf"
	 * [LINES_MAX + 1] = "\0"
	 */
	if (data.elapsed >= mlk_trace_hud.timeout) {
		data.elapsed = 0;
		memmove(&data.lines[0], &data.lines[1], sizeof (data.lines[0]) * LINES_MAX);
	}
}

void
mlk_trace_hud_draw(void)
{
	struct mlk_font *font;
	unsigned fh;

	font = &mlk_ui_fonts[MLK_UI_FONT_INTERFACE];
	fh = mlk_font_height(font);

	for (int i = 0; i < LINES_MAX && data.lines[i][0]; ++i) {
		mlk_ui_draw_text(
			MLK_ALIGN_NONE,
			font,
			MLK_UI_COLOR_DEBUG,
			data.lines[i],
			PAD,
			PAD + (i * fh),
			0,
			0
		);
	}
}

void
mlk_trace_hud_clear(void)
{
	memset(&data, 0, sizeof (data));
}
