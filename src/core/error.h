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
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stdnoreturn.h>

/**
 * Get the last error returned.
 *
 * \return the error string
 */
const char *
error(void);

/**
 * Convenient handler that sets last error from global C errno and then return
 * false.
 *
 * \return false
 */
bool
error_errno(void);

/**
 * Set the game error with a printf-like format.
 *
 * \param fmt the format string
 * \return false
 */
bool
error_printf(const char *fmt, ...);

/**
 * Similar to \a error_printf.
 *
 * \param fmt the format stinrg
 * \param ap the variadic arguments pointer
 * \return false
 */
bool
error_vprintf(const char *fmt, va_list ap);

/**
 * Print last registered error and exit with code 1.
 */
noreturn void
error_fatal(void);

/**
 * Prints an error to stderr and exit.
 *
 * \param fmt the format string
 */
noreturn void
error_fatalf(const char *fmt, ...);

/**
 * Similar to \a error_fatalf
 *
 * \param fmt the format string
 * \param ap the variadic arguments pointer
 */
noreturn void
error_vfatalf(const char *fmt, va_list ap);

#endif /* !MOLKO_ERROR_H */
