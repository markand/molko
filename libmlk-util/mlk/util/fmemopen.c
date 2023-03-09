/*
 * fmemopen.h -- fmemopen polyfill
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

#include <stdio.h>

#if !defined(MLK_HAVE_FMEMOPEN)

#if defined(_WIN32)

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <io.h>
#include <share.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

FILE *
mlk_util_fmemopen(void *buf, size_t size, const char *mode)
{
	char temppath[MAX_PATH + 1], filename[MAX_PATH + 1];
	FILE *fp;
	int fd, flags;

	flags = _O_CREAT        |
	        _O_SHORT_LIVED  |
	        _O_TEMPORARY    |
	        _O_RDWR         |
	        _O_NOINHERIT;

	if (strchr(mode, 'b'))
		flags |= _O_BINARY;

	if (!GetTempPathA(sizeof (temppath), temppath))
		return NULL;
	if (!GetTempFileNameA(temppath, "MLK", 0, filename))
		return NULL;
	if (_sopen_s(&fd, filename, flags, _SH_DENYRW, _S_IREAD | _S_IWRITE) < 0)
		return NULL;
	if (!(fp = _fdopen(fd, mode))) {
		_close(fd);
		return NULL;
	}

	fwrite(buf, size, 1, fp);
	rewind(fp);

	return fp;
}

#else

FILE *
mlk_util_fmemopen(void *buf, size_t len, const char *type)
{
	(void)buf;
	(void)len;
	(void)type;

	/* Unsupported platform. */
	return NULL;
}

#endif

#else

FILE *
mlk_util_fmemopen(void *buf, size_t len, const char *type)
{
	return fmemopen(buf, len, type);
}

#endif
