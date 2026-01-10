/*
 * clock.h -- track elapsed time
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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

#ifndef MLK_CORE_CLOCK_H
#define MLK_CORE_CLOCK_H

/**
 * \file mlk/core/clock.h
 * \brief Track elapsed time
 *
 * This simple module provide functions to keep track of elapsed time.
 *
 * It is mostly used in the game loop because most objects in the overall API
 * get the frame ticks instead.
 */

/**
 * \struct mlk_clock
 * \brief Clock structure
 *
 * This structure is non-opaque but has no public fields.
 */
struct mlk_clock {
	/** \cond MLK_PRIVATE_DECLS */
	unsigned int ticks;
	/** \endcond MLK_PRIVATE_DECLS */
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Start or reset the clock.
 *
 * \pre clock != NULL
 * \param clock the clock timer
 */
void
mlk_clock_start(struct mlk_clock *clock);

/**
 * Returns the number of elapsed milliseconds since last call to
 * ::mlk_clock_start.
 *
 * \pre clock != NULL
 * \param clock the clock timer
 * \return the number of elapsed milliseconds
 */
unsigned int
mlk_clock_elapsed(const struct mlk_clock *clock);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_CLOCK_H */
