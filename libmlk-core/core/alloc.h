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

#ifndef MOLKO_CORE_ALLOC_H
#define MOLKO_CORE_ALLOC_H

#include <stdbool.h>
#include <stddef.h>

#include "util.h"

/* Must be power of 2. */
#define ALLOC_POOL_INIT_DEFAULT (32)

/* Custom allocator. */
struct alloc_funcs {
	void *(*alloc)(size_t size);
	void *(*realloc)(void *ptr, size_t size);
	void (*free)(void *ptr);
};

/* Minimalist growable array for loading data. */
struct alloc_pool {
	void *data;
	size_t elemsize;
	size_t size;
	size_t capacity;
	void (*finalizer)(void *data);
};

/* allocator functions. */
void
alloc_set(const struct alloc_funcs *funcs);

void *
alloc_new(size_t size);

void *
alloc_new0(size_t size);

void *
alloc_array(size_t len, size_t elemsize);

void *
alloc_array0(size_t len, size_t elemsize);

void *
alloc_renew(void *ptr, size_t size);

void *
alloc_rearray(void *ptr, size_t newlen, size_t elemsize);

void *
alloc_rearray0(void *ptr, size_t oldlen, size_t newlen, size_t elemsize);

void *
alloc_dup(const void *ptr, size_t size);

char *
alloc_sdup(const char *src);

/* alloc_pool functions. */
bool
alloc_pool_init(struct alloc_pool *pool, size_t elemsize, void (*finalizer)(void *));

void *
alloc_pool_new(struct alloc_pool *pool);

void *
alloc_pool_get(const struct alloc_pool *pool, size_t index);

void
alloc_pool_finish(struct alloc_pool *pool);

#endif /* !MOLKO_CORE_ALLOC_H */
