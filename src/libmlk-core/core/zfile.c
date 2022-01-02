/*
 * zfile.c -- load potentially ZSTD compressed file
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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

#include "config.h"

#include <sys/stat.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#       include <io.h>
#else
#       include <unistd.h>
#endif

/*
 * If not enabled, we still need to check if a file is in zstandard so we use
 * the magic number defined in the zstd.h file or copy it if zstd is disabled.
 */
#if defined(MLK_WITH_ZSTD)
#       include <zstd.h>
#else
#       define ZSTD_MAGICNUMBER 0xFD2FB528
#endif

/* Windows thing. */
#if !defined(O_BINARY)
#       define O_BINARY 0
#endif

#include <port/port.h>

#include "zfile.h"

static int
is_zstd(int fd)
{
	uint32_t magic = 0;

	read(fd, &magic, sizeof (magic));
	lseek(fd, 0, SEEK_SET);

#if SDL_BYTEORDER != SDL_LIL_ENDIAN
	magic = SDL_Swap32(magic);
#endif

	return magic == ZSTD_MAGICNUMBER;
}

static int
decompress(int fd, struct zfile *zf)
{
#if defined(MLK_WITH_ZSTD)
	char *in = NULL;
	unsigned long long datasz;
	struct stat st;
	ssize_t nr;

	/* Load uncompressed data. */
	if (fstat(fd, &st) < 0)
		goto fail;
	if (!(in = calloc(1, st.st_size)) || (nr = read(fd, in, st.st_size)) != st.st_size)
		goto fail;

	switch ((datasz = ZSTD_getFrameContentSize(in, st.st_size))) {
	case ZSTD_CONTENTSIZE_ERROR:
		errno = EINVAL;
		goto fail;
	case ZSTD_CONTENTSIZE_UNKNOWN:
		errno = ENOTSUP;
		goto fail;
	default:
		break;
	}

	/* Finally decompress. */
	if (!(zf->data = calloc(1, datasz + 1)))
		goto fail;
	if (ZSTD_isError(ZSTD_decompress(zf->data, datasz, in, st.st_size))) {
		errno = EINVAL;
		goto fail;
	}
	if (!(zf->fp = port_fmemopen(zf->data, datasz, "r")))
		goto fail;

	close(fd);
	free(in);

	return 0;

fail:
	close(fd);
	free(zf->data);
	free(in);

	return -1;
#else
	(void)fd;
	(void)zf;

	errno = ENOTSUP;

	return -1;
#endif
}

static int
reopen(int fd, struct zfile *zf)
{
	if (!(zf->fp = fdopen(fd, "r")))
		return close(fd), -1;

	return 0;
}

int
zfile_open(struct zfile *zf, const char *path)
{
	assert(zf);
	assert(path);

	int fd;

	memset(zf, 0, sizeof (*zf));

	if ((fd = open(path, O_RDONLY | O_BINARY)) < 0)
		return -1;

	return is_zstd(fd) ? decompress(fd, zf) : reopen(fd, zf);
}

void
zfile_close(struct zfile *zf)
{
	assert(zf);

	free(zf->data);

	if (zf->fp)
		fclose(zf->fp);

	memset(zf, 0, sizeof (*zf));
}
