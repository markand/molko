/*
 * buf.h -- simple string buffer for C
 *
 * Copyright (c) 2019-2022 David Demelier <markand@malikania.fr>
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

#ifndef BUF_H
#define BUF_H

#include <stdarg.h>
#include <stddef.h>

#include "alloc.h"

#if !defined(BUF_MALLOC)
#	define BUF_MALLOC alloc_new
#endif

#if !defined(BUF_REALLOC)
#	define BUF_REALLOC alloc_renew
#endif

#if !defined(BUF_FREE)
#	define BUF_FREE alloc_free
#endif

#if defined(__cplusplus)
extern "C" {
#endif

struct buf {
	char *data;
	size_t length;
	size_t capacity;
};

void
buf_init(struct buf *);

int
buf_reserve(struct buf *, size_t);

int
buf_resize(struct buf *, size_t, char);

int
buf_shrink(struct buf *);

void
buf_erase(struct buf *, size_t, size_t);

int
buf_putc(struct buf *, char);

int
buf_puts(struct buf *, const char *);

int
buf_printf(struct buf *, const char *, ...);

int
buf_vprintf(struct buf *, const char *, va_list);

int
buf_sub(struct buf *, const struct buf *, size_t, size_t);

int
buf_dup(struct buf *, const struct buf *);

void
buf_clear(struct buf *);

void
buf_finish(struct buf *);

#if defined(__cplusplus)
}
#endif

#endif /* !BUF_H */
