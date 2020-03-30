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

#include "plat.h"

/**
 * Get the number of elements in a static array.
 *
 * \param x the array
 * \return the number of elements
 */
#define NELEM(x) sizeof ((x)) / sizeof ((x)[0])

/**
 * Wrapper around malloc(3) that exits on allocation failure.
 *
 * \param size the size
 * \return a pointer
 * \post returned pointer will never be NULL
 */
void *
emalloc(size_t size);

/**
 * Wrapper around calloc(3) that exits on allocation failure.
 *
 * \param n the number of objects to allocate
 * \param size the size per n
 * \return a pointer
 * \post returned pointer will never be NULL
 */
void *
ecalloc(size_t n, size_t size);

/**
 * Copy the region specified by ptr.
 *
 * \pre ptr != NULL
 * \param ptr the pointer
 * \param size the size of the memory to copy
 * \return a pointer
 * \post returned pointer will never be NULL
 */
void *
ememdup(const void *ptr, size_t size);

/**
 * Create a dynamically allocated string in the printf(3) format string.
 *
 * \pre fmt != NULL
 * \return The heap allocated string.
 * \post Returned string will never be NULL.
 */
char *
eprintf(const char *fmt, ...) PLAT_PRINTF(1, 2);

/**
 * Similar to \ref eprintf with arguments pointer.
 *
 * \copydoc eprintf
 */
char *
evprintf(const char *fmt, va_list ap) PLAT_PRINTF(1, 0);

/**
 * Put the thread to sleep for a given amount of milliseconds.
 *
 * \param ms the number of milliseconds to wait
 */
void
delay(unsigned int ms);

#endif /* !MOLKO_UTIL_H */
