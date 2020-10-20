/*
 * error.h -- error routines
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

#ifndef MOLKO_ERROR_H
#define MOLKO_ERROR_H

/**
 * \file error.h
 * \brief Error routines.
 * \ingroup basics
 */

#include <stdarg.h>
#include <stdbool.h>

#include "plat.h"

/**
 * Get the last error returned.
 *
 * \return The error string.
 */
const char *
error(void);

/**
 * Set the game error with a printf-like format.
 *
 * \pre fmt != NULL
 * \param fmt the format string
 * \return Always false.
 */
bool
errorf(const char *fmt, ...) PLAT_PRINTF(1, 2);

/**
 * Similar to \ref errorf.
 *
 * \pre fmt != NULL
 * \param fmt the format stinrg
 * \param ap the variadic arguments pointer
 * \return Always false.
 */
bool
verrorf(const char *fmt, va_list ap) PLAT_PRINTF(1, 0);

/**
 * Convenient helper that sets last error from global C errno and then return
 * false.
 *
 * \return Always false.
 */
bool
error_errno(void);

#endif /* !MOLKO_ERROR_H */
