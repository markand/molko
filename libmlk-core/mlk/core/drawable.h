/*
 * drawable.h -- automatic drawable objects
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

#ifndef MLK_CORE_DRAWABLE_H
#define MLK_CORE_DRAWABLE_H

/**
 * \file mlk/core/drawable.h
 * \brief Automatic drawable objects
 */

/**
 * \struct mlk_drawable
 * \brief Drawable structure
 */
struct mlk_drawable {
	/**
	 * (read-write, borrowed, optional)
	 *
	 * Arbitrary user data.
	 */
	void *data;

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
	 * (optional)
	 *
	 * Update the drawable with the given ticks since last frame.
	 *
	 * The callback should return non-zero if it is considered complete.
	 *
	 * \param self this drawable
	 * \param ticks frame ticks
	 * \return non-zero if complete
	 */
	int (*update)(struct mlk_drawable *self, unsigned int ticks);

	/**
	 * (optional)
	 *
	 * Draw the drawable.
	 *
	 * \param self this drawable
	 */
	void (*draw)(struct mlk_drawable *self);

	/**
	 * (optional)
	 *
	 * Terminate the drawable.
	 *
	 * In contrast to finish, this function should be called after the
	 * drawable was considered complete.
	 *
	 * \param self this drawable
	 */
	void (*end)(struct mlk_drawable *self);

	/**
	 * (optional)
	 *
	 * Dispose resources allocated by/for the drawable.
	 *
	 * \param self this drawable
	 */
	void (*finish)(struct mlk_drawable *self);
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Invoke ::mlk_drawable::update function if not null.
 *
 * \pre drawable != NULL
 * \param drawable the drawable
 * \param ticks frame ticks
 */
int
mlk_drawable_update(struct mlk_drawable *drawable, unsigned int ticks);

/**
 * Invoke ::mlk_drawable::draw function if not null.
 *
 * \pre drawable != NULL
 * \param drawable the drawable
 */
void
mlk_drawable_draw(struct mlk_drawable *drawable);

/**
 * Invoke ::mlk_drawable::end function if not null.
 *
 * \pre drawable != NULL
 * \param drawable the drawable
 */
void
mlk_drawable_end(struct mlk_drawable *drawable);

/**
 * Invoke ::mlk_drawable::finish function if not null.
 *
 * \pre drawable != NULL
 * \param drawable the drawable
 */
void
mlk_drawable_finish(struct mlk_drawable *drawable);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_DRAWABLE_H */
