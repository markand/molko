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
 * \brief Automatic drawable objects.
 *
 * This module is analogous to ::action but does not handle event and has a
 * position on the screen.
 */

#include <assert.h>

/**
 * \struct mlk_drawable
 * \brief Drawable structure
 */
struct mlk_drawable {
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
	 * Restart the drawable.
	 *
	 * This function should be used when the drawable can be reset in its
	 * original form, e.g. an animation from start.
	 */
	void (*start)(struct mlk_drawable *self);

	/**
	 * (optional)
	 *
	 * Update the drawable with the given ticks since last frame.
	 *
	 * The callback should return true if it is considered complete.
	 *
	 * \param ticks frame ticks
	 * \return true if drawable completed
	 */
	int (*update)(struct mlk_drawable *self, unsigned int ticks);

	/**
	 * (optional)
	 *
	 * Draw the drawable.
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
	 * This function is always called the least meaning that user is able
	 * to free the pointer if dynamically allocated.
	 */
	void (*finish)(struct mlk_drawable *self);
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Invoke ::mlk_drawable::start function if not null.
 */
static inline void
mlk_drawable_start(struct mlk_drawable *drawable)
{
	assert(drawable);

	if (drawable->start)
		drawable->start(drawable);
}

/**
 * Invoke ::mlk_drawable::update function if not null.
 */
static inline int
mlk_drawable_update(struct mlk_drawable *drawable, unsigned int ticks)
{
	assert(drawable);

	if (drawable->update)
		return drawable->update(drawable, ticks);

	return 0;
}

/**
 * Invoke ::mlk_drawable::draw function if not null.
 */
static inline void
mlk_drawable_draw(struct mlk_drawable *drawable)
{
	assert(drawable);

	if (drawable->draw)
		drawable->draw(drawable);
}

/**
 * Invoke ::mlk_drawable::end function if not null.
 */
static inline void
mlk_drawable_end(struct mlk_drawable *drawable)
{
	assert(drawable);

	if (drawable->end)
		drawable->end(drawable);
}

/**
 * Invoke ::mlk_drawable::finish function if not null.
 */
static inline void
mlk_drawable_finish(struct mlk_drawable *drawable)
{
	assert(drawable);

	if (drawable->finish)
		drawable->finish(drawable);
}

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_DRAWABLE_H */
