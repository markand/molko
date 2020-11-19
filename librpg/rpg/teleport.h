/*
 * teleport.h -- animate and teleport
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

#ifndef MOLKO_RPG_TELEPORT_H
#define MOLKO_RPG_TELEPORT_H

/**
 * \file teleport.h
 * \brief Animate screen and teleport.
 */

#include <stdbool.h>

#include <core/texture.h>

struct action;
struct map;
struct state;

/**
 * \brief Animate screen and teleport.
 *
 * This structure should be used as an action to block the current state input,
 * draw a fading effect and finally change the game state.
 */
struct teleport {
	struct state *state;    /*!< (+&) Next state to use. */
	unsigned int elapsed;   /*!< (-) Elapsed time since last frame. */
	unsigned int alpha;     /*!< (-) Current alpha */
	struct texture fade;    /*!< (-) Current fade level. */
};

/**
 * Start the teleport effect.
 *
 * This function will inhibit the current state input to avoid user interaction
 * while the effect is running.
 *
 * \pre tp != NULL
 * \param tp the teleport to start
 */
void
teleport_start(struct teleport *tp);

/**
 * Update the transition.
 *
 * \pre tp != NULL
 * \param tp the teleport effect
 * \param ticks the elapsed time since last frame
 * \return True if completed.
 */
bool
teleport_update(struct teleport *tp, unsigned int ticks);

/**
 * Draw the transition effect.
 *
 * \pre tp != NULL
 * \param tp the teleport effect
 */
void
teleport_draw(struct teleport *tp);

/**
 * Cleanup the teleport effect.
 *
 * \pre tp != NULL
 * \param tp the teleport effect to clear
 */
void
teleport_finish(struct teleport *tp);

/**
 * Transform the teleport effect into an action. This function will also call
 * teleport_start.
 *
 * \pre tp != NULL && tp->state != NULL
 * \pre act != NULL
 * \param tp the teleport object
 * \param act the action to fill
 * \post act->data contains tp
 * \post act->update will invoke teleport_update
 * \post act->draw will invoke teleport_draw
 * \post act->finish will invoke teleport_finish
 */
void
teleport_action(struct teleport *tp, struct action *act);

#endif /* !MOLKO_RPG_TELEPORT_H */
