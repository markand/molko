/*
 * panic.c -- unrecoverable error handling
 *
 * Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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
#include "error.h"
#include "panic.h"

static void
terminate(void)
{
	fprintf(stderr, _("abort: %s\n"), error());
	abort();
	exit(1);
}

void (*panic_handler)(void) = terminate;

void
panicf(const char *fmt, ...)
{
	assert(fmt);

	va_list ap;

	/*
	 * Store the error before calling panic because va_end would not be
	 * called.
	 */
	va_start(ap, fmt);
	errorva(fmt, ap);
	va_end(ap);

	panic();
}

void
panicva(const char *fmt, va_list ap)
{
	assert(fmt);
	assert(panic_handler);

	errorva(fmt, ap);
	panic();
}

void
panic(void)
{
	assert(panic_handler);

	panic_handler();

	/*
	 * This should not happen, if it does it means the user did not fully
	 * satisfied the constraint of panic_handler.
	 */
	fprintf(stderr, _("abort: panic handler returned\n"));
	exit(1);
}
