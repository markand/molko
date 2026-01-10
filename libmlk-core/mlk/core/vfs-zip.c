/*
 * vfs-zip.c -- VFS subsystem for zip archives
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

#include "sysconfig.h"

#if defined(MLK_WITH_ZIP)

#include <assert.h>
#include <string.h>

#include <zip.h>

#include "alloc.h"
#include "err.h"
#include "util.h"
#include "vfs-zip.h"
#include "vfs.h"

#define MLK_VFS_ZIP_FILE(self) \
	MLK_UTIL_CONTAINER_OF(self, struct mlk_vfs_zip_file, file)

#define MLK_VFS_ZIP(self) \
	MLK_UTIL_CONTAINER_OF(self, struct mlk_vfs_zip, vfs)

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
file_read(struct mlk_vfs_file *self, void *buf, size_t bufsz)
{
	return mlk_vfs_zip_file_read(MLK_VFS_ZIP_FILE(self), buf, bufsz);
}

static void
file_finish(struct mlk_vfs_file *self)
{
	mlk_vfs_zip_file_finish(MLK_VFS_ZIP_FILE(self));
}

static struct mlk_vfs_file *
vfs_open(struct mlk_vfs *self, const char *entry, const char *mode)
{
	return mlk_vfs_zip_open(MLK_VFS_ZIP(self), entry, mode);
}

static void
vfs_finish(struct mlk_vfs *self)
{
	mlk_vfs_zip_finish(MLK_VFS_ZIP(self));
}

int
mlk_vfs_zip_init(struct mlk_vfs_zip *zip, const char *file, const char *mode)
{
	assert(zip);
	assert(file);
	assert(mode);

	if (!(zip->handle = zip_open(file, mkflags(mode), NULL))) {
		mlk_errf("%s: unable to open zip file", file);
		return -1;
	}

	zip->vfs.open = vfs_open;
	zip->vfs.finish = vfs_finish;

	return 0;
}

struct mlk_vfs_file *
mlk_vfs_zip_open(struct mlk_vfs_zip *zip, const char *entry, const char *mode)
{
	(void)mode;

	struct mlk_vfs_zip_file *file;

	file = mlk_alloc_new0(1, sizeof (*file));

	if (!(file->handle = zip_fopen(zip->handle, entry, 0))) {
		mlk_errf("unable to open file in archive");
		mlk_alloc_free(file);
		return NULL;
	}

	file->file.read = file_read;
	file->file.finish = file_finish;

	return &file->file;
}

void
mlk_vfs_zip_finish(struct mlk_vfs_zip *zip)
{
	assert(zip);

	zip_close(zip->handle);
	zip->handle = NULL;
}

size_t
mlk_vfs_zip_file_read(struct mlk_vfs_zip_file *file, void *buf, size_t bufsz)
{
	assert(file);
	assert(buf);

	zip_int64_t rv;

	if ((rv = zip_fread(file->handle, buf, bufsz)) < 0) {
		mlk_errf("%s", zip_file_strerror(file->handle));
		return -1;
	}

	return rv;
}

void
mlk_vfs_zip_file_finish(struct mlk_vfs_zip_file *file)
{
	assert(file);

	zip_fclose(file->handle);
	mlk_alloc_free(file);
}

#endif /* !MLK_WITH_ZIP */
