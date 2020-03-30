/*
 * util.c -- utilities
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

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>

#include "panic.h"
#include "util.h"

void *
emalloc(size_t size)
{
	void *mem;

	if (!(mem = malloc(size)))
		panicf("%s", strerror(errno));

	return mem;
}

void *
ecalloc(size_t n, size_t size)
{
	void *mem;

	if (!(mem = calloc(n, size)))
		panicf("%s", strerror(errno));

	return mem;
}

void *
ememdup(const void *ptr, size_t size)
{
	void *mem;

	if (!(mem = malloc(size)))
		panicf("%s", strerror(errno));

	return memcpy(mem, ptr, size);
}

char *
eprintf(const char *fmt, ...)
{
	assert(fmt);

	va_list ap;
	char *ret;

	va_start(ap, fmt);
	ret = evprintf(fmt, ap);
	va_end(ap);

	return ret;
}

char *
evprintf(const char *fmt, va_list args)
{
	assert(fmt);

	va_list ap;
	int size;
	char *ret;

	/* Count number of bytes required. */
	va_copy(ap, args);

	if ((size = vsnprintf(NULL, 0, fmt, ap)) < 0)
		panicf("%s", strerror(errno));

	/* Do actual copy. */
	ret = emalloc(size + 1);
	va_copy(ap, args);

	if (vsnprintf(ret, size, fmt, ap) != size) {
		free(ret);
		panicf("%s", strerror(errno));
	}

	return ret;
}

void
delay(unsigned int ms)
{
	SDL_Delay(ms);
}
