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

#include "align.h"
#include "button.h"
#include "checkbox.h"
#include "frame.h"
#include "label.h"
#include "theme.h"

#define THEME(t) (t ? t : &default_theme)

/* Default font catalog. */
#define FONT(bin, size, index)                                          \
	{ bin, sizeof (bin), size, &default_theme.fonts[index], {0} }

/* Default theme. */
static struct theme default_theme = {
	.colors = {
		[THEME_COLOR_DEBUG]     = 0xff0000ff,
		[THEME_COLOR_NORMAL]    = 0xffffffff,
		[THEME_COLOR_SELECTED]  = 0x006554ff,
		[THEME_COLOR_SHADOW]    = 0x000000ff
	},
	.padding = 10,
	.draw_frame = frame_draw_default,
	.draw_label = label_draw_default,
	.draw_button = button_draw_default,
	.draw_checkbox = checkbox_draw_default
};

static struct font_catalog {
	const unsigned char *data;
	const size_t datasz;
	unsigned int size;
	struct font **dest;
	struct font font;
} default_fonts[] = {
	FONT(fonts_f25_bank_printer, 10, THEME_FONT_DEBUG),
	FONT(fonts_comic_neue, 18, THEME_FONT_INTERFACE)
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
