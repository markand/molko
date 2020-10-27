/*
 * alloc.h -- custom allocators
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

#include "alloc.h"
#include "error.h"
#include "panic.h"

static void *
panic_alloc(size_t size)
{
	void *mem;

	if (!(mem = malloc(size)))
		panicf("%s", strerror(errno));

	return mem;
}

static void *
panic_realloc(void *ptr, size_t size)
{
	void *mem;

	if (!(mem = realloc(ptr, size)))
		panicf("%s", strerror(errno));

	return mem;
}

struct allocator allocator = {
	.alloc = panic_alloc,
	.realloc = panic_realloc,
	.free = free
};

void *
alloc(size_t n, size_t size)
{
	assert(n != 0);
	assert(size != 0);

	size_t total = n * size;

	if (total / n != size)
		return errorf("%s", strerror(ENOMEM)), NULL;

	return allocator.alloc(total);
}

void *
alloc_zero(size_t n, size_t size)
{
	assert(n != 0);
	assert(size != 0);

	void *mem;
	size_t total = n * size;

	if (total / n != size)
		return errorf("%s", strerror(ENOMEM)), NULL;

	if ((mem = allocator.alloc(total)))
		memset(mem, 0, total);

	return mem;
}

void *
alloc_dup(const void *ptr, size_t size)
{
	assert(ptr);
	assert(size != 0);

	void *mem;

	if ((mem = allocator.alloc(size)))
		memcpy(mem, ptr, size);

	return mem;
}
