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
		panic("%s\n", strerror(errno));

	return mem;
}

void *
ecalloc(size_t n, size_t size)
{
	void *mem;

	if (!(mem = calloc(n, size)))
		panic("%s\n", strerror(errno));

	return mem;
}

void *
ememdup(const void *ptr, size_t size)
{
	void *mem;

	if (!(mem = malloc(size)))
		panic("%s\n", strerror(errno));

	return memcpy(mem, ptr, size);
}

void
delay(unsigned int ms)
{
	SDL_Delay(ms);
}
