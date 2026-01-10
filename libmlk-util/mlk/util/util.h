/*
 * util.h -- miscellaneous utilities and portability
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

#ifndef MLK_UTIL_UTIL_H
#define MLK_UTIL_UTIL_H

/**
 * \file mlk/util/util.h
 * \brief Miscellaneous utilities and portability
 */

#include "sysconfig.h"

#include <limits.h>
#include <stdio.h>

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(_MSC_VER)
#       define MLK_THREAD_LOCAL
#else
#       define MLK_THREAD_LOCAL _Thread_local
#endif

#if defined(MLK_HAVE_PATH_MAX)
#       define MLK_PATH_MAX PATH_MAX
#else
#       define MLK_PATH_MAX 1024
#endif

/**
 * Print a fatal message and exit with code 1.
 *
 * \pre fmt != NULL
 * \param fmt the printf format string
 */
void
mlk_util_die(const char *fmt, ...);

/**
 * Compatibility version of OpenBSD [strlcpy].
 *
 * [strlcpy]: http://man.openbsd.org/strlcpy
 */
size_t
mlk_util_strlcpy(char *dst, const char *src, size_t dstsz);

/**
 * Compatibility version of OpenBSD [strlcat].
 *
 * [strlcat]: http://man.openbsd.org/strlcat
 */
size_t
mlk_util_strlcat(char *dst, const char *src, size_t dstsz);

/**
 * Portable version of POSIX/C23 [fmemopen].
 *
 * [basename]: https://pubs.opengroup.org/onlinepubs/9699919799/functions/fmemopen.html
 */
FILE *
mlk_util_fmemopen(void *, size_t, const char *);

/**
 * Portable version of POSIX [basename].
 *
 * [basename]: https://pubs.opengroup.org/onlinepubs/9699919799/functions/basename.html
 */
char *
mlk_util_basename(char *);

/**
 * Portable version of POSIX [dirname].
 *
 * [dirname]: https://pubs.opengroup.org/onlinepubs/9699919799/functions/dirname.html
 */
char *
mlk_util_dirname(char *);

extern int mlk_util_opterr;
extern int mlk_util_optind;
extern int mlk_util_optopt;
extern char *mlk_util_optarg;

/**
 * Portable version of POSIX [getopt].
 *
 * All variables that the standard getopt uses are prefixed with `mlk_util_`.
 *
 * - optarg -> mlk_util_optarg
 * - opterr -> mlk_util_opterr
 * - optind -> mlk_util_optind
 * - optopt -> mlk_util_optopt
 *
 * [getopt]: https://pubs.opengroup.org/onlinepubs/9699919799/functions/getopt.html
 */
int
mlk_util_getopt(int argc, char **argv, const char *optstring);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_UTIL_UTIL_H */
