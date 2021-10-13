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
#include <string.h>

#include "vfs.h"

int
vfs_open(struct vfs *vfs, struct vfs_file *file, const char *entry, const char *mode)
{
	assert(vfs);
	assert(entry);

	return vfs->open(vfs, file, entry, mode);
}

void
vfs_finish(struct vfs *vfs)
{
	assert(vfs);

	vfs->finish(vfs);
	memset(vfs, 0, sizeof (*vfs));
}

size_t
vfs_file_read(struct vfs_file *file, void *buf, size_t bufsz)
{
	assert(file);
	assert(buf);

	return file->read(file, buf, bufsz);
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
