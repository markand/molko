/*
 * alloc.h -- memory allocators
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

/**
 * \file mlk/core/alloc.h
 * \brief Memory allocators.
 *
 * This module provides function to dynamically allocate data on the heap.
 *
 * ## Synopsis
 *
 * Most of the Molko's Engine API does not allocate data except in very few
 * cases, otherwise this module is used each time it is required.
 *
 * ## Pointer block
 *
 * Data allocated by this module isn't a simple pointer to the region data but
 * a custom block data that holds the size of the element and the number of it.
 * This has been designed in the sense that reallocating data is easier for the
 * caller as it is not required to pass the data length along.
 *
 * An allocated blocks looks like this:
 *
 * | Type              | Description                 |
 * |-------------------|-----------------------------|
 * | `size_t`          | Number of items allocated   |
 * | `size_t`          | Size of individual elements |
 * | `unsigned char *` | User data                   |
 *
 * The structure is allocated using the [flexible array member][fam] to avoid
 * allocating data twice.
 *
 * Example:
 *
 * Allocating three ints with `int *ptr = mlk_alloc_new(3, sizeof (int))` will
 * create a block as following:
 *
 * | Type                      | Description                 |
 * |---------------------------|-----------------------------|
 * | `size_t` = 3              | Number of items allocated   |
 * | `size_t` = `sizeof (int)` | Size of individual elements |
 * | `unsigned char *`         | `int[3]`                    |
 *
 * Finally, calling `mlk_alloc_renew(ptr, 6)` will extend the memory for three
 * more ints.
 *
 * [fam]: https://en.wikipedia.org/wiki/Flexible_array_member
 */

#include <stddef.h>

#include "core.h"

struct mlk_alloc_funcs {
	void *(*alloc)(size_t);
	void *(*realloc)(void *, size_t);
	void (*free)(void *);
};

struct mlk_alloc_pool {
	void *data;
	size_t elemsize;
	size_t size;
	size_t capacity;
	void (*finalizer)(void *);
};

MLK_CORE_BEGIN_DECLS

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

MLK_CORE_END_DECLS

#endif /* !MLK_CORE_ALLOC_H */
