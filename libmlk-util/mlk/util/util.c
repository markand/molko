/*
 * util.c -- miscellaneous utilities and portability
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

#include "sysconfig.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(MLK_HAVE_LIBGEN_H)
#       include <libgen.h>
#endif

void
mlk_util_die(const char *fmt, ...)
{
	assert(fmt);

	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	exit(1);
}

/*
 * The following functions are just wrapper for the native system routine if they
 * are available on the system, otherwise the symbol will be implemented in
 * openbsd/<name>.c
 */

#if defined(MLK_HAVE_STRLCPY)

size_t
mlk_util_strlcpy(char *dst, const char *src, size_t dstsz)
{
	return strlcpy(dst, src, dstsz);
}

#endif

#if defined(MLK_HAVE_STRLCAT)

size_t
mlk_util_strlcat(char *dst, const char *src, size_t dstsz)
{
	return strlcat(dst, src, dstsz);
}

#endif

#if defined(MLK_HAVE_BASENAME)

char *
mlk_util_basename(char *path)
{
	return basename(path);
}

#endif

#if defined(MLK_HAVE_DIRNAME)

char *
mlk_util_dirname(char *path)
{
	return dirname(path);
}

#endif
