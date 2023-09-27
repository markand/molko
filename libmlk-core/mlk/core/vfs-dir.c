/*
 * vfs-dir.c -- VFS subsystem for directories
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
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"
#include "err.h"
#include "util.h"
#include "vfs-dir.h"
#include "vfs.h"

#define MLK_VFS_DIR_FILE(self) \
	MLK_CONTAINER_OF(self, struct mlk_vfs_dir_file, file)

#define MLK_VFS_DIR(self) \
	MLK_CONTAINER_OF(self, struct mlk_vfs_dir, vfs)

static inline void
normalize(char *path)
{
	size_t len = strlen(path);

	for (char *p = path; *p; ++p)
		if (*p == '\\')
			*p = '/';

	while (len && path[len - 1] == '/')
		path[--len] = 0;
}

static size_t
file_read(struct mlk_vfs_file *self, void *buf, size_t bufsz)
{
	struct mlk_vfs_dir_file *file = MLK_VFS_DIR_FILE(self);
	size_t rv;

	rv = fread(buf, 1, bufsz, file->handle);

	if (ferror(file->handle))
		return -1;

	return rv;
}

static size_t
file_write(struct mlk_vfs_file *self, const void *buf, size_t bufsz)
{
	struct mlk_vfs_dir_file *file = MLK_VFS_DIR_FILE(self);
	size_t rv;

	rv = fwrite(buf, 1, bufsz, file->handle);

	if (ferror(file->handle))
		return -1;

	return rv;
}

static int
file_flush(struct mlk_vfs_file *self)
{
	struct mlk_vfs_dir_file *file = MLK_VFS_DIR_FILE(self);

	return fflush(file->handle) == EOF ? -1 : 0;
}

static void
file_free(struct mlk_vfs_file *self)
{
	struct mlk_vfs_dir_file *file = MLK_VFS_DIR_FILE(self);

	fclose(file->handle);
	mlk_alloc_free(file);
}

static struct mlk_vfs_file *
vfs_open(struct mlk_vfs *self, const char *entry, const char *mode)
{
	struct mlk_vfs_dir *dir = MLK_VFS_DIR(self);
	struct mlk_vfs_dir_file *file;

	file = mlk_alloc_new0(1, sizeof (*file));

	snprintf(file->path, sizeof (file->path), "%s/%s", dir->path, entry);

	if (!(file->handle = fopen(file->path, mode))) {
		mlk_errf("%s: %s", file->path, strerror(errno));
		mlk_alloc_free(file);
		return NULL;
	}

	file->file.read = file_read;
	file->file.write = file_write;
	file->file.flush = file_flush;
	file->file.free = file_free;

	return &file->file;
}

void
mlk_vfs_dir_init(struct mlk_vfs_dir *dir, const char *path)
{
	assert(dir);
	assert(path);

	mlk_util_strlcpy(dir->path, path, sizeof (dir->path));

	/* Remove terminator and switch to UNIX paths. */
	normalize(dir->path);

	dir->vfs.data = NULL;
	dir->vfs.open = vfs_open;
	dir->vfs.finish = NULL;
}
