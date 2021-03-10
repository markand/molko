/*
 * panic_state.c -- panic state
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
#include <stdlib.h>
#include <string.h>

#include <core/alloc.h>
#include <core/error.h>
#include <core/event.h>
#include <core/font.h>
#include <core/game.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/state.h>
#include <core/sys.h>
#include <core/texture.h>
#include <core/window.h>

#include <ui/align.h>
#include <ui/theme.h>

#include "panic.h"

#define BACKGROUND 0x4f5070ff
#define FOREGROUND 0xffffffff

#define OUT "molko-adventure.txt"

struct self {
	struct state state;

	struct {
		struct texture tex;
		int x;
		int y;
	} texts[4];
};

static void
die(const char *fmt, ...)
{
	assert(fmt);

	va_list ap;

	va_start(ap, fmt);
	fprintf(stderr, "abort: ");
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	abort();
	exit(1);
}

static void
stop(void)
{
	die("%s", error());
}

static void
dump(void)
{
	FILE *fp;

	if ((fp = fopen(OUT, "w"))) {
		/* TODO: add more info here. */
		fprintf(fp, "Molko's Adventure crash dump report\n");
		fclose(fp);
	}

	abort();
}

static void
handle_keydown(const struct event_key *ev)
{
	assert(ev);

	switch (ev->key) {
	case KEY_q:
		game_quit();
		break;
	case KEY_s:
		dump();
		break;
	default:
		break;
	}
}

static void
start(struct state *state)
{
	struct self *self = state->data;
	struct theme *theme;
	struct font *font;

	/* We remove the panic handler to avoid infinite recursion. */
	panic_handler = stop;

	theme = theme_default();
	font = theme->fonts[THEME_FONT_INTERFACE];

	if (font_render(font, &self->texts[0].tex, "An unrecoverable error occured and the game cannot continue.", FOREGROUND) < 0||
	    font_render(font, &self->texts[1].tex, "Please report the detailed error as provided below.", FOREGROUND) < 0||
	    font_render(font, &self->texts[2].tex, "Press <s> to save information and generate a core dump.", FOREGROUND) < 0||
	    font_render(font, &self->texts[3].tex, "Press <q> to quit without saving information.", FOREGROUND) < 0)
		die("%s", error());

	/* All align x the same. */
	for (size_t i = 0; i < UTIL_SIZE(self->texts); ++i)
		self->texts[i].x = theme->padding;

	/* Header (0-1). */
	self->texts[0].y = theme->padding;
	self->texts[1].y = self->texts[0].y + self->texts[0].tex.h + theme->padding;

	/* Footer. (2-3). */
	self->texts[3].y = window.h - self->texts[2].tex.h - theme->padding;
	self->texts[2].y = self->texts[3].y - self->texts[3].tex.h - theme->padding;
}

static void
handle(struct state *state, const union event *ev)
{
	assert(ev);

	(void)state;

	switch (ev->type) {
	case EVENT_KEYDOWN:
		handle_keydown(&ev->key);
		break;
	default:
		break;
	}
}

static void
draw(struct state *state)
{
	struct self *self = state->data;
	struct theme *theme = theme_default();
	struct texture tex;
	struct font *font;
	int x, y;

	painter_set_color(BACKGROUND);
	painter_clear();

	for (size_t i = 0; i < UTIL_SIZE(self->texts); ++i)
		texture_draw(&self->texts[i].tex, self->texts[i].x, self->texts[i].y);

	/* The error is only available here. */
	font = theme->fonts[THEME_FONT_INTERFACE];

	if (font_render(font, &tex, error(), FOREGROUND) < 0)
		die("%s\n", error());

	align(ALIGN_LEFT, &x, &y, tex.w, tex.h, 0, 0, window.w, window.h);

	texture_draw(&tex, x + theme->padding, y);
	texture_finish(&tex);

	painter_present();
}

static void
finish(struct state *state)
{
	struct self *self = state->data;

	for (size_t i = 0; i < UTIL_SIZE(self->texts); ++i)
		texture_finish(&self->texts[i].tex);

	free(self);
}

struct state *
state_panic_new(void)
{
	struct self *self;

	self = alloc_new0(sizeof (*self));
	self->state.data = self;
	self->state.start = start;
	self->state.handle = handle;
	self->state.draw = draw;
	self->state.finish = finish;

	return &self->state;
}
