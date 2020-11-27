/*
 * util.h -- utilities
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

#ifndef MOLKO_UTIL_H
#define MOLKO_UTIL_H

/**
 * \file util.h
 * \brief Utilities.
 * \ingroup basics
 *
 * This file contains several utilities.
 *
 * \note In contrast to other files, identifiers are not prefixed with `util_`
 *       for convenience.
 */

#include <stdarg.h>
#include <stddef.h>

/**
 * Get the number of elements in a static array.
 *
 * \param x the array
 * \return the number of elements
 */
#define NELEM(x) sizeof ((x)) / sizeof ((x)[0])

/**
 * Put the thread to sleep for a given amount of milliseconds.
 *
 * \param ms the number of milliseconds to wait
 */
void
delay(unsigned int ms);

/**
 * Construct a temporary path to a file that can fit in a PATH_MAX array.
 *
 * This function is useful when specifying paths into a function invocation such
 * as `fopen(pprintf("%s.png", i), "r"))`.
 *
 * \pre fmt != NULL
 * \param fmt the format string
 * \warning This function is not reentrant, it returns a static storage path.
 * \return A non null path to a file.
 * \post Returned string is never NULL.
 */
const char *
pprintf(const char *fmt, ...);

/**
 * Generate a random number between lower and upper (included).
 *
 * \pre upper must be <= RAND_MAX
 * \param lower the lower bound
 * \param upper the upper bound
 * \return The generated number.
 */
unsigned int
nrand(unsigned int lower, unsigned int upper);

#endif /* !MOLKO_UTIL_H */
