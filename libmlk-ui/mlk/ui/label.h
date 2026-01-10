/*
 * label.h -- GUI label
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

#ifndef MLK_UI_LABEL_H
#define MLK_UI_LABEL_H

/**
 * \file label.h
 * \brief GUI label.
 */

struct mlk_font;
struct mlk_label;
struct mlk_label_style;

/**
 * \struct mlk_label
 * \brief UI label text.
 */
struct mlk_label {
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
	 * (read-write, borrowed)
	 *
	 * Text to show.
	 */
	const char *text;

	/**
	 * (read-write, borrowed, optional)
	 *
	 * Style to use for drawing this label.
	 */
	struct mlk_label_style *style;
};

/**
 * \struct mlk_label_style
 * \brief Label style.
 */
struct mlk_label_style {
	/**
	 * (read-write)
	 *
	 * Text color.
	 */
	unsigned long color;

	/**
	 * (read-write, borrowed, optional)
	 *
	 * Font to use.
	 */
	struct mlk_font *font;

	/**
	 * Check the required size for this label.
	 *
	 * \param self this style
	 * \param label the label to query dimensions
	 * \param w destination width (can be NULL)
	 * \param h destination height (can be NULL)
	 */
	int (*query)(struct mlk_label_style *self,
	             struct mlk_label *label,
	             unsigned int *w,
	             unsigned int *h);

	/**
	 * (read-write, optional)
	 *
	 * Update this label.
	 *
	 * \param self this style
	 * \param label the label to update
	 * \param ticks frame ticks
	 */
	void (*update)(struct mlk_label_style *self, struct mlk_label *label, unsigned int ticks);

	/**
	 * (read-write, optional)
	 *
	 * Draw the label.
	 *
	 * \param self this style
	 * \param label the label to draw
	 */
	void (*draw)(struct mlk_label_style *self, struct mlk_label *label);
};

/**
 * \brief Dark default style for label.
 */
extern struct mlk_label_style mlk_label_style_dark;

/**
 * \brief Light default style for label.
 */
extern struct mlk_label_style mlk_label_style_light;

/**
 * \brief Default style for all labels.
 */
extern struct mlk_label_style *mlk_label_style;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Invoke ::mlk_label_style::query.
 */
int
mlk_label_query(struct mlk_label *label, unsigned int *w, unsigned int *h);

/**
 * Invoke ::mlk_label_style::update.
 */
void
mlk_label_update(struct mlk_label *label, unsigned int ticks);

/**
 * Invoke ::mlk_label_style::draw.
 */
void
mlk_label_draw(struct mlk_label *label);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_UI_LABEL_H */
