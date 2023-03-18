/*
 * err.h -- error handing
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

#ifndef MLK_ERR_H
#define MLK_ERR_H

/**
 * \file mlk/core/err.h
 * \brief Error handling
 *
 * Because the Molko's Adventure framework use many different external
 * libraries, it is not an easy task to transform all external error codes to
 * a general one.
 *
 * Instead, the library stores a thread-local [errno]-like API that the user
 * can access as a string. When a function fails, the user should retrieve the
 * error as early as possible.
 *
 * [errno]: https://en.cppreference.com/w/c/error/errno
 */

#include <stdarg.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Set the error using printf format string.
 *
 * \pre fmt != NULL
 * \param fmt the format string
 * \return -1 for convenience
 */
int
mlk_errf(const char *fmt, ...);

/**
 * Similar to ::mlk_errf but using a `va_list` instead.
 *
 * \pre fmt != NULL
 * \param fmt the printf format string
 * \param ap the variadic handle
 * \return -1 for convenience
 */
int
mlk_errva(const char *fmt, va_list ap);

/**
 * Retrieve the last stored error as a string.
 *
 * \return the current error
 */
const char *
mlk_err(void);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_ERROR_H */
