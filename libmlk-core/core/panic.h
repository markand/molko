/*
 * panic.h -- unrecoverable error handling
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

#ifndef MOLKO_CORE_PANIC_H
#define MOLKO_CORE_PANIC_H

/**
 * \file panic.h
 * \brief Unrecoverable error handling.
 * \ingroup basics
 *
 * This set of functions should be used to detect runtime errors that are
 * unexpected. They should be used only when the game cannot continue because
 * it is in a unrecoverable state.
 *
 * Examples of appropriate use cases:
 *
 * - Game saved data is corrupt,
 * - Assets are missing,
 * - No more memory.
 *
 * In other contexts, use asserts to indicates programming error and
 * appropriate solutions to recover the game otherwise.
 */

#include <stdarg.h>

/**
 * \brief Global panic handler.
 *
 * The default implementation shows the last error and exit with code 1. The
 * function must not return so you have to implement a setjmp/longjmp or a
 * exception to be thrown.
 *
 * If the user defined function returns, panic routines will finally exit with
 * code 1.
 */
extern void (*panic_handler)(void);

/**
 * Terminate the program using the \ref panic_handler routine.
 *
 * This function will first set the global error with the provided format
 * string and then call the handler.
 *
 * \pre fmt != NULL
 * \param fmt the printf(3) format string
 */
void
panicf(const char *fmt, ...);

/**
 * Similar to \ref panicf but with a arguments pointer.
 *
 * \pre fmt != NULL
 * \param fmt the printf(3) format string
 * \param ap the arguments pointer
 */
void
vpanicf(const char *fmt, va_list ap);

/**
 * Similar to \ref panicf but use last error stored using \ref error.h
 * routines.
 */
void
panic(void);

#endif /* !MOLKO_CORE_PANIC_H */
