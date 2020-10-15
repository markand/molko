/*
 * animation.h -- basic animations
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

#ifndef MOLKO_ANIMATION_H
#define MOLKO_ANIMATION_H

/**
 * \file animation.h
 * \brief Basic animations.
 * \ingroup drawing
 */

#include <stdbool.h>

struct drawable;
struct sprite;

/**
 * \brief Animation object
 */
struct animation {
	struct sprite *sprite;  /*!< (+&) Sprite to use. */
	unsigned int row;       /*!< (-) Current row. */
	unsigned int column;    /*!< (-) Current column. */
	unsigned int delay;     /*!< (-) Delay between frames. */
	unsigned int elapsed;   /*!< (-) Elapsed time since last frame. */
};

/**
 * Initialize the animation.
 *
 * The animation does not take sprite ownership, it must be valid until
 * animation is no longer used.
 *
 * \pre an != NULL
 * \pre sprite != NULL
 * \param an the animation
 * \param sprite the sprite to use
 * \param delay the delay between frames in milliseconds
 */
void
animation_init(struct animation *an, struct sprite *sprite, unsigned int delay);

/**
 * Start an animation.
 *
 * \pre an != NULL
 * \param an the animation
 */
void
animation_start(struct animation *an);

/**
 * Update the animation.
 *
 * You must call this function at each loop iteration to update the animation
 * frame depending on its delay.
 *
 * \pre an != NULL
 * \param an the animation
 * \param ticks the elapsed ticks since the last call
 * \return true if the animation is complete
 */
bool
animation_update(struct animation *an, unsigned int ticks);

/**
 * Draw the animation.
 *
 * \pre an != NULL
 * \param an the animation
 * \param x the X coordinate
 * \param y the Y coordinate
 */
void
animation_draw(struct animation *an, int x, int y);

/**
 * Create a drawable from an animation.
 *
 * The animation must be kept alive until the drawable is used.
 *
 * The dw->data member will be set to the animation pointer and can be
 * safely used by the user for custom drawable operation if needed.
 *
 * The dw->finish member isn't used and can be re-used by the user.
 *
 * \pre an != NULL
 * \pre dw != NULL
 * \param an the animation
 * \param dw the drawable
 * \param x x position on screen
 * \param y y position on screen
 * \post dw->data contains an
 * \post dw->update contains an update function
 * \post dw->draw contains a drawing function
 * \post dw->finish is NULL
 */
void
animation_drawable(struct animation *an, struct drawable *dw, int x, int y);

#endif /* !MOLKO_ANIMATION_H */
