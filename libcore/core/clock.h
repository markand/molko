/*
 * clock.h -- track elapsed time
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

#ifndef MOLKO_CLOCK_H
#define MOLKO_CLOCK_H

/**
 * \file clock.h
 * \brief Track elapsed time.
 * \ingroup basics
 */

/**
 * \brief Clock structure.
 */
struct clock {
	unsigned int ticks;     /*!< time point on initialization */
};

/**
 * Start the clock and track elapsed time.
 *
 * \pre clock != NULL
 * \param clock the clock
 */
void
clock_start(struct clock *clock);

/**
 * Tell the measured time.
 *
 * \pre clock != NULL
 * \param clock the clock
 * \return the elapsed time in milliseconds
 */
unsigned int
clock_elapsed(const struct clock *clock);

#endif /* !MOLKO_CLOCK_H */
