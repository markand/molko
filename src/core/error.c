/*
 * error.c -- error routines
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "error_p.h"

#include <SDL.h>

static char buffer[2048];

const char *
error(void)
{
	return buffer;
}

bool
error_errno(void)
{
	error_printf("%s", strerror(errno));

	return false;
}

bool
error_printf(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	error_vprintf(fmt, ap);
	va_end(ap);

	return false;
}

bool
error_vprintf(const char *fmt, va_list ap)
{
	vsnprintf(buffer, sizeof (buffer), fmt, ap);

	return false;
}

noreturn void
error_fatal(void)
{
	fprintf(stderr, "%s\n", buffer);
	exit(1);
}

noreturn void
error_fatalf(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	error_vfatalf(fmt, ap);
	va_end(ap);
}

noreturn void
error_vfatalf(const char *fmt, va_list ap)
{
	fprintf(stderr, fmt, ap);
	exit(1);
}

/* private: error_p.h */

bool
error_sdl(void)
{
	error_printf("%s", SDL_GetError());

	return false;
}
