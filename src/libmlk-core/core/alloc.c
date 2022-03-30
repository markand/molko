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

	void *ptr;

	if ((ptr = funcs->alloc(size)))
		memset(ptr, 0, size);

	return ptr;
}

void *
alloc_array(size_t len, size_t elemsize)
{
	assert(len != 0);
	assert(elemsize != 0);

	size_t size = len * elemsize;

	if (size / len != elemsize)
		return errorf("%s", strerror(ENOMEM)), NULL;

	return funcs->alloc(size);
}

void *
alloc_array0(size_t len, size_t elemsize)
{
	assert(len != 0);
	assert(elemsize != 0);

	void *mem;
	size_t size = len * elemsize;

	if (size / len != elemsize)
		return errorf("%s", strerror(ENOMEM)), NULL;

	if ((mem = funcs->alloc(size)))
		memset(mem, 0, size);

	return mem;
}

void *
alloc_renew(void *ptr, size_t size)
{
	return funcs->realloc(ptr, size);
}

void *
alloc_rearray(void *ptr, size_t len, size_t elemsize)
{
	size_t size = len * elemsize;

	if (size / len != elemsize)
		return errorf("%s", strerror(ENOMEM)), NULL;

	return funcs->realloc(ptr, size);
}

void *
alloc_rearray0(void *ptr, size_t oldlen, size_t newlen, size_t elemsize)
{
	size_t size = newlen * elemsize;

	if (size / newlen != elemsize)
		return errorf("%s", strerror(ENOMEM)), NULL;
	if (!(ptr = funcs->realloc(ptr, size)))
		return NULL;

	if (newlen > oldlen)
		memset((unsigned char *)ptr + (oldlen * elemsize), 0, (newlen - oldlen) * elemsize);

	return ptr;
}

void *
alloc_dup(const void *ptr, size_t size)
{
	assert(ptr);
	assert(size != 0);

	void *mem;

	if ((mem = funcs->alloc(size)))
		memcpy(mem, ptr, size);

	return mem;
}

char *
alloc_sdup(const char *src)
{
	assert(src);

	char *ret;
	size_t length = strlen(src) + 1;

	if ((ret = funcs->alloc(length)))
		memcpy(ret, src, length + 1);

	return ret;
}

char *
alloc_sdupf(const char *fmt, ...)
{
	struct buf buf = {0};
	char *ret;
	va_list ap;

	va_start(ap, fmt);
	buf_vprintf(&buf, fmt, ap);
	va_end(ap);

	if (!buf.data)
		panicf("%s", strerror(ENOMEM));

	/*
	 * We need to reallocate a copy because the API expects to use
	 * alloc_free.
	 */
	ret = alloc_dup(buf.data, buf.length + 1);
	buf_finish(&buf);

	return ret;
}

int
alloc_pool_init(struct alloc_pool *pool, size_t elemsize, void (*finalizer)(void *))
{
	assert(pool);
	assert(elemsize != 0);

	if (!(pool->data = alloc_array(ALLOC_POOL_INIT_DEFAULT, elemsize)))
		return -1;

	pool->elemsize = elemsize;
	pool->size = 0;
	pool->capacity = ALLOC_POOL_INIT_DEFAULT;
	pool->finalizer = finalizer;

	return 0;
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
