/*
 * animation.h -- basic animations
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

#ifndef MLK_CORE_ANIMATION_H
#define MLK_CORE_ANIMATION_H

/**
 * \file mlk/core/animation.h
 * \brief Basic animations
 *
 * Drawable animations.
 *
 * Animations are small objects using a ::mlk_sprite to update themselves and
 * draw next frames depending on delay set.
 */

struct mlk_sprite;

/**
 * \struct mlk_animation
 * \brief Animation structure
 */
struct mlk_animation {
	/**
	 * (read-write, borrowed)
	 *
	 * The sprite to draw.
	 */
	const struct mlk_sprite *sprite;

	/**
	 * (read-write)
	 *
	 * Delay in milliseconds between each frame.
	 */
	unsigned int delay;

	/** \cond MLK_PRIVATE_DECLS */
	unsigned int elapsed;
	unsigned int row;
	unsigned int column;
	/** \endcond MLK_PRIVATE_DECLS */
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Start or reset the animation to the beginning.
 *
 * \pre animation != NULL
 * \param animation the animation
 */
void
mlk_animation_start(struct mlk_animation *animation);

/**
 * Tells if the animation is complete.
 *
 * \pre animation != NULL
 * \param animation the animation
 * \return non-zero if completed
 */
int
mlk_animation_completed(const struct mlk_animation *animation);

/**
 * Update the animation.
 *
 * This function MUST not be called if the animation is complete.
 *
 * \pre animation != NULL and animation is not complete
 * \param animation the animation
 * \param ticks frame ticks
 * \return non-zero if completed
 */
int
mlk_animation_update(struct mlk_animation *animation, unsigned int ticks);

/**
 * Draw the animation at the given position
 *
 * This function MUST not be called if the animation is complete.
 *
 * \pre animation != NULL and animation is not complete
 * \param animation the animation
 * \param x the x coordinate
 * \param y the y coordinate
 * \return 0 on success or any error propagated from ::mlk_sprite_draw.
 */
int
mlk_animation_draw(const struct mlk_animation *animation, int x, int y);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_ANIMATION_H */
