/*
 * vfs.c -- virtual file system abstraction
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

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "buf.h"
#include "error.h"
#include "vfs.h"
#include "vfs_p.h"

int
vfs_open(struct vfs *vfs, struct vfs_file *file, const char *entry, const char *mode)
{
	assert(vfs);
	assert(entry);

	memset(file, 0, sizeof (*file));

	return vfs->open(vfs, file, entry, mode);
}

int
vfs_ok(struct vfs *vfs)
{
	return vfs && vfs->open && vfs->finish;
}

void
vfs_finish(struct vfs *vfs)
{
	assert(vfs);

	vfs->finish(vfs);
	memset(vfs, 0, sizeof (*vfs));
}

int
vfs_file_ok(struct vfs_file *file)
{
	return file && file->read && file->write && file->flush && file->finish;
}

size_t
vfs_file_read(struct vfs_file *file, void *buf, size_t bufsz)
{
	assert(file);
	assert(buf);

	return file->read(file, buf, bufsz);
}

char *
vfs_file_aread(struct vfs_file *file, size_t *outlen)
{
	struct buf buf = {0};
	char data[BUFSIZ];
	size_t nr;

	while ((nr = vfs_file_read(file, data, sizeof (data))) > 0) {
		if (buf_printf(&buf, "%.*s", (int)nr, data) < 0) {
			errorf("%s", strerror(errno));
			buf_finish(&buf);
			return NULL;
		}
	}

	if (outlen)
		*outlen = buf.length;

	return buf.data;
}

size_t
vfs_file_write(struct vfs_file *file, void *buf, size_t bufsz)
{
	assert(file);
	assert(buf);

	return file->write(file, buf, bufsz);
}

int
vfs_file_flush(struct vfs_file *file)
{
	assert(file);

	return file->flush(file);
}

void
vfs_file_finish(struct vfs_file *file)
{
	assert(file);

	file->finish(file);
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
vfs_to_rw(struct vfs_file *file)
{
	SDL_RWops *ops;
	char *data;
	size_t datasz;

	if (!(data = vfs_file_aread(file, &datasz)))
		return NULL;
	if (!(ops = SDL_RWFromConstMem(data, datasz))) {
		free(data);
		return errorf("%s", SDL_GetError()), NULL;
	}

	ops->close = rw_vfs_file_close;

	return ops;
}
