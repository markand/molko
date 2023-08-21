/*
 * ui.c -- libui convenient header
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

#include "sysconfig.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include <libintl.h>

#include <SDL.h>

#include <mlk/core/err.h>
#include <mlk/core/event.h>
#include <mlk/core/font.h>
#include <mlk/core/texture.h>
#include <mlk/core/trace.h>
#include <mlk/core/util.h>
#include <mlk/core/sys.h>

#include <assets/fonts/opensans-regular.h>

#include "align.h"
#include "button.h"
#include "checkbox.h"
#include "frame.h"
#include "label.h"
#include "ui.h"
#include "ui_p.h"

/* Default font catalog. */
#define FONT_DEF(b, s, i)                                               \
{                                                                       \
        .data = b,                                                      \
        .datasz = sizeof (b),                                           \
        .size = s,                                                      \
        .font = &mlk_ui_fonts[i]                                        \
}

static struct font_def {
	const unsigned char *data;
	const size_t datasz;
	unsigned int size;
	struct mlk_font *font;
} fonts[MLK_UI_FONT_LAST] = {
	FONT_DEF(assets_fonts_opensans_regular, 14, MLK_UI_FONT_INTERFACE)
};

/* Global fonts. */
struct mlk_font mlk_ui_fonts[MLK_UI_FONT_LAST] = {};

int
mlk_ui_init(void)
{
	struct font_def *def;

	/* Open all fonts and set the appropriate pointer address. */
	for (size_t i = 0; i < MLK_UTIL_SIZE(fonts); ++i) {
		def = &fonts[i];

		if (mlk_font_openmem(def->font, def->data, def->datasz, def->size) < 0) {
			mlk_ui_finish();
			return -1;
		}
	}

#if defined(MLK_WITH_NLS)
	bindtextdomain("libmlk-ui", mlk_sys_dir(MLK_SYS_DIR_LOCALES));
#endif

	return 0;
}

void
mlk_ui_handle(const union mlk_event *ev)
{
	if (ev->type == MLK_EVENT_THEME)
		mlk_ui_set_theme(ev->theme.theme);
}

void
mlk_ui_set_theme(enum mlk_window_theme theme)
{
	if (theme == MLK_WINDOW_THEME_DARK) {
		mlk_button_style = &mlk_button_style_dark;
		mlk_checkbox_style = &mlk_checkbox_style_dark;
		mlk_frame_style = &mlk_frame_style_dark;
		mlk_label_style = &mlk_label_style_dark;
	} else {
		mlk_button_style = &mlk_button_style_light;
		mlk_checkbox_style = &mlk_checkbox_style_light;
		mlk_frame_style = &mlk_frame_style_light;
		mlk_label_style = &mlk_label_style_light;
	}
}

void
mlk_ui_draw_text(enum mlk_align align,
                 struct mlk_font *font,
                 unsigned long color,
                 const char *text,
                 int px,
                 int py,
                 unsigned int pw,
                 unsigned int ph)
{
	assert(font);
	assert(text && strlen(text) > 0);

	struct mlk_texture texture;
	int x, y;

	if (mlk_font_render(font, &texture, text, color) < 0)
		mlk_tracef(_("unable to render text: %s"), mlk_err());
	else {
		mlk_align(align, &x, &y, texture.w, texture.h, px, py, pw, ph);
		mlk_texture_draw(&texture, x, y);
		mlk_texture_finish(&texture);
	}
}

void
mlk_ui_finish(void)
{
}
