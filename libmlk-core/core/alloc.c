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

	if (!(mem = realloc(ptr, size)) && size != 0)
		panicf("%s", strerror(errno));

	return mem;
}

struct allocator allocator = {
	.alloc = panic_alloc,
	.realloc = panic_realloc,
	.free = free
};

void *
alloc_new(size_t size)
{
	assert(size != 0);

	return allocator.alloc(size);
}

void *
alloc_new0(size_t size)
{
	assert(size != 0);

	void *ptr;

	if ((ptr = allocator.alloc(size)))
		memset(ptr, 0, size);

	return ptr;
}

void *
alloc_array(size_t n, size_t size)
{
	assert(n != 0);
	assert(size != 0);

	size_t total = n * size;

	if (total / n != size)
		return errorf("%s", strerror(ENOMEM)), NULL;

	return allocator.alloc(total);
}

void *
alloc_array0(size_t n, size_t size)
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
alloc_renew(void *ptr, size_t amount)
{
	return allocator.realloc(ptr, amount);
}

void *
alloc_rearray(void *ptr, size_t n, size_t size)
{
	size_t total = n * size;

	if (total / n != size)
		return errorf("%s", strerror(ENOMEM)), NULL;

	return allocator.realloc(ptr, total);
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

char *
alloc_sdup(const char *src)
{
	assert(src);

	char *ret;
	size_t length = strlen(src) + 1;

	if ((ret = allocator.alloc(length)))
		memcpy(ret, src, length + 1);

	return ret;
}

bool
alloc_pool_init(struct alloc_pool *pool, size_t elemsize, void (*finalizer)(void *))
{
	assert(pool);
	assert(elemsize != 0);

	if (!(pool->data = alloc_array(ALLOC_POOL_INIT_DEFAULT, elemsize)))
		return false;

	pool->elemsize = elemsize;
	pool->size = 0;
	pool->capacity = ALLOC_POOL_INIT_DEFAULT;
	pool->finalizer = finalizer;

	return true;
}

void *
alloc_pool_new(struct alloc_pool *pool)
{
	assert(pool);

	if (pool->size >= pool->capacity) {
		void *newptr = alloc_rearray(pool->data, pool->capacity * 2, pool->elemsize);

		if (!newptr)
			return NULL;

		pool->data = newptr;
		pool->capacity *= 2;
	}

	return ((unsigned char *)pool->data) + pool->size++ * pool->elemsize;
}

void *
alloc_pool_get(const struct alloc_pool *pool, size_t index)
{
	assert(pool);
	assert(index < pool->size);

	return ((unsigned char *)pool->data) + index * pool->elemsize;
}

void
alloc_pool_finish(struct alloc_pool *pool)
{
	if (pool->finalizer) {
		unsigned char *tab = pool->data;

		for (size_t i = 0; i < pool->size; ++i)
			pool->finalizer(tab + i * pool->elemsize);
	}

	free(pool->data);
	memset(pool, 0, sizeof (*pool));
}