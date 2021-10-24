/*
 * alloc.h -- custom allocators
 *
 * Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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

#ifndef MLK_CORE_ALLOC_H
#define MLK_CORE_ALLOC_H

#include <stddef.h>

#include "core.h"
#include "util.h"

/* Must be power of 2. */
#define ALLOC_POOL_INIT_DEFAULT (32)

/* Custom allocator. */
struct alloc_funcs {
	void *(*alloc)(size_t);
	void *(*realloc)(void *, size_t);
	void (*free)(void *);
};

/* Minimalist growable array for loading data. */
struct alloc_pool {
	void *data;
	size_t elemsize;
	size_t size;
	size_t capacity;
	void (*finalizer)(void *);
};

CORE_BEGIN_DECLS

/* allocator functions. */
void
alloc_set(const struct alloc_funcs *);

void *
alloc_new(size_t);

void *
alloc_new0(size_t);

void *
alloc_array(size_t, size_t);

void *
alloc_array0(size_t, size_t);

void *
alloc_renew(void *, size_t);

void *
alloc_rearray(void *, size_t, size_t);

void *
alloc_rearray0(void *, size_t, size_t, size_t);

void *
alloc_dup(const void *, size_t);

char *
alloc_sdup(const char *);

/* alloc_pool functions. */
int
alloc_pool_init(struct alloc_pool *, size_t , void (*)(void *));

void *
alloc_pool_new(struct alloc_pool *);

void *
alloc_pool_get(const struct alloc_pool *, size_t);

void
alloc_pool_finish(struct alloc_pool *);

CORE_END_DECLS

#endif /* !MLK_CORE_ALLOC_H */
