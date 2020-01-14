/*
 * util.h -- utilities
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

#ifndef MOLKO_UTIL_H
#define MOLKO_UTIL_H

#include <stddef.h>

/**
 * Wrapper around malloc(3) that exits on allocation failure.
 *
 * \param size the size
 * \return a pointer
 * \post returned pointer will never be NULL
 */
void *
emalloc(size_t size);

/**
 * Wrapper around calloc(3) that exits on allocation failure.
 *
 * \param n the number of objects to allocate
 * \param size the size per n
 * \return a pointer
 * \post returned pointer will never be NULL
 */
void *
ecalloc(size_t n, size_t size);

/**
 * Put the thread to sleep for a given amount of milliseconds.
 *
 * \param ms the number of milliseconds to wait
 */
void
delay(unsigned int ms);

#endif /* !MOLKO_UTIL_H */
