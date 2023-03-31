/*
 * walksprite.h -- sprite designed for walking entities
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

#ifndef MLK_RPG_WALKSPRITE_H
#define MLK_RPG_WALKSPRITE_H

/**
 * \file mlk/rpg/walksprite.h
 * \brief Sprite designed for walking entities
 *
 * This module provides a convenient animation for walking entities using a
 * specific sprite texture.
 *
 * The entity can have up to 8 orientation including diagonals which are defined
 * as following:
 *
 * ```
 * 7   0   1
 *   ↖ ↑ ↗
 * 6 ←   → 2
 *   ↙ ↓ ↘
 * 5   4   3
 * ```
 *
 * Where numbers define row in the sprite according to the character
 * orientation. In other terms, your image sprite should look like this:
 *
 * ```
 * row columns in your image
 * ---|---------------------
 *  0 | ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
 *  1 | ↗↗↗↗↗↗↗↗↗↗↗↗↗↗↗↗↗↗↗↗
 *  2 | →→→→→→→→→→→→→→→→→→→→
 *  3 | ↘↘↘↘↘↘↘↘↘↘↘↘↘↘↘↘↘↘↘↘
 *  4 | ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
 *  5 | ↙↙↙↙↙↙↙↙↙↙↙↙↙↙↙↙↙↙↙↙
 *  6 | ←←←←←←←←←←←←←←←←←←←←
 *  7 | ↖↖↖↖↖↖↖↖↖↖↖↖↖↖↖↖↖↖↖↖
 * ```
 *
 * The first column in each row should be static position which means that the
 * entity is not moving. Using ::mlk_walksprite_reset resets the column index to
 * this position so that you can use this module even on non-moving entities.
 */

struct mlk_sprite;

/**
 * \struct mlk_walksprite
 * \brief Walking sprite structure
 */
struct mlk_walksprite {
	/**
	 * (read-write, borrowed)
	 *
	 * Sprite file to use for animation.
	 */
	struct mlk_sprite *sprite;

	/**
	 * (read-write)
	 *
	 * Delay between each frame.
	 */
	unsigned int delay;

	/** \cond MLK_PRIVATE_DECLS */
	unsigned int index;
	unsigned int elapsed;
	/** \endcond MLK_PRIVATE_DECLS */
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Initialize the walking sprite.
 *
 * \pre ws != NULL
 * \param ws walking sprite to initialize
 */
void
mlk_walksprite_init(struct mlk_walksprite *ws);

/**
 * Reset the walking sprite to first column.
 *
 * \pre ws != NULL
 * \param ws walking sprite to reset
 */
void
mlk_walksprite_reset(struct mlk_walksprite *ws);

/**
 * Update the walking sprite, moving to the next animation if needed.
 *
 * \pre ws != NULL
 * \param ws walking sprite to update
 * \param ticks frame ticks
 */
void
mlk_walksprite_update(struct mlk_walksprite *ws, unsigned int ticks);

/**
 * Draw the current image frame at the given position.
 *
 * \pre ws != NULL
 * \param ws walking sprite to draw
 * \param orientation the orientation [0..7]
 * \param x the x coordinate
 * \param y the y coordinate
 */
void
mlk_walksprite_draw(const struct mlk_walksprite *ws,
                    unsigned int orientation,
                    int x,
                    int y);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_RPG_WALKSPRITE_H */
