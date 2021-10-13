/*
 * vfs-zip.c -- VFS subsystem for zip archives
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

#include <zip.h>

#include "error.h"
#include "vfs-zip.h"
#include "vfs.h"

static inline int
mkflags(const char *mode)
{
	/* TODO: this should check for mutual exclusions. */
	int flags = 0;

	for (; *mode; ++mode) {
		switch (*mode) {
		case 'w':
			flags |= ZIP_CREATE;
			break;
		case 'x':
			flags |= ZIP_EXCL;
			break;
		case '+':
			flags |= ZIP_TRUNCATE;
			break;
		case 'r':
			flags |= ZIP_RDONLY;
			break;
		default:
			break;
		}
	}

	return flags;
}

static size_t
file_read(struct vfs_file *file, void *buf, size_t bufsz)
{
	return zip_fread(file->data, buf, bufsz);
}

static size_t
file_write(struct vfs_file *file, const void *buf, size_t bufsz)
{
	(void)file;
	(void)buf;
	(void)bufsz;

	return errorf("operation not supported");
}

static int
file_flush(struct vfs_file *file)
{
	(void)file;

	return 0;
}

static void
file_finish(struct vfs_file *file)
{
	zip_fclose(file->data);
}

static int
impl_open(struct vfs *vfs, struct vfs_file *file, const char *entry, const char *mode)
{
	(void)mode;

	if (!(file->data = zip_fopen(vfs->data, entry, 0)))
		return errorf("unable to open file in archive");

	file->read = file_read;
	file->write = file_write;
	file->flush = file_flush;
	file->finish = file_finish;

	return 0;
}

static void
impl_finish(struct vfs *vfs)
{
	zip_close(vfs->data);
}

int
vfs_zip(struct vfs *vfs, const char *file, const char *mode)
{
	assert(vfs);
	assert(file);

	memset(vfs, 0, sizeof (*vfs));

	if (!(vfs->data = zip_open(file, mkflags(mode), NULL)))
		return errorf("%s: unable to open zip file", file);

	vfs->open = impl_open;
	vfs->finish = impl_finish;

	return 0;
}
