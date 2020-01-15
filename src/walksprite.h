/*
 * walksprite.h -- sprite designed for walking entities
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

#ifndef MOLKO_WALKSPRITE_H
#define MOLKO_WALKSPRITE_H

/**
 * \file walksprite.h
 * \brief Sprite designed for walking entities.
 */

#include <stdint.h>

struct sprite;

/**
 * \brief Sprite designed for walking entities.
 *
 * This structure works with sprite images that are defined as using the
 * following conventions:
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
 */
struct walksprite {
	struct sprite *sprite;  /*!< (RW) The sprite to use */
	uint16_t delay;         /*!< (RW) The delay between frames */
	uint8_t index;          /*!< (RO) Current column index */
	uint16_t elapsed;       /*!< (RO) Elapsed time since last frame */
};

/**
 * Initialize the walking sprite.
 *
 * \pre ws != NULL
 * \pre sprite != NULL
 * \param ws the walking sprite
 * \param sprite the sprite to use
 * \param delay the delay between sprites
 */
void
walksprite_init(struct walksprite *ws, struct sprite *sprite, uint16_t delay);

/**
 * Update the walking sprite
 *
 * \pre ws != NULL
 * \param ws the walking sprite
 * \param ticks the number of milliseconds between last frame
 */
void
walksprite_update(struct walksprite *ws, unsigned ticks);

/**
 * Draw the appropriate sprite cell to the screen according to the orientation
 * given.
 *
 * \pre ws != NULL
 * \pre orientation < 8
 * \param ws the walking sprite
 * \param orientation the orientation (or the row between 0 and 7)
 * \param x the x coordinate
 * \param y the y coordinate
 */
void
walksprite_draw(struct walksprite *ws, uint8_t orientation, int x, int y);

#endif /* !MOLKO_WALKSPRITE_H */
