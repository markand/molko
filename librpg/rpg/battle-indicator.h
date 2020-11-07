/*
 * battle-indicator.h -- drawable for rendering a hp/mp count usage
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

#ifndef MOLKO_BATTLE_INDICATOR_H
#define MOLKO_BATTLE_INDICATOR_H

/**
 * \file battle-indicator.h
 * \brief Drawable for rendering a hp/mp count usage.
 *
 * This small module creates an animation drawable that shows a label with a
 * smooth color/fading transition, used to display hp/mp damage.
 */

#include <stdbool.h>

#include <core/texture.h>

/**
 * \brief Default color for HP indicator.
 */
#define BATTLE_INDICATOR_HP_COLOR (0xa5303000)

/**
 * \brief Default color for MP indicator.
 */
#define BATTLE_INDICATOR_MP_COLOR (0xa23e8c00)

struct theme;

/**
 * \brief Structure context for indicator animation.
 *
 * You must zero-initialize this structure and fill the color and amount field
 * with the color target and the amount of damage to show respectively. Then use
 * start, update and draw function in order until it completes.
 */
struct battle_indicator {
	unsigned int color;             /*!< (+) Destination color. */
	unsigned int amount;            /*!< (+) Amount text to draw. */
	const struct theme *theme;      /*!< (+&?) Optional theme to use. */
	unsigned int cur;               /*!< (-) Current color target. */
	unsigned int elapsed;           /*!< (-) Elapsed time in transition. */
	unsigned int alpha;             /*!< (-) Current alpha. */
	struct texture tex[2];          /*!< (*) Rendered texture. */
};

/**
 * Start the battle indicator. You must call this function only once.
 *
 * \pre bti != NULL
 * \param bti the indicator
 */
void
battle_indicator_start(struct battle_indicator *bti);

/**
 * Tells if the indicator has completed.
 *
 * \pre battle_indicator_ok(bti)
 * \param bti the indicator to check
 * \return True if completed.
 */
bool
battle_indicator_completed(const struct battle_indicator *bti);

/**
 * Tells if the indicator is valid.
 *
 * \param bti the indicator to check (may be NULL)
 * \return True if correctly initialized.
 */
bool
battle_indicator_ok(const struct battle_indicator *bti);

/**
 * Update the indicator.
 *
 * \pre battle_indicator_ok(bti)
 * \param bti the indicator
 * \param ticks the elapsed milliseconds since last frame
 * \return True if completed.
 */
bool
battle_indicator_update(struct battle_indicator *bti, unsigned int ticks);

/**
 * Draw the indicator.
 *
 * \pre battle_indicator_ok(bti)
 * \param bti the indicator
 * \param x the x coordinate
 * \param y the y coordinate
 */
void
battle_indicator_draw(const struct battle_indicator *bti, int x, int y);

/**
 * Dispose resources.
 *
 * \pre battle_indicator_ok(bti)
 * \param bti the indicator
 */
void
battle_indicator_finish(struct battle_indicator *bti);

#endif /* !MOLKO_BATTLE_INDICATOR_H */
