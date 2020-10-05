/*
 * rbuf.c -- basic utility for reading input buffers
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

#include <assert.h>

#include "rbuf.h"

void
rbuf_open(struct rbuf *rb, const void *data, size_t datasz)
{
	assert(rb);
	assert(data);

	rb->s = data;
	rb->e = rb->s + datasz;
}

bool
rbuf_readline(struct rbuf *rb, char *output, size_t outputsz)
{
	assert(rb);
	assert(output);
	assert(outputsz > 0);

	if (rb->s == rb->e)
		return false;

	for (--outputsz; rb->s != rb->e && *rb->s != '\n' && outputsz; outputsz--)
		*output++ = *rb->s++;

	/* Not enough space? */
	if (!outputsz && rb->s != rb->e && *rb->s != '\n')
		return false;

	/* Remove this '\n' if still present. */
	if (rb->s != rb->e && *rb->s == '\n')
		rb->s++;

	*output = '\0';

	return true;
}
