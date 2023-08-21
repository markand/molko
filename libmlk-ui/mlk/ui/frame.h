/*
 * frame.h -- GUI frame
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

#ifndef MLK_UI_FRAME_H
#define MLK_UI_FRAME_H

/**
 * \file mlk/ui/frame.h
 * \brief GUI frame.
 */

struct mlk_frame;
struct mlk_frame_style;

struct mlk_frame {
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
	 * (read-write, borrowed)
	 *
	 * Frame style.
	 */
	struct mlk_frame_style *style;
};

/**
 * \struct mlk_frame_style
 * \brief Frame style.
 */
struct mlk_frame_style {
	unsigned long background;
	unsigned long border;
	unsigned int border_size;

	/*
	 * (read-write, borrowed, optional)
	 *
	 * Arbitrary user data.
	 */
	void *data;

	/**
	 * (read-write, optional)
	 *
	 * Update the label.
	 *
	 * \param self this style
	 * \param frame the frame to update
	 * \param ticks number of ticks since last frame
	 */
	void (*update)(struct mlk_frame_style *self,
	               struct mlk_frame *frame,
	               unsigned int ticks);

	/**
	 * (read-write, optional)
	 *
	 * Draw this frame.
	 *
	 * \param self this style
	 * \param frame the frame to draw
	 */
	void (*draw)(struct mlk_frame_style *self,
	             struct mlk_frame *frame);
};

/**
 * \brief Dark default style for frame.
 */
extern struct mlk_frame_style mlk_frame_style_dark;

/**
 * \brief Light default style for frame.
 */
extern struct mlk_frame_style mlk_frame_style_light;

/**
 * \brief Default style for all frames.
 */
extern struct mlk_frame_style *mlk_frame_style;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Invoke ::mlk_frame_style::update.
 */
void
mlk_frame_update(struct mlk_frame *frame, unsigned int ticks);

/**
 * Invoke ::mlk_frame_style::draw.
 */
void
mlk_frame_draw(struct mlk_frame *frame);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_UI_FRAME_H */
