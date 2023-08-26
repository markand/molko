/*
 * ui.h -- libui convenient header
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

#ifndef MLK_UI_UI_H
#define MLK_UI_UI_H

#include <mlk/core/core.h>
#include <mlk/core/font.h>

/* TODO: make this a global variable to allow modification of default theme. */
/* https://lospec.com/palette-list/duel */
#define MLK_UI_COLOR_TEXT       (0x222323ff)
#define MLK_UI_COLOR_SELECTED   (0x55b67dff)
#define MLK_UI_COLOR_BG         (0xf5f7faff)
#define MLK_UI_COLOR_BORDER     (0xcdd2daff)
#define MLK_UI_COLOR_DEBUG      (0xe45c5fff)

#define MLK_UI_PADDING          (10)
#define MLK_UI_BORDER           (2)

enum mlk_align;
enum mlk_window_theme;

union mlk_event;

/**
 * \enum mlk_ui_font
 * \brief Predefined font alias.
 */
enum mlk_ui_font {
	/**
	 * Font for UI elements.
	 */
	MLK_UI_FONT_INTERFACE,

	/**
	 * Unused sentinel value.
	 */
	MLK_UI_FONT_LAST
};

/**
 * \brief Global array of predefined fonts.
 */
extern struct mlk_font mlk_ui_fonts[MLK_UI_FONT_LAST];

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Initialize the libmlk-ui library.
 */
int
mlk_ui_init(void);

/**
 * Handle an event in the libmlk-ui library.
 *
 * This function is required to adjust global parameters such as dark/light
 * theme if desired.
 *
 * \pre ev != NULL
 * \param ev the event
 */
void
mlk_ui_handle(const union mlk_event *ev);

/**
 * Apply the light or dark theme to the predefined UI elements.
 *
 * \pre theme must be ::MLK_WINDOW_THEME_DARK or ::MLK_WINDOW_THEME_LIGHT
 * \param theme new theme to set
 */
void
mlk_ui_set_theme(enum mlk_window_theme theme);

/**
 * Convenient helper to draw and align text directly on the current rendering
 * target.
 *
 * \pre font != NULL
 * \pre text != NULL and not empty
 * \param align alignment into the destination region
 * \param font the font to use
 * \param color the text color
 * \param text the text to show
 * \param x the x coordinate
 * \param y the y coordinate
 * \param w the rectangle destination width
 * \param h the rectangle destination height
 */
void
mlk_ui_draw_text(enum mlk_align align,
                 struct mlk_font *font,
                 unsigned long color,
                 const char *text,
                 int x,
                 int y,
                 unsigned int w,
                 unsigned int h);

/**
 * Cleanup resources allocated by the library.
 */
void
mlk_ui_finish(void);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_UI_UI_H */
