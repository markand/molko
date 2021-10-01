/*
 * port.h -- portability bits
 *
 * Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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

#ifndef MOLKO_PORT_H
#define MOLKO_PORT_H

#include "config.h"

/* {{{ strlcpy (BSD extension, incoming in next POSIX). */

#if !defined(MLK_HAS_STRLCPY)

#include <stddef.h>

size_t
strlcpy(char *, const char *, size_t);

#endif /* !MLK_HAS_STRLCPY */

/* }}} */

/* {{{ fmemopen (POSIX). */

#if !defined(MLK_HAS_FMEMOPEN)

#include <stdio.h>

FILE *
fmemopen(void *, size_t, const char *);

#endif /* !MLK_HAS_FMEMOPEN */

/* }}} */

/* {{{ basename (POSIX). */

#if !defined(MLK_HAS_BASENAME)

char *
basename(char *);

#endif /* !MLK_HAS_BASENAME */

/* }}} */

/* {{{ dirname (POSIX). */

#if !defined(MLK_HAS_BASENAME)

char *
dirname(char *);

#endif /* !MLK_HAS_DIRNAME */

/* }}} */

/* {{{ getopt (POSIX) */

#if !defined(MLK_HAS_GETOPT)

extern int opterr;
extern int optind;
extern int optopt;
extern char *optarg;

int
getopt(int, char **, const char *);

#endif /* !MLK_HAS_GETOPT */

/* }}} */

/* PATH_MAX (defined in limits.h) (POSIX) */
#if !defined(MLK_HAS_PATH_MAX)
#       define PATH_MAX 2048
#endif

/* ssize_t (defined in sys/types.h) (POSIX) */
#if !defined(MLK_HAS_SSIZE_T)
typedef long long int ssize_t;
#endif

#endif /* !MOLKO_PORT_H */
