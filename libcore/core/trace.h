/*
 * trace.h -- non-fatal message logs
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

#ifndef MOLKO_TRACE_H
#define MOLKO_TRACE_H

/**
 * \file trace.h
 * \brief Non-fatal message logs.
 *
 * The purpose of this module is to provide a feedback from the code when there
 * are non-fatal programming error or unexpected results. In contrast to the
 * \ref debug.h module this one is always activated no manner if the build
 * is in Debug or Release.
 *
 * For example, having an animation with a delay of 0 is not a technical issue
 * but is probably not what the use wants. Thus, a trace warning may be
 * generated in that way.
 */

#include <stdarg.h>

/**
 * \brief Maximum length for a trace log.
 */
#define TRACE_LINE_MAX (1024)

/**
 * \brief Global trace handler.
 *
 * The default one use a simple printf on the standard output.
 */
extern void (*trace_handler)(const char *);

/**
 * Log some information.
 *
 * \pre fmt != NULL
 * \param fmt the printf(3) format string
 */
void
tracef(const char *fmt, ...);

/**
 * Similar to \ref tracef with a va_list arguments pointer.
 *
 * \pre fmt != NULL
 * \param fmt the printf(3) format string
 * \param ap the argument list
 */
void
vtracef(const char *fmt, va_list ap);

#endif /* !MOLKO_TRACE_H */
