/*
 * buf.c -- simple string buffer for C
 *
 * Copyright (c) 2019-2021 David Demelier <markand@malikania.fr>
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
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "buf.h"

/*
 * Try to increase the buffer length by a power of two until we have enough
 * space to fit `desired'.
 *
 * Detects overflow and return -1 if happened or reallocation could not
 * occur.
 */
static int
growdbl(struct buf *b, size_t desired)
{
	size_t newcap = b->capacity;
	void *newptr;

	while (desired > newcap - b->length) {
		const size_t r = newcap * 2;

		/* Overflow occured. */
		if (r / newcap != 2) {
#if defined(ENOMEM)
			errno = ENOMEM;
#endif
			return -1;
		}

		newcap = r;
	}

	/* At this step we must have enough room. */
	assert(newcap - b->length >= desired);

	/* Pretty much impossible to reach but always assume it's possible. */
	if (newcap == SIZE_MAX) {
#if defined(ENOMEM)
		errno = ENOMEM;
#endif
		return -1;
	}

	if (!(newptr = BUF_REALLOC(b->data, newcap + 1)))
		return -1;

	b->data = newptr;
	b->capacity = newcap;

	return 0;
}

/*
 * Try to allocate just enough space for the `desired' amount of space. This is
 * only used when the buffer is already too large but hasn't reached SIZE_MAX
 * yet.
 *
 * Returns -1 if allocation failed.
 */
static int
growmin(struct buf *b, size_t desired)
{
	size_t newcap;
	void *newptr;

	if (desired >= SIZE_MAX - b->length) {
#if defined(ENOMEM)
		errno = ENOMEM;
#endif
		return -1;
	}

	/* Don't forget to keep what's remaining between capacity and length. */
	newcap = b->capacity + (desired - (b->capacity - b->length));

	/* Try to reallocate. */
	if (!(newptr = BUF_REALLOC(b->data, newcap + 1)))
		return -1;

	b->data = newptr;
	b->capacity = newcap;

	return 0;
}

/*
 * Entry point for reallocating data. Will try to allocate twice until we have
 * enough room and then only the minimal amount.
 */
static int
grow(struct buf *b, size_t desired)
{
	const size_t avail = b->capacity - b->length;

	if (avail >= desired)
		return 0;

	if (!b->capacity) {
		if (!(b->data = BUF_MALLOC(desired + 1)))
			return -1;

		b->capacity = desired;
	} else if (growdbl(b, desired) < 0 && growmin(b, desired) < 0)
		return -1;

	return 0;
}

void
buf_init(struct buf *b)
{
	assert(b);

	memset(b, 0, sizeof (*b));
}

int
buf_reserve(struct buf *b, size_t amount)
{
	assert(b);

	if (grow(b, amount) < 0)
		return -1;

	return 0;
}

int
buf_resize(struct buf *b, size_t size, char ch)
{
	assert(b);

	/* New size is smaller than curren't length, just update it. */
	if (size < b->length) {
		b->data[b->length = size] = 0;
		return 0;
	}

	/* New size is bigger, data may be reallocated. */
	if (grow(b, size - b->length) < 0)
		return -1;

	memset(&b->data[b->length], ch, size - b->length);
	b->length = size;
	b->data[b->length] = 0;

	return 0;
}

int
buf_shrink(struct buf *b)
{
	assert(b);

	void *newptr;

	if (b->length == 0) {
		free(b->data);
		b->data = NULL;
		b->length = b->capacity = 0;
		return 0;
	}

	if (!(newptr = BUF_REALLOC(b->data, b->length + 1)))
		return -1;

	b->data = newptr;
	b->capacity = b->length;

	return 0;
}

void
buf_erase(struct buf *b, size_t pos, size_t count)
{
	assert(b);
	assert(pos <= b->length);

	if (count > b->length - pos) {
		/* Optimize whole erase at pos. */
		b->data[pos] = 0;
		b->length = pos;
	} else {
		memmove(&b->data[pos], &b->data[pos + count], b->length - count);
		b->length -= count;
	}
}

int
buf_putc(struct buf *b, char c)
{
	assert(b);

	if (grow(b, 1) < 0)
		return -1;

	b->data[b->length++] = c;
	b->data[b->length] = 0;

	return 0;
}

int
buf_puts(struct buf *b, const char *s)
{
	assert(b);
	assert(s);

	const size_t len = strlen(s);

	if (grow(b, len) < 0)
		return -1;

	memcpy(&b->data[b->length], s, len + 1);
	b->length += len;

	return 0;
}

int
buf_printf(struct buf *b, const char *fmt, ...)
{
	assert(b);
	assert(fmt);

	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = buf_vprintf(b, fmt, ap);
	va_end(ap);

	return ret;
}

int
buf_vprintf(struct buf *b, const char *fmt, va_list args)
{
	assert(b);
	assert(fmt);

	va_list ap;
	int amount;

	/* Determine length. */
	va_copy(ap, args);
	amount = vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);

	if (amount < 0)
		return -1;

	/* Do actual copy. */
	if (grow(b, amount) < 0)
		return -1;

	va_copy(ap, args);
	amount = vsprintf(&b->data[b->length], fmt, ap);
	va_end(ap);

	if (amount < 0)
		return -1;

	b->length += amount;

	return 0;
}

int
buf_sub(struct buf *b, const struct buf *src, size_t pos, size_t count)
{
	assert(b);
	assert(src);
	assert(pos <= src->length);

	if (count >= src->length)
		count = src->length - pos;
	if (!(b->data = BUF_MALLOC(count + 1)))
		return -1;

	strncpy(b->data, &src->data[pos], count);
	b->length = count;
	b->capacity = count;
	b->data[b->length] = 0;

	return 0;
}

int
buf_dup(struct buf *b, const struct buf *src)
{
	assert(b);
	assert(src);

	if (!src->data)
		return 0;
	if (!(b->data = BUF_MALLOC(src->length + 1)))
		return -1;

	memcpy(b->data, src->data, src->length + 1);
	b->capacity = src->length;
	b->length = src->length;

	return 0;
}

void
buf_clear(struct buf *b)
{
	assert(b);

	if (b->data)
		b->data[b->length = 0] = 0;
}

void
buf_finish(struct buf *b)
{
	assert(b);

	BUF_FREE(b->data);
	b->data = NULL;
	b->capacity = b->length = 0;
}
