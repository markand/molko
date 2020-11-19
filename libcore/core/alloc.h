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

#include <stddef.h>

#include "util.h"

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
 * \brief Global allocator object.
 */
extern struct allocator allocator;

/**
 * Shortcut for allocator->alloc.
 *
 * \pre n != 0 && size != 0
 * \param n the number of objects to allocate
 * \param size the size of each object
 * \return The result of allocator->alloc.
 */
void *
alloc(size_t n, size_t size);

/**
 * Shortcut for allocator->alloc and then use memset to clear memory.
 *
 * \pre n != 0 && size != 0
 * \param n the number of objects to allocate
 * \param size the size of each object
 * \return The result of allocator->alloc.
 */
void *
alloc_zero(size_t n, size_t size);

/**
 * Duplicate region pointer by ptr.
 *
 * This function calls \ref alloc to allocate memory.
 *
 * \pre ptr != NULL
 * \param ptr the pointer
 * \param size the size of the memory to copy
 * \return The result of allocator->alloc filled with a copy of ptr.
 */
void *
alloc_dup(const void *ptr, size_t size);

#endif /* !MOLKO_ALLOC_H */
