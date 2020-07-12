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

#include "button.h"
#include "checkbox.h"
#include "font.h"
#include "frame.h"
#include "label.h"
#include "maths.h"
#include "painter.h"
#include "panic.h"
#include "texture.h"
#include "theme.h"
#include "util.h"

#include "assets/fonts/ComicNeue-Regular.h"

#define THEME(t) (t ? t : &default_theme)

#define CHECKBOX_W 16
#define CHECKBOX_H 16
#define CHECKBOX_RAD 6

static struct font default_font;

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
	struct texture tex;
	int x = label->x, y = label->y;
	int *px = &x, *py = &y;

	if (label->flags & LABEL_NO_HCENTER)
		px = NULL;
	if (label->flags & LABEL_NO_VCENTER)
		py = NULL;

	/* Shadow text, only if enabled. */
	if (!(label->flags & LABEL_NO_SHADOW)) {
		t->fonts[THEME_FONT_INTERFACE]->color = t->colors[THEME_COLOR_SHADOW];

		if (!font_render(t->fonts[THEME_FONT_INTERFACE], &tex, label->text))
			panic();

		maths_centerize(px, py, tex.w, tex.h,
		    label->x, label->y, label->w, label->h);

		texture_draw(&tex, x + 1, y + 1);
		texture_finish(&tex);
	}

	/* Normal text. */
	t->fonts[THEME_FONT_INTERFACE]->color = label->color
		? label->color
		: t->colors[THEME_COLOR_NORMAL];

	if (!font_render(t->fonts[THEME_FONT_INTERFACE], &tex, label->text))
		panic();

	maths_centerize(px, py, tex.w, tex.h,
	    label->x, label->y, label->w, label->h);

	texture_draw(&tex, x, y);
	texture_finish(&tex);
}

static void
draw_button(struct theme *t, const struct button *button)
{
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
			.flags = LABEL_NO_HCENTER,
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
        { bin, sizeof (bin), size, &default_theme.fonts[index] }

static struct font_catalog {
	const unsigned char *data;
	const size_t datasz;
	unsigned int size;
	struct font **dest;
	struct font font;
} default_fonts[] = {
	FONT(ComicNeue_Regular, 20, THEME_FONT_INTERFACE)
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
