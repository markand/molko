/*
 * theme.c -- abstract theming
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
#include <stddef.h>
#include <string.h>

#include <mlk/core/font.h>
#include <mlk/core/maths.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/texture.h>
#include <mlk/core/util.h>

#include <assets/fonts/opensans-light.h>
#include <assets/fonts/opensans-medium.h>
#include <assets/fonts/opensans-regular.h>

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
static struct mlk_theme default_theme = {
	.colors = {
		[MLK_THEME_COLOR_DEBUG]         = 0xff0000ff,
		[MLK_THEME_COLOR_NORMAL]        = 0xffffffff,
		[MLK_THEME_COLOR_SELECTED]      = 0x006554ff,
		[MLK_THEME_COLOR_SHADOW]        = 0x000000ff
	},
	.padding = 10,
	.draw_frame = mlk_frame_draw_default,
	.draw_label = mlk_label_draw_default,
	.draw_button = mlk_button_draw_default,
	.draw_checkbox = mlk_checkbox_draw_default
};

static struct font_catalog {
	const unsigned char *data;
	const size_t datasz;
	unsigned int size;
	struct mlk_font **dest;
	struct mlk_font font;
} default_fonts[] = {
	FONT(assets_fonts_opensans_light, 12, MLK_THEME_FONT_DEBUG),
	FONT(assets_fonts_opensans_regular, 14, MLK_THEME_FONT_INTERFACE),
	FONT(assets_fonts_opensans_medium, 14, MLK_THEME_FONT_IMPORTANT)
};

int
mlk_theme_init(void)
{
	/* Open all fonts. */
	for (size_t i = 0; i < MLK_UTIL_SIZE(default_fonts); ++i) {
		struct font_catalog *fc = &default_fonts[i];

		if (mlk_font_openmem(&fc->font, fc->data, fc->datasz, fc->size) < 0)
			goto failed;

		/* Reference this font into the catalog. */
		*default_fonts[i].dest = &default_fonts[i].font;
	}

	return 0;

failed:
	mlk_theme_finish();

	return -1;
}

struct mlk_theme *
mlk_theme_default(void)
{
	return &default_theme;
}

unsigned int
theme_padding(const struct mlk_theme *t)
{
	return THEME(t)->padding;
}

void
mlk_theme_shallow(struct mlk_theme *dst, const struct mlk_theme *src)
{
	assert(dst);

	memcpy(dst, src ? src : &default_theme, sizeof (*src));
}

void
mlk_theme_draw_frame(const struct mlk_theme *t, const struct mlk_frame *frame)
{
	assert(frame);

	THEME(t)->draw_frame(THEME(t), frame);
}

void
mlk_theme_draw_label(const struct mlk_theme *t, const struct mlk_label *label)
{
	assert(label);

	THEME(t)->draw_label(THEME(t), label);
}

void
mlk_theme_draw_button(const struct mlk_theme *t, const struct mlk_button *button)
{
	assert(button);

	THEME(t)->draw_button(THEME(t), button);
}

void
mlk_theme_draw_checkbox(const struct mlk_theme *t, const struct mlk_checkbox *cb)
{
	assert(cb);

	THEME(t)->draw_checkbox(THEME(t), cb);
}

void
mlk_theme_finish(void)
{
	for (size_t i = 0; i < MLK_UTIL_SIZE(default_fonts); ++i) {
		mlk_font_finish(&default_fonts[i].font);
		*default_fonts[i].dest = NULL;
	}
}
