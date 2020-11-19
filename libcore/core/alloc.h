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

#ifndef MOLKO_ALLOC_H
#define MOLKO_ALLOC_H

/**
 * \file alloc.h
 * \brief Custom allocators.
 *
 * This module controls how the API should allocate memory. Although dynamic
 * allocation isn't much used in the core API, it is used in few places where
 * static arrays would not fit (e.g. loading maps).
 *
 * To change the allocator, simply modify the global allocator object.
 */

#include <stdbool.h>
#include <stddef.h>

#include "util.h"

/**
 * \brief Default size to allocate in struct alloc_pool.
 * \warning Must be a power of 2.
 */
#define ALLOC_POOL_INIT_DEFAULT 32

/**
 * \brief Global allocator strategy.
 */
struct allocator {
	/**
	 * (+) Allocate some data.
	 *
	 * The default implementation uses malloc and calls \ref panic in case
	 * of failure.
	 *
	 * \pre size != 0
	 * \param size the size to alloc
	 * \return A pointer to the allocated region (NULL is allowed).
	 */
	void *(*alloc)(size_t size);

	/**
	 * (+) Realloc a region.
	 *
	 * The default implementation uses malloc and calls \ref panic in case
	 * of failure.
	 *
	 * \pre size != 0
	 * \param ptr the old region
	 * \param size the new size
	 * \return A pointer to the allocated region (NULL is allowed).
	 */
	void *(*realloc)(void *ptr, size_t size);

	/**
	 * (+) Free resources.
	 *
	 * The default implementation calls standard C free function.
	 *
	 * \param ptr the region (may be NULL)
	 */
	void (*free)(void *ptr);
};

/**
 * \brief Pool allocator.
 *
 * This small structure is a helper to reallocate data each time a new slot is
 * requested. It allocates twice as the current storage when size exceeds
 * capacity.
 *
 * It uses realloc mechanism to upgrade the new storage space so pointers
 * returned must not be referenced directly.
 *
 * It is designed in mind to help allocating resources locally that may be
 * referenced in another module without having to manage an array from the user
 * code. Because it is designed for this responsability only it only supports
 * insertion.
 *
 * The initial capacity is controlled by the ALLOC_POOL_INIT_DEFAULT macro and
 * **must** be a power of two.
 *
 * A custom finalizer function can be set to finalize each individual object if
 * necessary.
 */
struct alloc_pool {
	void *data;             /*!< (+?) Pointer to the region. */
	size_t elemsize;        /*!< (-) Size of individual element. */
	size_t size;            /*!< (-) Number of items in array. */
	size_t capacity;        /*!< (-) Current capacity. */

	/**
	 * (+?) Optional finalizer.
	 *
	 * This function will be invoked for every element when \ref
	 * alloc_pool_finish is called.
	 *
	 * \param data the element to finalize
	 */
	void (*finalizer)(void *data);
};

/**
 * \brief Global allocator object.
 */
extern struct allocator allocator;

/**
 * Shortcut for allocator->alloc.
 *
 * \pre size != 0
 * \param size the amount of bytes to allocate
 * \return The result of allocator->alloc.
 */
void *
alloc_new(size_t size);

/**
 * Similar to alloc_new but zero-initialize the memory.
 *
 * \pre size != 0
 * \param size the amount of bytes to allocate
 * \return The result of allocator->alloc.
 */
void *
alloc_new0(size_t size);

/**
 * Shortcut for allocator->alloc but specialized for arrays.
 *
 * \pre n != 0 && size != 0
 * \param n the number of objects to allocate
 * \param size the size of each object
 * \return The result of allocator->alloc.
 */
void *
alloc_array(size_t n, size_t size);

/**
 * Similar to alloc_array but zero-initialize the memory.
 *
 * \pre n != 0 && size != 0
 * \param n the number of objects to allocate
 * \param size the size of each object
 * \return The result of allocator->alloc.
 */
void *
alloc_array0(size_t n, size_t size);

/**
 * Shortcut for allocator->realloc.
 *
 * \param ptr the old pointer (maybe NULL)
 * \param amount the new amount (maybe 0)
 * \return The result of allocator->realloc
 */
void *
alloc_renew(void *ptr, size_t amount);

/**
 * Shortcut for allocator->realloc but specialized for arrays.
 *
 * \param ptr the old pointer (maybe NULL)
 * \param n the number of element
 * \param size the size of each object
 * \return The result of allocator->realloc
 */
void *
alloc_rearray(void *ptr, size_t n, size_t size);

/**
 * Duplicate region pointer by ptr.
 *
 * This function calls \ref alloc_new to allocate memory.
 *
 * \pre ptr != NULL
 * \param ptr the pointer
 * \param size the size of the memory to copy
 * \return The result of allocator->alloc filled with a copy of ptr.
 */
void *
alloc_dup(const void *ptr, size_t size);

/**
 * Initialize the pool.
 *
 * This will effectively create a initial storage according to
 * ALLOC_POOL_INIT_DEFAULT.
 *
 * This function effectively use the global allocator object to initialize the
 * array.
 *
 * \pre pool != NULL
 * \pre elemsize != 0
 * \param pool the pool to initialize
 * \param elemsize size of each individual element
 * \param finalizer a finalizer to use when clearing the pool.
 * \return True if allocation suceedeed.
 */
bool
alloc_pool_init(struct alloc_pool *pool, size_t elemsize, void (*finalizer)(void *));

/**
 * Request a new slot from the pool.
 *
 * If the current size has reached the capacity, it will be doubled in that case
 * it is possible that all previous pointer may be invalidated.
 *
 * This function effectively use the global allocator object to realloc the
 * array.
 *
 * \pre pool != NULL
 * \param pool the pool
 * \return The pointer to the region.
 */
void *
alloc_pool_new(struct alloc_pool *pool);

/**
 * Finalize the pool and all individual element if a finalizer is set.
 *
 * You must call \ref alloc_pool_init again before reusing it.
 *
 * \pre pool != NULL
 * \param pool the pool to clear
 */
void
alloc_pool_finish(struct alloc_pool *pool);

#endif /* !MOLKO_ALLOC_H */
