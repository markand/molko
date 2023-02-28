/*
 * alloc.h -- custom allocators
 *
 * Copyright (c) 2020-2023 David Demelier <markand@malikania.fr>
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
#include "panic.h"

#define OOM_MSG "out of memory"
#define BLKSIZE (offsetof (struct block, data))

struct block {
	size_t n;
	size_t w;
	unsigned char data[];
};

static void *
panic_alloc(size_t size)
{
	void *mem;

	if (!(mem = malloc(size)))
		mlk_panicf(OOM_MSG);

	return mem;
}

static void *
panic_realloc(void *ptr, size_t size)
{
	void *mem;

	if (!(mem = realloc(ptr, size)) && size != 0)
		mlk_panicf(OOM_MSG);

	return mem;
}

static const struct mlk_alloc_funcs *funcs = &(const struct mlk_alloc_funcs) {
	.alloc = panic_alloc,
	.realloc = panic_realloc,
	.free = free
};

static void *
wrap_sdl_malloc(size_t s)
{
	return funcs->alloc(s);
}

static void *
wrap_sdl_calloc(size_t n, size_t w)
{
	const size_t s = n * w;

	return memset(funcs->alloc(s), 0, s);
}

static void *
wrap_sdl_realloc(void *mem, size_t s)
{
	return funcs->realloc(mem, s);
}

static void
wrap_sdl_free(void *mem)
{
	funcs->free(mem);
}

void
mlk_alloc_set(const struct mlk_alloc_funcs *newfuncs)
{
	assert(funcs);
	assert(funcs->alloc);
	assert(funcs->realloc);
	assert(funcs->free);

	funcs = newfuncs;

	/* Change SDL allocators as well. */
	SDL_SetMemoryFunctions(wrap_sdl_malloc, wrap_sdl_calloc,
	    wrap_sdl_realloc, wrap_sdl_free);
}

static inline struct block *
blockat(void *ptr)
{
	unsigned char *addr = (unsigned char *)ptr;

	return (struct block *)&addr[-BLKSIZE];
}

static inline void *
allocate(size_t n, size_t w, int zero)
{
	struct block *b;
	size_t s = n * w;

	// TODO: overflow check.

	b = funcs->alloc(BLKSIZE + s);
	b->n = n;
	b->w = w;

	if (zero)
		memset(b->data, 0, s);

	return b->data;
}

static inline void *
reallocate(void *ptr, size_t n, int zero)
{
	assert(ptr);

	struct block *b = blockat(ptr);
	size_t osize = b->n * b->w;
	size_t nsize = (b->n + n) * b->w;

#if !defined(NDEBUG)
	assert(SIZE_MAX - osize >= nsize);
#endif

	b = funcs->realloc(b, BLKSIZE + nsize);

	if (zero && nsize > osize)
		memset(b->data + osize, 0, nsize - osize);

	return b->data;
}

void *
mlk_alloc_new(size_t n, size_t w)
{
	assert(n);
	assert(w);

	return allocate(n, w, 0);
}

void *
mlk_alloc_new0(size_t n, size_t w)
{
	assert(n);
	assert(w);

	return allocate(n, w, 1);
}

void *
mlk_alloc_renew(void *ptr, size_t n)
{
	return reallocate(ptr, n, 0);
}

void *
mlk_alloc_renew0(void *ptr, size_t n)
{
	return reallocate(ptr, n, 1);
}

void *
mlk_alloc_dup(const void *ptr, size_t n, size_t w)
{
	assert(ptr);
	assert(n);
	assert(w);

	return memcpy(mlk_alloc_new(n, w), ptr, n * w);
}

char *
mlk_alloc_sdup(const char *src)
{
	assert(src);

	size_t len = strlen(src) + 1;

	return memcpy(mlk_alloc_new(len, 1), src, len);
}

char *
mlk_alloc_sdupf(const char *fmt, ...)
{
	va_list ap;
	char *str;
	int size;

	va_start(ap, fmt);
	size = vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);

	if (size <= 0)
		return NULL;

	str = mlk_alloc_new(size + 1, 1);
	va_start(ap, fmt);
	vsnprintf(str, size + 1, fmt, ap);
	va_end(ap);

	return str;
}

void
mlk_alloc_free(void *ptr)
{
	if (ptr)
		funcs->free(blockat(ptr));
}

void
mlk_alloc_pool_init(struct mlk_alloc_pool *pool,
                    size_t poolsize,
                    size_t elemsize,
                    void (*finalizer)(void *))
{
	assert(pool);
	assert(poolsize > 0 && (poolsize & (poolsize - 1)) == 0);
	assert(elemsize != 0);

	pool->data = mlk_alloc_new(poolsize, elemsize);
	pool->elemsize = elemsize;
	pool->size = 0;
	pool->capacity = poolsize;
	pool->finalizer = finalizer;
}

void *
mlk_alloc_pool_new(struct mlk_alloc_pool *pool)
{
	assert(pool);

	if (pool->size >= pool->capacity) {
		pool->capacity *= 2;
		pool->data = mlk_alloc_renew(pool->data, pool->capacity);
	}

	return ((unsigned char *)pool->data) + pool->size++ * pool->elemsize;
}

void *
mlk_alloc_pool_get(const struct mlk_alloc_pool *pool, size_t index)
{
	assert(pool);
	assert(index < pool->size);

	return ((unsigned char *)pool->data) + index * pool->elemsize;
}

void *
mlk_alloc_pool_shrink(struct mlk_alloc_pool *pool)
{
	assert(pool);

	void *ptr;

	ptr = mlk_alloc_renew(pool->data, pool->size);
	memset(pool, 0, sizeof (*pool));

	return ptr;
}

void
mlk_alloc_pool_finish(struct mlk_alloc_pool *pool)
{
	unsigned char *tab;

	if (!pool)
		return;

	if (pool->finalizer) {
		tab = pool->data;

		for (size_t i = 0; i < pool->size; ++i)
			pool->finalizer(tab + i * pool->elemsize);
	}

	mlk_alloc_free(pool->data);
	memset(pool, 0, sizeof (*pool));
}
