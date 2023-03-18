/*
 * util.h -- libmlk-core utilities
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

#ifndef MLK_CORE_UTIL_H
#define MLK_CORE_UTIL_H

/**
 * \file mlk/core/util.h
 * \brief libmlk-core utilities
 */

/**
 * Compute the length of an fixed size array.
 *
 * \param x the fixed size array
 * \return the length
 */
#define MLK_UTIL_SIZE(x) (sizeof ((x)) / sizeof ((x)[0]))

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Suspend the calling thread for the specific duration.
 *
 * \param duration time to sleep in milliseconds
 */
void
mlk_util_sleep(unsigned int duration);

/**
 * Convenient function to create an return a string to a filesystem path using
 * a printf format string.
 *
 * \pre fmt != NULL
 * \param fmt the format string
 * \note The returned string is static thread-local and will be modified on
 *       subsequent calls.
 */
const char *
mlk_util_pathf(const char *fmt, ...);

/**
 * Compute a random number between [min-max).
 *
 * \param min the minimum range (included)
 * \param max the maximum range (excluded)
 * \return a random number
 */
unsigned int
mlk_util_nrand(unsigned int min, unsigned int max);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_UTIL_H */
