/*
 * trace_hud.c -- on screen hud
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

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <mlk/core/action.h>
#include <mlk/core/font.h>
#include <mlk/core/trace.h>
#include <mlk/core/window.h>

#include <mlk/ui/label.h>
#include <mlk/ui/theme.h>

#include "trace-hud.h"

#define LINES_MAX       (4)
#define THEME(t)        ((t) ? (t) : theme_default())

static struct {
	char lines[LINES_MAX + 1][TRACE_LINE_MAX];
	unsigned int elapsed;
} data;

struct trace_hud trace_hud = {
	.timeout = TRACE_HUD_TIMEOUT_DEFAULT
};

void
trace_hud_handler(const char *str)
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
trace_hud_update(unsigned int ticks)
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
	if (data.elapsed >= trace_hud.timeout) {
		data.elapsed = 0;
		memmove(&data.lines[0], &data.lines[1], sizeof (data.lines[0]) * LINES_MAX);
	}
}

void
trace_hud_draw(void)
{
	struct theme *th;
	int x, y;

	th = THEME(trace_hud.theme);
	x = th->padding;
	y = th->padding;

	for (int i = 0; i < LINES_MAX && data.lines[i][0]; ++i) {
		mlk_label_draw(&(struct mlk_label) {
			.x = x,
			.y = y,
			.text = data.lines[i],
			.theme = th,
			.flags = MLK_LABEL_FLAGS_SHADOW
		});

		y += mlk_font_height(th->fonts[THEME_FONT_INTERFACE]);
		y += th->padding;
	}
}

void
trace_hud_clear(void)
{
	memset(&data, 0, sizeof (data));
}

static int
update(struct mlk_action *a, unsigned int ticks)
{
	(void)a;

	trace_hud_update(ticks);

	return 0;
}

static void
draw(struct mlk_action *a)
{
	(void)a;

	trace_hud_draw();
}

struct mlk_action *
trace_hud_action(void)
{
	static struct mlk_action a = {
		.update = update,
		.draw = draw
	};

	return &a;
}
