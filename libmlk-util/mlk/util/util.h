/*
 * util.h -- miscellaneous utilities
 *
 * Copyright (c) 2020-2023 David Demelier <markand@malikania.fr>
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

#ifndef MLK_PORT_H
#define MLK_PORT_H

#include <limits.h>
#include <stdio.h>

/*
 * This file helps finding what are the available features accross the various
 * operating system in the landscape.
 *
 * The following macros are automatically set depending on the operating
 * system:
 *
 * - MLK_OS_WINDOWS: running on any Windows machine
 * - MLK_OS_POSIX: every mostly POSIX systems
 *
 * The following macro will be automatically defined unless the user override
 * them:
 *
 * - MLK_HAS_FMEMOPEN: defined if fmemopen function is available.
 * - MLK_HAS_SSIZE_T: defined if ssize_t typedef is available.
 */

#if defined(_WIN32)
#       define MLK_OS_WINDOWS
#elif defined(__FreeBSD__)
#       define MLK_OS_POSIX
#elif defined(__DragonFly__)
#       define MLK_OS_POSIX
#elif defined(__OpenBSD__)
#       define MLK_OS_POSIX
#elif defined(__NetBSD__)
#       define MLK_OS_POSIX
#elif defined(__linux__)
#       define MLK_OS_POSIX
#elif defined(__APPLE__)
#       define MLK_OS_POSIX
#       define MLK_OS_APPLE
#endif

#if !defined(PATH_MAX)
#       define PATH_MAX 2048
#endif

#if defined(MLK_OS_POSIX) && !defined(MLK_HAS_SSIZE_T)
#       define MLK_HAS_SSIZE_T
#endif

#if !defined(MLK_HAS_SSIZE_T)
typedef long long int ssize_t;
#endif

#if defined(MLK_OS_POSIX) && !defined(MLK_HAS_FMEMOPEN)
#       define MLK_HAS_FMEMOPEN
#endif

size_t
mlk_util_strlcpy(char *, const char *, size_t);

size_t
mlk_util_strlcat(char *, const char *, size_t);

FILE *
mlk_util_fmemopen(void *, size_t, const char *);

char *
mlk_util_basename(char *);

char *
mlk_util_dirname(char *);

extern int mlk_util_opterr;
extern int mlk_util_optind;
extern int mlk_util_optopt;
extern char *mlk_util_optarg;

int
mlk_util_getopt(int, char **, const char *);

#endif /* !MLK_PORT_H */