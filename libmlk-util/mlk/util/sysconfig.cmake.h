/*
 * sysconfig.cmake.h -- miscellaneous utilities and portability
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

#ifndef MLK_UTIL_SYSCONFIG_H
#define MLK_UTIL_SYSCONFIG_H

#define MLK_PREFIX      "@CMAKE_INSTALL_PREFIX@"
#define MLK_BINDIR      "@CMAKE_INSTALL_BINDIR@"
#define MLK_LIBDIR      "@CMAKE_INSTALL_LIBDIR@"
#define MLK_LOCALEDIR   "@CMAKE_INSTALL_LOCALEDIR@"

#cmakedefine MLK_WITH_NLS
#cmakedefine MLK_WITH_ZIP

#cmakedefine MLK_HAVE_PATH_MAX

#cmakedefine MLK_HAVE_DIRENT_H
#cmakedefine MLK_HAVE_LIBGEN_H

#cmakedefine MLK_HAVE_BASENAME
#cmakedefine MLK_HAVE_DIRNAME
#cmakedefine MLK_HAVE_FMEMOPEN
#cmakedefine MLK_HAVE_STRLCAT
#cmakedefine MLK_HAVE_STRLCPY

#endif /* !MLK_UTIL_SYSCONFIG_H */
