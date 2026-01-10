/*
 * alloc.h -- memory allocators
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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
 * ### Example: allocating three ints
 *
 * Use the ::mlk_alloc_new function to allocate three ints with uninitialized
 * memory.
 *
 * ```c
 * int *ptr = mlk_alloc_new(3, sizeof (int));
 *
 * ptr[0] = 10;
 * ptr[1] = 20;
 * ptr[2] = 30;
 * ```
 *
 * This generates the following blocks in memory.
 *
 * ```
 * +-----------------+----------------+------------------------+
 * | What            | Value          | Description            |
 * +-----------------+----------------+------------------------+
 * | size_t          | 3              | Three `int` requested  |
 * | size_t          | sizeof (int)   | Size of one `int`      |
 * | unsigned char * | undefined      | Content of user memory |
 * +-----------------+----------------+------------------------+
 * ```
 *
 * \warning The returned address is the `unsigned char *` so the user must
 *          never free this address directly.
 *
 * If you use ::mlk_alloc_resize, the storage will be expanded/shrinked
 * automatically without the need to pass the actual size of the array.
 *
 * ```c
 * ptr = mlk_alloc_renew(ptr, 6);
 *
 * // Now ptr holds 6 ints.
 * //
 * // ptr[0], ptr[1], ptr[2] are kept unchanged.
 * // ptr[3-5] are uninitialized.
 * ```
 *
 * The block now looks like this:
 *
 * ```
 * +-----------------+----------------+------------------------+
 * | What            | Value          | Description            |
 * +-----------------+----------------+------------------------+
 * | size_t          | 6              | Six `int` requested    |
 * | size_t          | sizeof (int)   | Size of one `int`      |
 * | unsigned char * | undefined      | Content of user memory |
 * +-----------------+----------------+------------------------+
 * ```
 *
 * Finally, free the result using ::mlk_alloc_free, DON'T free the pointer
 * directly.
 *
 * ```c
 * mlk_alloc_free(ptr);
 * ```
 *
 * ## Custom allocators
 *
 * The whole library uses allocators defined in this module when it needs to
 * allocate data. By default, C standard `malloc`, `realloc` and `free` are
 * used but they call ::mlk_panic in case of memory exhaustion.
 *
 * Those functions can be overridden through the ::mlk_alloc_set function with a
 * set of functions defined in the structure ::mlk_alloc_funcs.
 *
 * When using custom allocators, it is recommended to call ::mlk_alloc_set as
 * early as possible because it will also set internal libraries to use those.
 *
 * [fam]: https://en.wikipedia.org/wiki/Flexible_array_member
 */

#include <stddef.h>

/**
 * \struct mlk_alloc_funcs
 * \brief Custom allocator
 */
struct mlk_alloc_funcs {
	/**
	 * (read-write)
	 *
	 * Allocate the number of bytes uninitialized.
	 *
	 * \param size the number of bytes to allocate (always > 0)
	 * \return a pointer to the allocated memory or NULL on error
	 */
	void *(*alloc)(size_t size);

	/**
	 * (read-write)
	 *
	 * Realloc the pointer with the new size.
	 *
	 * \param ptr the pointer to reallocate (never NULL)
	 * \param size the new size (always > 0)
	 * \return a pointer to the allocated memory or NULL on error
	 */
	void *(*realloc)(void *, size_t);

	/**
	 * (read-write)
	 *
	 * Free the pointer memory.
	 *
	 * \param ptr the pointer to release
	 */
	void (*free)(void *);
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Replace the memory allocators.
 *
 * The parameter funcs must be kept alive until the program ends or it is
 * replaced with a new value.
 *
 * \param funcs the new allocator or NULL to re-use the defaults
 */
void
mlk_alloc_set(const struct mlk_alloc_funcs *funcs);

/**
 * Allocates a block memory.
 *
 * \param n the number of elements to allocate
 * \param w the size of each individual element
 * \return whatever the allocator returned
 */
void *
mlk_alloc_new(size_t n, size_t w);

/**
 * \copydoc mlk_alloc_new
 *
 * Similar to ::mlk_alloc_new but also ensure the data is zero initialized.
 */
void *
mlk_alloc_new0(size_t n, size_t w);

/**
 * Reallocate a block memory.
 *
 * \note In contrast to standard `realloc` function, it is NOT possible to pass
 *       a NULL pointer in this function because it needs to know the initial
 *       block memory size.
 *
 * \pre ptr != NULL
 * \pre n > 0
 * \param ptr the pointer to reallocate
 * \param n the new size
 * \return whatever the allocator returned to rearrange the pointer memory
 */
void *
mlk_alloc_resize(void *ptr, size_t n);

/**
 * \copydoc mlk_alloc_resize
 *
 * Similar to ::mlk_alloc_resize but also ensure expanded data is zero
 * initialized.
 */
void *
mlk_alloc_resize0(void *ptr, size_t n);

/**
 * Similar to ::mlk_alloc_resize but expands the storage by n.
 *
 * Example, if the current pointer holds 10 `int`, passing 90 as n argument
 * will expand the memory to a total of 100 `int`.
 *
 * \pre ptr != NULL
 * \param ptr the pointer to expand
 * \param n the number of items to append
 * \return whatever the allocator returned to rearrange the pointer memory
 */
void *
mlk_alloc_expand(void *ptr, size_t n);

/**
 * \copydoc mlk_alloc_expand
 *
 * Similar to ::mlk_alloc_expand but also ensure expanded data is zero
 * initialized.
 */
void *
mlk_alloc_expand0(void *ptr, size_t n);

/**
 * Duplicate a memory block.
 *
 * \pre ptr != NULL
 * \param ptr the pointer to copy
 * \param n the number of elements to allocate
 * \param w the size of each individual element
 * \return whatever the allocator returned
 */
void *
mlk_alloc_dup(const void *ptr, size_t n, size_t w);

/**
 * Duplicate a string.
 *
 * \pre src != NULL
 * \param src the string to duplicate
 * \return whatever the allocator returned
 */
char *
mlk_alloc_sdup(const char *src);

/**
 * Duplicate a string using a printf(3) format.
 *
 * \pre fmt != NULL
 * \param fmt the printf(3) format string
 * \return whatever the allocator returned
 */
char *
mlk_alloc_sdupf(const char *fmt, ...);

/**
 * Get the number of items in ptr.
 *
 * \pre ptr != NULL
 * \param ptr the pointer block
 * \return how many number of elements ptr holds
 */
size_t
mlk_alloc_getn(const void *ptr);

/**
 * Get the element width in ptr.
 *
 * \pre ptr != NULL
 * \param ptr the pointer block
 * \return size of individual elements ptr holds
 */
size_t
mlk_alloc_getw(const void *ptr);

/**
 * Free resources specified by pointer.
 *
 * If pointer is null, function is no-op.
 *
 * \param ptr the memory to release (maybe NULL)
 */
void
mlk_alloc_free(void *ptr);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_ALLOC_H */
