/*
 * button.h -- GUI button
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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

#ifndef MLK_UI_BUTTON_H
#define MLK_UI_BUTTON_H

/**
 * \file mlk/ui/button.h
 * \brief GUI button
 */

struct mlk_button;
struct mlk_button_style;
struct mlk_font;

union mlk_event;

/**
 * \struct mlk_button
 * \brief GUI button structure.
 */
struct mlk_button {
	/**
	 * (read-write)
	 *
	 * Position in x.
	 */
	int x;

	/**
	 * (read-write)
	 *
	 * Position in y.
	 */
	int y;

	/**
	 * (read-write)
	 *
	 * Frame width.
	 */
	unsigned int w;

	/**
	 * (read-write)
	 *
	 * Frame height.
	 */
	unsigned int h;

	/**
	 * (read-write, borrowed, optional)
	 *
	 * Button text.
	 */
	const char *text;

	/**
	 * (read-only)
	 *
	 * Indicate if the button is pressed.
	 *
	 * This value goes non-zero when the user clicks the button even if the
	 * mouse is not released which is different than considered active. For
	 * example, if the user clicks the button but release the mouse outside
	 * of the button region it is no longer considered active and no action
	 * should be performed.
	 */
	int pressed;

	/**
	 * (read-write, borrowed, optional)
	 *
	 * Style to use for drawing this button.
	 */
	struct mlk_button_style *style;
};

/**
 * \struct mlk_button_style
 * \brief Button style.
 */
struct mlk_button_style {
	/**
	 * (read-write)
	 *
	 * Background color.
	 */
	unsigned long background;

	/**
	 * (read-write)
	 *
	 * Border color.
	 */
	unsigned long border;

	/**
	 * (read-write)
	 *
	 * Border size.
	 */
	unsigned int border_size;

	/**
	 * (read-write)
	 *
	 * Text color.
	 */
	unsigned long color;

	/**
	 * (read-write, borrowed, optional)
	 *
	 * Text font.
	 */
	struct mlk_font *font;

	/**
	 * (read-write, optional)
	 *
	 * Update this button.
	 *
	 * \param self this style
	 * \param button the button to update
	 * \param ticks frame ticks
	 */
	void (*update)(struct mlk_button_style *style,
	               struct mlk_button *button,
	               unsigned int ticks);

	/**
	 * (read-write, optional)
	 *
	 * Draw bounding frame owning the button.
	 *
	 * \param self this style
	 * \param button the button
	 */
	void (*draw_frame)(struct mlk_button_style *style,
	                   struct mlk_button *button);

	/**
	 * (read-write, optional)
	 *
	 * Draw the butotn text.
	 *
	 * \param self this style
	 * \param button the button
	 */
	void (*draw_text)(struct mlk_button_style *style,
	                  struct mlk_button *button);
};

/**
 * \brief Dark default style for button.
 */
extern struct mlk_button_style mlk_button_style_dark;

/**
 * \brief Light default style for button.
 */
extern struct mlk_button_style mlk_button_style_light;

/**
 * \brief Default style for all buttons.
 */
extern struct mlk_button_style *mlk_button_style;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Handle event.
 *
 * \pre button != NULL
 * \pre ev != NULL
 * \param button the button
 * \param ev the event
 */
int
mlk_button_handle(struct mlk_button *button, const union mlk_event *ev);

/**
 * Update the button.
 *
 * \pre button != NULL
 * \param button the button update
 * \param ticks frame ticks
 */
void
mlk_button_update(struct mlk_button *button, unsigned int ticks);

/**
 * Draw the button.
 *
 * \pre button != NULL
 * \param button the button to draw
 */
void
mlk_button_draw(struct mlk_button *button);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_UI_BUTTON_H */
