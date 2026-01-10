/*
 * panic.c -- unrecoverable error handling
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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
#include <stdlib.h>

#include "core_p.h"
#include "err.h"
#include "panic.h"

static void
terminate(const char *what)
{
	fprintf(stderr, _("abort: %s\n"), what);
	abort();
	exit(1);
}

void (*mlk_panic_handler)(const char *) = terminate;

static void
alive(void)
{
	/*
	 * This should not happen, if it does it means the user did not fully
	 * satisfied the constraint of mlk_panic_handler.
	 */
	fprintf(stderr, _("abort: panic handler returned\n"));
	exit(1);
}

void
mlk_panicf(const char *fmt, ...)
{
	assert(fmt);

	va_list ap;
	char buf[PANIC_LINE_MAX];

	/*
	 * We can't use mlk_panicva directly because we won't be able to call
	 * va_end then.
	 */
	va_start(ap, fmt);
	vsnprintf(buf, sizeof (buf), fmt, ap);
	va_end(ap);

	mlk_panic_handler(buf);
	alive();
}

void
mlk_panicva(const char *fmt, va_list ap)
{
	assert(fmt);
	assert(mlk_panic_handler);

	char buf[PANIC_LINE_MAX];

	vsnprintf(buf, sizeof (buf), fmt, ap);
	mlk_panic_handler(buf);
	alive();
}

void
mlk_panic(void)
{
	mlk_panicf("%s", mlk_err());
}
