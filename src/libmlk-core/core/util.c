/*
 * util.c -- utilities
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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
#include <limits.h>
#include <stdlib.h>

#include <SDL.h>

#include <port/port.h>

#include "util.h"

void
util_delay(unsigned int ms)
{
	SDL_Delay(ms);
}

const char *
util_pathf(const char *fmt, ...)
{
	static char path[PATH_MAX];
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(path, sizeof (path), fmt, ap);
	va_end(ap);

	return path;
}

unsigned int
util_nrand(unsigned int lower, unsigned int upper)
{
	assert(upper <= RAND_MAX);

	return (rand() % (upper - lower)) + lower;
}
