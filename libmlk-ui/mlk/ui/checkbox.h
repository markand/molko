/*
 * checkbox.h -- GUI checkbox
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

#ifndef MLK_UI_CHECKBOX_H
#define MLK_UI_CHECKBOX_H

/**
 * \file mlk/ui/checkbox.h
 * \brief GUI checkbox.
 */

struct mlk_checkbox;
struct mlk_checkbox_style;

union mlk_event;

/**
 * \struct mlk_checkbox
 * \brief UI clickable checkbox.
 */
struct mlk_checkbox {
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
	 * Checkbox width.
	 */
	unsigned int w;

	/**
	 * (read-write)
	 *
	 * Checkbox height.
	 */
	unsigned int h;

	/**
	 * (read-write)
	 *
	 * Checkbox status, non-zero if activated.
	 */
	int checked;

	/**
	 * (read-write, borrowed, optional)
	 *
	 * Style to use for drawing this checkbox.
	 */
	struct mlk_checkbox_style *style;
};

/**
 * \struct mlk_checkbox_style
 * \brief Checkbox style.
 */
struct mlk_checkbox_style {
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
	 * (read-write, optional)
	 *
	 * Update the checkbox.
	 *
	 * \param self this style
	 * \param cb the checkbox to update
	 * \param ticks number of ticks since last frame
	 */
	void (*update)(struct mlk_checkbox_style *self,
	               struct mlk_checkbox *cb,
	               unsigned int ticks);

	/**
	 * (read-write, optional)
	 *
	 * Draw this checkbox.
	 *
	 * \param self this style
	 * \param cb the checkbox to update
	 */
	void (*draw)(struct mlk_checkbox_style *self,
	             struct mlk_checkbox *cb);
};

/**
 * \brief Dark default style for check.
 */
extern struct mlk_checkbox_style mlk_checkbox_style_dark;

/**
 * \brief Light default style for checkbox.
 */
extern struct mlk_checkbox_style mlk_checkbox_style_light;

/**
 * \brief Default style for all checkboxes.
 */
extern struct mlk_checkbox_style *mlk_checkbox_style;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Invoke ::mlk_checkbox_style::update.
 */
int
mlk_checkbox_handle(struct mlk_checkbox *cb, const union mlk_event *ev);

/**
 * Invoke ::mlk_checkbox_style::update.
 */
void
mlk_checkbox_update(struct mlk_checkbox *cb, unsigned int ticks);

/**
 * Invoke ::mlk_checkbox_style::draw.
 */
void
mlk_checkbox_draw(struct mlk_checkbox *cb);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_UI_CHECKBOX_H */
