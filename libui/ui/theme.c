/*
 * theme.c -- abstract theming
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
#include <stddef.h>
#include <string.h>

#include <core/font.h>
#include <core/maths.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/texture.h>
#include <core/util.h>

#include <core/assets/fonts/f25-bank-printer.h>
#include <core/assets/fonts/comic-neue.h>

#include "button.h"
#include "checkbox.h"
#include "frame.h"
#include "label.h"
#include "theme.h"

#define THEME(t) (t ? t : &default_theme)

#define CHECKBOX_W 16
#define CHECKBOX_H 16
#define CHECKBOX_RAD 6

static void
box(int x, int y, unsigned int w, unsigned int h)
{
	/* Some basic outlines. */
	painter_set_color(0x4d3533ff);

	painter_draw_line(x, y, x + w, y);
	painter_draw_line(x, y + h, x + w, y + h);
	painter_draw_line(x, y, x, y + h);
	painter_draw_line(x + w, y, x + w, y + h);
}

static void
draw_frame(struct theme *t, const struct frame *frame)
{
	(void)t;

	if (frame->style == FRAME_STYLE_BOX)
		painter_set_color(0x6e4c30ff);
	else
		painter_set_color(0xce9248ff);

	painter_draw_rectangle(frame->x, frame->y, frame->w, frame->h);
	box(frame->x, frame->y, frame->w, frame->h);
}

static void
draw_label(struct theme *t, const struct label *label)
{
	struct font *font;
	struct texture tex;
	int x, y, bx, by;
	unsigned int tw, th, bw, bh;

	/* Compute real box size according to padding. */
	bx = label->x + t->padding;
	by = label->y + t->padding;
	bw = label->w - (t->padding * 2);
	bh = label->h - (t->padding * 2);

	/* Make a shallow copy of the interface font. */
	font = t->fonts[THEME_FONT_INTERFACE];

	/* Compute text size. */
	if (!font_box(font, label->text, &tw, &th))
		panic();

	/* Compute position according to alignment and box. */
	switch (label->align) {
	case LABEL_ALIGN_CENTER:
		maths_centerize(&x, &y, tw, th, bx, by, bw, bh);
		break;
	case LABEL_ALIGN_TOP_LEFT:
		x = bx;
		y = by;
		break;
	case LABEL_ALIGN_TOP:
		maths_centerize(&x, NULL, tw, th, bx, by, bw, bh);
		y = by;
		break;
	case LABEL_ALIGN_TOP_RIGHT:
		x = bx + bw - tw;
		y = by;
		break;
	case LABEL_ALIGN_RIGHT:
		maths_centerize(NULL, &y, tw, th, bx, by, bw, bh);
		x = bx + bw - tw;
		break;
	case LABEL_ALIGN_BOTTOM_RIGHT:
		x = bx + bw - tw;
		y = by + bh - th;
		break;
	case LABEL_ALIGN_BOTTOM:
		maths_centerize(&x, NULL, tw, th, bx, by, bw, bh);
		y = by + bh - th;
		break;
	case LABEL_ALIGN_BOTTOM_LEFT:
		x = bx;
		y = by + bh - th;
		break;
	case LABEL_ALIGN_LEFT:
		maths_centerize(NULL, &y, tw, th, bx, by, bw, bh);
		x = bx;
		break;
	default:
		x = label->x;
		y = label->y;
		break;
	}

	/* Shadow text, only if enabled. */
	if (label->flags & LABEL_FLAGS_SHADOW) {
		font->color = t->colors[THEME_COLOR_SHADOW];

		if (!font_render(font, &tex, label->text))
			panic();

		texture_draw(&tex, x + 1, y + 1);
		texture_finish(&tex);
	}

	/* Normal text. */
	font->color = t->colors[THEME_COLOR_NORMAL];

	if (!font_render(font, &tex, label->text))
		panic();

	texture_draw(&tex, x, y);
	texture_finish(&tex);
}

static void
draw_button(struct theme *t, const struct button *button)
{
	(void)t;

	struct label label = {
		.text = button->text,
		.x = button->x,
		.y = button->y,
		.w = button->w,
		.h = button->h
	};

	painter_set_color(0xabcdefff);
	painter_draw_rectangle(button->x, button->y, button->w, button->h);

	label_draw(&label);

	box(button->x, button->y, button->w, button->h);
}

static void
draw_checkbox(struct theme *t, const struct checkbox *cb)
{
	box(cb->x, cb->y, CHECKBOX_W, CHECKBOX_H);

	if (cb->checked)
		painter_draw_rectangle(cb->x + 5, cb->y + 5, CHECKBOX_W - 9, CHECKBOX_H - 9);

	if (cb->label) {
		const unsigned int w = cb->w - (t->padding * 2) - CHECKBOX_W;
		const int x = cb->x + (t->padding * 2) + CHECKBOX_W;

		struct label label = {
			.text = cb->label,
			.align = LABEL_ALIGN_LEFT,
			.x = x,
			.y = cb->y,
			.w = w,
			.h = cb->h
		};

		draw_label(t, &label);
	}
}

/* Default theme. */
static struct theme default_theme = {
	.colors = {
		[THEME_COLOR_NORMAL]    = 0xffffffff,
		[THEME_COLOR_SELECTED]  = 0x006554ff,
		[THEME_COLOR_SHADOW]    = 0x000000ff
	},
	.padding = 10,
	.draw_frame = draw_frame,
	.draw_label = draw_label,
	.draw_button = draw_button,
	.draw_checkbox = draw_checkbox
};

/* Default font catalog. */
#define FONT(bin, size, index)                                          \
	{ bin, sizeof (bin), size, &default_theme.fonts[index], {0} }

static struct font_catalog {
	const unsigned char *data;
	const size_t datasz;
	unsigned int size;
	struct font **dest;
	struct font font;
} default_fonts[] = {
	FONT(fonts_f25_bank_printer, 10, THEME_FONT_DEBUG),
	FONT(fonts_comic_neue, 20, THEME_FONT_INTERFACE)
};

bool
theme_init(void)
{
	/* Open all fonts. */
	for (size_t i = 0; i < NELEM(default_fonts); ++i) {
		struct font_catalog *fc = &default_fonts[i];

		if (!font_openmem(&fc->font, fc->data, fc->datasz, fc->size))
			goto failed;

		/* Reference this font into the catalog. */
		*default_fonts[i].dest = &default_fonts[i].font;
	}

	return true;

failed:
	theme_finish();

	return false;
}

struct theme *
theme_default(void)
{
	return &default_theme;
}

unsigned int
theme_padding(const struct theme *t)
{
	return THEME(t)->padding;
}

void
theme_shallow(struct theme *dst, const struct theme *src)
{
	assert(dst);

	memcpy(dst, src ? src : &default_theme, sizeof (*src));
}

void
theme_draw_frame(struct theme *t, const struct frame *frame)
{
	assert(frame);

	THEME(t)->draw_frame(THEME(t), frame);
}

void
theme_draw_label(struct theme *t, const struct label *label)
{
	assert(label);

	THEME(t)->draw_label(THEME(t), label);
}

void
theme_draw_button(struct theme *t, const struct button *button)
{
	assert(button);

	THEME(t)->draw_button(THEME(t), button);
}

void
theme_draw_checkbox(struct theme *t, const struct checkbox *cb)
{
	assert(cb);

	THEME(t)->draw_checkbox(THEME(t), cb);
}

void
theme_finish(void)
{
	for (size_t i = 0; i < NELEM(default_fonts); ++i) {
		font_finish(&default_fonts[i].font);
		*default_fonts[i].dest = NULL;
	}
}
