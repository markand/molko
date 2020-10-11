/*
 * theme.h -- abstract theming
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

#ifndef MOLKO_THEME_H
#define MOLKO_THEME_H

/**
 * \file theme.h
 * \brief Abstract theming.
 */

#include <stdbool.h>

struct checkbox;
struct button;
struct font;
struct frame;
struct label;

/**
 * \brief Font component.
 */
enum theme_font {
	THEME_FONT_DEBUG,       /*!< Font for debug messages. */
	THEME_FONT_INTERFACE,   /*!< Font for interface elements. */
	THEME_FONT_LAST         /*!< Unused. */
};

/**
 * \brief Theme colors.
 */
enum theme_color {
	THEME_COLOR_DEBUG,      /*!< Debug color font. */
	THEME_COLOR_NORMAL,     /*!< Normal font color. */
	THEME_COLOR_SELECTED,   /*!< Font color for selected elements. */
	THEME_COLOR_SHADOW,     /*!< Shadow color. */
	THEME_COLOR_LAST        /*!< Unused. */
};

/**
 * \brief Abstract theme structure.
 */
struct theme {
	/**
	 * (RW, ref) Fonts catalog.
	 */
	struct font *fonts[THEME_FONT_LAST];

	/**
	 * (RW) Miscellaneous colors.
	 */
	unsigned long colors[THEME_COLOR_LAST];

	/**
	 * (RW) Padding between GUI elements.
	 */
	unsigned int padding;

	/**
	 * Draw a frame.
	 *
	 * This function is used to draw a box usually as a container where UI
	 * elements will be put.
	 *
	 * \see \ref theme_draw_frame
	 */
	void (*draw_frame)(struct theme *, const struct frame *);

	/**
	 * Draw a label.
	 *
	 * \see \ref theme_draw_label
	 */
	void (*draw_label)(struct theme *, const struct label *);

	/**
	 * Draw a button.
	 *
	 * \see \ref theme_draw_button
	 */
	void (*draw_button)(struct theme *, const struct button *);

	/**
	 * Draw a checkbox.
	 *
	 * \see \ref theme_draw_button
	 */
	void (*draw_checkbox)(struct theme *t, const struct checkbox *);
};

/**
 * Initialize the theming system.
 *
 * \return false on errors
 * \warning This function must be called before any other theme functions.
 */
bool
theme_init(void);

/**
 * Get a reference to the default theme.
 *
 * \return A non-owning pointer to a static storage for the default theme
 */
struct theme *
theme_default(void);

/**
 * Get the desired padding between GUI elements.
 *
 * \param t the theme to use (may be NULL)
 * \return the padding in pixels
 */
unsigned int
theme_padding(const struct theme *t);

/**
 * Draw a frame.
 *
 * \pre frame != NULL
 * \param t the theme to use (may be NULL)
 * \param frame the frame
 */
void
theme_draw_frame(struct theme *t, const struct frame *frame);

/**
 * Draw a label.
 *
 * \pre label != NULL
 * \param t the theme to use (may be NULL)
 * \param label the label
 */
void
theme_draw_label(struct theme *t, const struct label *label);

/**
 * Draw a button.
 *
 * \pre button != NULL
 * \param t the theme to use (may be NULL)
 * \param button the button
 */
void
theme_draw_button(struct theme *t, const struct button *button);

/**
 * Draw a checkbox.
 *
 * \param t the theme to use (may be NULL)
 * \param cb the checkbox
 */
void
theme_draw_checkbox(struct theme *t, const struct checkbox *cb);

/**
 * Close associated resources.
 */
void
theme_finish(void);

#endif /* !MOLKO_THEME_H */
