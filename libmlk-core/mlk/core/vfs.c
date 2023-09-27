/*
 * vfs.c -- virtual file system abstraction
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

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "alloc.h"
#include "err.h"
#include "vfs.h"
#include "vfs_p.h"

struct mlk_vfs_file *
mlk_vfs_open(struct mlk_vfs *vfs,
             const char *entry,
             const char *mode)
{
	assert(vfs);
	assert(entry);
	assert(mode);

	return vfs->open(vfs, entry, mode);
}

void
mlk_vfs_finish(struct mlk_vfs *vfs)
{
	assert(vfs);

	if (vfs->finish)
		vfs->finish(vfs);
}

size_t
mlk_vfs_file_read(struct mlk_vfs_file *file, void *buf, size_t bufsz)
{
	assert(file);
	assert(buf);

	return file->read(file, buf, bufsz);
}

char *
mlk_vfs_file_aread(struct mlk_vfs_file *file, size_t *outlen)
{
	char data[BUFSIZ], *str;
	size_t nr, len = 0, cap = 128;

	/* Initial allocation. */
	str = mlk_alloc_new0(cap, 1);

	while ((nr = mlk_vfs_file_read(file, data, sizeof (data))) > 0) {
		if (nr >= cap - len) {
			cap *= 2;
			str  = mlk_alloc_resize(str, cap);
		}

		sprintf(&str[len], "%.*s", (int)nr, data);
		len += nr;
	}

	if (nr == (size_t)-1) {
		mlk_alloc_free(str);
		return NULL;
	}

	if (outlen)
		*outlen = len;

	return str;
}

size_t
mlk_vfs_file_write(struct mlk_vfs_file *file, void *buf, size_t bufsz)
{
	assert(file);
	assert(buf);

	return file->write(file, buf, bufsz);
}

int
mlk_vfs_file_flush(struct mlk_vfs_file *file)
{
	assert(file);

	if (file->flush)
		return file->flush(file);

	return 0;
}

void
mlk_vfs_file_free(struct mlk_vfs_file *file)
{
	assert(file);

	if (file->free)
		file->free(file);
}

/* private */

static int
rw_vfs_file_close(SDL_RWops *context)
{
	free(context->hidden.mem.base);
	free(context);

	return 0;
}

SDL_RWops *
mlk__vfs_to_rw(struct mlk_vfs_file *file)
{
	SDL_RWops *ops;
	char *data;
	size_t datasz;

	if (!(data = mlk_vfs_file_aread(file, &datasz)))
		return NULL;
	if (!(ops = SDL_RWFromConstMem(data, datasz))) {
		free(data);
		return mlk_errf("%s", SDL_GetError()), NULL;
	}

	ops->close = rw_vfs_file_close;

	return ops;
}
