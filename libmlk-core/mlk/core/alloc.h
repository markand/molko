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

#ifndef MLK_CORE_ALLOC_H
#define MLK_CORE_ALLOC_H

#include <stddef.h>

#include "core.h"

/* Custom allocator. */
struct mlk_alloc_funcs {
	void *(*alloc)(size_t);
	void *(*realloc)(void *, size_t);
	void (*free)(void *);
};

/* Minimalist growable array for loading data. */
struct mlk_alloc_pool {
	void *data;
	size_t elemsize;
	size_t size;
	size_t capacity;
	void (*finalizer)(void *);
};

CORE_BEGIN_DECLS

/* allocator functions. */
void
mlk_alloc_set(const struct mlk_alloc_funcs *);

void *
mlk_alloc_new(size_t, size_t);

void *
mlk_alloc_new0(size_t, size_t);

void *
mlk_alloc_renew(void *, size_t);

void *
mlk_alloc_renew0(void *, size_t);

void *
mlk_alloc_dup(const void *, size_t, size_t);

char *
mlk_alloc_sdup(const char *);

char *
mlk_alloc_sdupf(const char *, ...);

void
mlk_alloc_free(void *);

/* alloc_pool functions. */
void
mlk_alloc_pool_init(struct mlk_alloc_pool *, size_t, size_t , void (*)(void *));

void *
mlk_alloc_pool_new(struct mlk_alloc_pool *);

void *
mlk_alloc_pool_get(const struct mlk_alloc_pool *, size_t);

void *
mlk_alloc_pool_shrink(struct mlk_alloc_pool *);

void
mlk_alloc_pool_finish(struct mlk_alloc_pool *);

CORE_END_DECLS

#endif /* !MLK_CORE_ALLOC_H */
