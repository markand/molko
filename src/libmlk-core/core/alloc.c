/*
 * alloc.h -- custom allocators
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
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>

#include "alloc.h"
#include "buf.h"
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

static const struct alloc_funcs default_alloc_funcs = {
	.alloc = panic_alloc,
	.realloc = panic_realloc,
	.free = free
};

static const struct alloc_funcs *funcs = &default_alloc_funcs;

void
alloc_set(const struct alloc_funcs *newfuncs)
{
	assert(funcs);
	assert(funcs->alloc);
	assert(funcs->realloc);
	assert(funcs->free);

	funcs = newfuncs;

	/* Change SDL allocators as well. */
	SDL_SetMemoryFunctions(alloc_new, alloc_array0, alloc_renew, free);
}

void *
alloc_new(size_t size)
{
	assert(size != 0);

	return funcs->alloc(size);
}

void *
alloc_new0(size_t size)
{
	assert(size != 0);

	return memset(funcs->alloc(size), 0, size);
}

void *
alloc_array(size_t len, size_t elemsize)
{
	assert(len != 0);
	assert(elemsize != 0);

	return funcs->alloc(len * elemsize);
}

void *
alloc_array0(size_t len, size_t elemsize)
{
	assert(len != 0);
	assert(elemsize != 0);

	return alloc_new0(len * elemsize);
}

void *
alloc_renew(void *ptr, size_t size)
{
	return funcs->realloc(ptr, size);
}

void *
alloc_rearray(void *ptr, size_t len, size_t elemsize)
{
	assert(elemsize != 0);

	return funcs->realloc(ptr, len * elemsize);
}

void *
alloc_rearray0(void *ptr, size_t oldlen, size_t newlen, size_t elemsize)
{
	ptr = funcs->realloc(ptr, newlen * elemsize);

	if (newlen > oldlen)
		memset((unsigned char *)ptr + (oldlen * elemsize), 0, (newlen - oldlen) * elemsize);

	return ptr;
}

void *
alloc_dup(const void *ptr, size_t size)
{
	assert(ptr);
	assert(size != 0);

	return memcpy(funcs->alloc(size), ptr, size);
}

char *
alloc_sdup(const char *src)
{
	assert(src);

	size_t len = strlen(src) + 1;

	return memcpy(funcs->alloc(len), src, len);
}

char *
alloc_sdupf(const char *fmt, ...)
{
	struct buf buf = {0};
	va_list ap;

	va_start(ap, fmt);
	buf_vprintf(&buf, fmt, ap);
	va_end(ap);

	return buf.data;
}

void
alloc_free(void *ptr)
{
	funcs->free(ptr);
}

void
alloc_pool_init(struct alloc_pool *pool, size_t elemsize, void (*finalizer)(void *))
{
	assert(pool);
	assert(elemsize != 0);

	pool->data = alloc_array(ALLOC_POOL_INIT_DEFAULT, elemsize);
	pool->elemsize = elemsize;
	pool->size = 0;
	pool->capacity = ALLOC_POOL_INIT_DEFAULT;
	pool->finalizer = finalizer;
}

void *
alloc_pool_new(struct alloc_pool *pool)
{
	assert(pool);

	if (pool->size >= pool->capacity) {
		pool->capacity *= 2;
		pool->data = alloc_rearray(pool->data, pool->capacity, pool->elemsize);
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

void *
alloc_pool_shrink(struct alloc_pool *pool)
{
	assert(pool);

	void *ptr;

	ptr = alloc_rearray(pool->data, pool->size, pool->elemsize);
	memset(pool, 0, sizeof (*pool));

	return ptr;
}

void
alloc_pool_finish(struct alloc_pool *pool)
{
	unsigned char *tab;

	if (!pool)
		return;

	if (pool->finalizer) {
		tab = pool->data;

		for (size_t i = 0; i < pool->size; ++i)
			pool->finalizer(tab + i * pool->elemsize);
	}

	free(pool->data);
	memset(pool, 0, sizeof (*pool));
}
