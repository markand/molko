/*
 * panic.h -- unrecoverable error handling
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

#ifndef MLK_CORE_PANIC_H
#define MLK_CORE_PANIC_H

/**
 * \file panic.h
 * \brief Unrecoverable error handling
 *
 * This module is intended to be used whenever an unrecoverable error happens
 * and the action is left to the developer.
 *
 * Most of the API don't call panic directly.
 */

#include <stdarg.h>

/**
 * Maximum trace line
 */
#define PANIC_LINE_MAX (128)

/**
 * Default panic handler.
 *
 * The default handler prints the line and exit using C [abort].
 *
 * [abort]: https://en.cppreference.com/w/c/program/abort
 *
 * \param line the line to print
 */
extern void (*mlk_panic_handler)(const char *line);

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Panic the game with using a [printf] format style message
 *
 * [printf]: https://en.cppreference.com/w/c/io/fprintf
 *
 * \pre fmt != NULL
 * \param fmt the printf format string
 */
void
mlk_panicf(const char *, ...);

/**
 * Similar to ::mlk_panicf but using a `va_list` instead.
 *
 * \pre fmt != NULL
 * \param fmt the printf format string
 * \param ap the variadic handle
 */
void
mlk_panicva(const char *, va_list);

/**
 * Similar to ::mlk_panicf but uses ::mlk_err to retrieve the last error as the
 * final message.
 */
void
mlk_panic(void);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_PANIC_H */
