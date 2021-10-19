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

#include <limits.h>
#include <stdio.h>

/* PATH_MAX (defined in limits.h) (POSIX) */
#if !defined(PATH_MAX)
#       define PATH_MAX 2048
#endif

/* (POSIX) */
#if defined(_WIN32)
typedef long long int ssize_t;
#endif

/* OpenBSD extension (in next POSIX version). */
size_t
port_strlcpy(char *, const char *, size_t);

/* Same as strlcpy. */
size_t
port_strlcat(char *, const char *, size_t);

/* POSIX. */
FILE *
port_fmemopen(void *, size_t, const char *);

/* POSIX. */
char *
port_basename(char *);

/* POSIX. */
char *
port_dirname(char *);

extern int port_opterr;
extern int port_optind;
extern int port_optopt;
extern char *port_optarg;

int
port_getopt(int, char **, const char *);

#endif /* !MOLKO_PORT_H */
