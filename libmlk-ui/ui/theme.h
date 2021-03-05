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

#ifndef MOLKO_UI_THEME_H
#define MOLKO_UI_THEME_H

/**
 * \file theme.h
 * \brief Abstract theming.
 * \ingroup ui
 */

#include <stdbool.h>

#include <core/core.h>

struct button;
struct checkbox;
struct font;
struct frame;
struct label;
struct sprite;

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
 * \brief Special sprites.
 */
enum theme_sprite {
	THEME_SPRITE_CURSOR,    /*!< Sprite cursor. */
	THEME_SPRITE_LAST,
};

/**
 * \brief Abstract theme structure.
 */
struct theme {
	/**
	 * (+&) Fonts catalog.
	 */
	struct font *fonts[THEME_FONT_LAST];

	/**
	 * (+&?) Special sprites catalog.
	 */
	const struct sprite *sprites[THEME_SPRITE_LAST];

	/**
	 * (+) User interface colors.
	 */
	unsigned long colors[THEME_COLOR_LAST];

	/**
	 * (+) Padding between GUI elements.
	 */
	unsigned int padding;

	/**
	 * (+) Draw a frame.
	 *
	 * This function is used to draw a box usually as a container where UI
	 * elements will be put.
	 *
	 * \see \ref theme_draw_frame
	 */
	void (*draw_frame)(const struct theme *, const struct frame *);

	/**
	 * (+) Draw a label.
	 *
	 * \see \ref theme_draw_label
	 */
	void (*draw_label)(const struct theme *, const struct label *);

	/**
	 * (+) Draw a button.
	 *
	 * \see \ref theme_draw_button
	 */
	void (*draw_button)(const struct theme *, const struct button *);

	/**
	 * (+) Draw a checkbox.
	 *
	 * \see \ref theme_draw_button
	 */
	void (*draw_checkbox)(const struct theme *t, const struct checkbox *);
};

CORE_BEGIN_DECLS

/**
 * Initialize the theming system.
 *
 * This function is automatically called from \ref ui_init and thus not
 * necessary from user.
 *
 * \return False on error.
 */
bool
theme_init(void);

/**
 * Get a reference to the default theme.
 *
 * The returned theme may be modified to modify the whole UI.
 *
 * \return A non-owning pointer to a static storage for the default theme.
 */
struct theme *
theme_default(void);

/**
 * Convenient shortcut to shallow copy src into dst.
 *
 * Use this function when you want your own local copy of a theme because you
 * want to modify some attributes.
 *
 * You should not modify underlying objects within the new theme because they
 * still point to the same region and you may erase the user settings.
 *
 * This code is incorrect:
 *
 * ```c
 * struct theme th;
 *
 * theme_shallow(&th, theme_default());
 * th.fonts[THEME_FONT_INTERFACE].style = FONT_STYLE_NONE;
 *
 * //
 * // Since th.fonts contain same pointers to theme_default, you'll erase
 * // the default theme settings.
 * //
 * ```
 *
 * Instead, if you really need to modify an underlying object, you have to copy
 * it too.
 *
 * ```c
 * struct font font;
 *
 * font_shallow(&font, theme_default()->fonts[THEME_FONT_INTERFACE];
 * font.style = FONT_STYLE_NONE;
 *
 * // No font_finish needed either, it is only a shallow copy.
 * ```
 *
 * This is a shortcut to `memcpy(dst, src, sizeof (*src))`.
 *
 * \pre dst != NULL
 * \param dst the destination theme
 * \param src the source theme (may be NULL)
 */
void
theme_shallow(struct theme *dst, const struct theme *src);

/**
 * Draw a frame.
 *
 * \pre frame != NULL
 * \param t the theme to use (may be NULL)
 * \param frame the frame
 */
void
theme_draw_frame(const struct theme *t, const struct frame *frame);

/**
 * Draw a label.
 *
 * \pre label != NULL
 * \param t the theme to use (may be NULL)
 * \param label the label
 */
void
theme_draw_label(const struct theme *t, const struct label *label);

/**
 * Draw a button.
 *
 * \pre button != NULL
 * \param t the theme to use (may be NULL)
 * \param button the button
 */
void
theme_draw_button(const struct theme *t, const struct button *button);

/**
 * Draw a checkbox.
 *
 * \param t the theme to use (may be NULL)
 * \param cb the checkbox
 */
void
theme_draw_checkbox(const struct theme *t, const struct checkbox *cb);

/**
 * This function is automatically called from \ref ui_finish and thus not
 * necessary from user.
 */
void
theme_finish(void);

CORE_END_DECLS

#endif /* !MOLKO_UI_THEME_H */
