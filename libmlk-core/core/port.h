/*
 * port.h -- portability bits
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

#ifndef MOLKO_CORE_PORT_H
#define MOLKO_CORE_PORT_H

/* {{{ strlcpy (BSD extension, incoming in next POSIX). */

#if defined(_WIN32)
#       define MOLKO_PORT_NEED_STRLCPY
#endif

#if defined(MOLKO_PORT_NEED_STRLCPY)

#include <stddef.h>

size_t
strlcpy(char *, const char *, size_t);

#endif

/* }}} */

/* {{{ fmemopen (POSIX). */

#if defined(_WIN32)
#       define MOLKO_PORT_NEED_FMEMOPEN
#endif

#if defined(MOLKO_PORT_NEED_FMEMOPEN)

#include <stdio.h>

FILE *
fmemopen(void *, size_t, const char *);

#endif

/* }}} */

#endif /* !MOLKO_CORE_PORT_H */
