/*
 * trace.h -- non-fatal message logs
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

#include <assert.h>
#include <stdio.h>

#include "trace.h"

static void
default_handler(const char *line)
{
	assert(line);

	puts(line);
}

void (*mlk_trace_handler)(const char *) = default_handler;

void
mlk_tracef(const char *fmt, ...)
{
	assert(fmt);

	va_list ap;

	if (!mlk_trace_handler)
		return;

	va_start(ap, fmt);
	mlk_traceva(fmt, ap);
	va_end(ap);
}

void
mlk_traceva(const char *fmt, va_list ap)
{
	assert(fmt);

	char buf[TRACE_LINE_MAX];

	if (!mlk_trace_handler)
		return;

	vsnprintf(buf, sizeof (buf), fmt, ap);
	mlk_trace_handler(buf);
}
