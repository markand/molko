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
struct mlk_frame_delegate;
struct mlk_style;

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
	struct mlk_style *style;

	/**
	 * (read-write, borrowed)
	 *
	 * Frame delegate.
	 */
	struct mlk_frame_delegate *delegate;
};

/**
 * \struct mlk_frame_delegate
 * \brief Frame delegate.
 */
struct mlk_frame_delegate {
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
	 * \param self this delegate
	 * \param frame the frame to update
	 * \param ticks number of ticks since last frame
	 */
	void (*update)(struct mlk_frame_delegate *self,
	               struct mlk_frame *frame,
	               unsigned int ticks);

	/**
	 * (read-write, optional)
	 *
	 * Draw this label.
	 *
	 * \param self this delegate
	 * \param frame the frame to update
	 */
	void (*draw)(struct mlk_frame_delegate *self,
	             const struct mlk_frame *frame);

	/**
	 * (read-write, optional)
	 *
	 * Cleanup this delegate associated with the label.
	 *
	 * \param self this delegate
	 * \param frame the underlying frame
	 */
	void (*finish)(struct mlk_frame_delegate *self,
	               struct mlk_frame *frame);
};

/**
 * \brief Default stateless delegate for frame.
 */
extern struct mlk_frame_delegate mlk_frame_delegate;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Initialize the frame with default values.
 *
 * This is not required if you use designated initializers.
 *
 * \pre frame != NULL
 * \param frame the frame to default initialize
 * \param st style to use (or NULL to use a default)
 * \param dt delegate to use (or NULL to use a default)
 */
void
mlk_frame_init(struct mlk_frame *frame,
               struct mlk_style *st,
               struct mlk_frame_delegate *dt);

/**
 * Invoke ::mlk_frame_delegate::update.
 */
void
mlk_frame_update(struct mlk_frame *frame, unsigned int ticks);

/**
 * Invoke ::mlk_frame_delegate::draw.
 */
void
mlk_frame_draw(const struct mlk_frame *frame);

/**
 * Invoke ::mlk_frame_delegate::finish.
 */
void
mlk_frame_finish(struct mlk_frame *frame);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_UI_FRAME_H */
