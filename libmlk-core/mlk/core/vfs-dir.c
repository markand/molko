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
	size_t len;

	len = strlen(path);

	for (char *p = path; *p; ++p)
		if (*p == '\\')
			*p = '/';

	while (len && path[len - 1] == '/')
		path[--len] = 0;
}

/*
 * TODO
 *
 * Add an optional POSIX implementation based on open(2) if function is
 * available.
 */
static FILE *
makefile(const char *path, const char *mode)
{
	FILE *fp;
	const char *fmode = NULL;
	int r = 0, w = 0, e = 0, t = 0, seek = 0, flags, whence;
	long offset;
	size_t len;

	/*
	 * Determine which read-write mode we need, if unset by the user assumes
	 * read-only for convenience.
	 *
	 * Conversion as following:
	 *
	 * | mode  | fopen mode | fseek?  | tewr        |
	 * |-------+------------+---------+-------------|
	 * | r     | rb         | no      | 0001 (0x01) |
	 * | w     | r+b        | yes, 0  | 0010 (0x02) |
	 * | we    | r+b        | yes, -1 | 0110 (0x06) |
	 * | rw    | r+b        | yes, 0  | 0011 (0x03) |
	 * | rwe   | r+b        | yes, -1 | 0111 (0x07) |
	 * | wt    | wb         | no      | 1010 (0x0a) |
	 * | rwt   | w+b        | no      | 1011 (0x0b) |
	 *
	 * Notes:
	 *
	 * Standard C function fopen does not have support to open a file for
	 * writing only at the beginning (without destroying content) so we have
	 * to read-write with a call to fseek if at-end is requested. The mode
	 * 'a' is never applicable because it always append to the file no
	 * matter fseek position.
	 *
	 * We could use open(2) POSIX function but that would make the code less
	 * portable.
	 */
	len = strlen(mode);

	r = strcspn(mode, "r") != len;
	w = strcspn(mode, "w") != len;
	e = strcspn(mode, "e") != len;
	t = strcspn(mode, "t") != len;

	/* For simplicity, convert the individual open mode into a bitmask. */
	flags = r << 0 | w << 1 | e << 2 | t << 3;

	switch (flags) {
	case 0x01:
		fmode = "rb";
		break;
	case 0x02:
	case 0x03:
		fmode = "r+b";
		seek = 1;
		offset = 0;
		whence = SEEK_SET;
		break;
	case 0x06:
	case 0x07:
		fmode = "r+b";
		seek = 1;
		offset = 0;
		whence = SEEK_END;
		break;
	case 0x0a:
		fmode = "wb";
		break;
	case 0x0b:
		fmode = "w+b";
		break;
	default:
		mlk_errf("incompatible modes specified");
		return NULL;
	}

	if (!(fp = fopen(path, fmode))) {
		mlk_errf("%s", strerror(errno));
		return NULL;
	}

	if (seek && fseek(fp, offset, whence) < 0) {
		fclose(fp);
		mlk_errf("%s", strerror(errno));
		return NULL;
	}

	return fp;
}

static size_t
file_read(struct mlk_vfs_file *self, void *buf, size_t bufsz)
{
	return mlk_vfs_dir_file_read(MLK_VFS_DIR_FILE(self), buf, bufsz);
}

static size_t
file_write(struct mlk_vfs_file *self, const void *buf, size_t bufsz)
{
	return mlk_vfs_dir_file_write(MLK_VFS_DIR_FILE(self), buf, bufsz);
}

static int
file_flush(struct mlk_vfs_file *self)
{
	return mlk_vfs_dir_file_flush(MLK_VFS_DIR_FILE(self));
}

static void
file_finish(struct mlk_vfs_file *self)
{
	mlk_vfs_dir_file_finish(MLK_VFS_DIR_FILE(self));
}

static struct mlk_vfs_file *
vfs_open(struct mlk_vfs *self, const char *entry, const char *mode)
{
	return mlk_vfs_dir_open(MLK_VFS_DIR(self), entry, mode);
}

void
mlk_vfs_dir_init(struct mlk_vfs_dir *dir, const char *path)
{
	assert(dir);
	assert(path);

	mlk_util_strlcpy(dir->path, path, sizeof (dir->path));

	/* Remove terminator and switch to UNIX paths. */
	normalize(dir->path);

	dir->vfs.open = vfs_open;
	dir->vfs.finish = NULL;
}

struct mlk_vfs_file *
mlk_vfs_dir_open(struct mlk_vfs_dir *dir, const char *entry, const char *mode)
{
	struct mlk_vfs_dir_file *file;

	file = mlk_alloc_new0(1, sizeof (*file));

	snprintf(file->path, sizeof (file->path), "%s/%s", dir->path, entry);

	if (!(file->handle = makefile(file->path, mode))) {
		mlk_alloc_free(file);
		return NULL;
	}

	file->file.read = file_read;
	file->file.write = file_write;
	file->file.flush = file_flush;
	file->file.finish = file_finish;

	return &file->file;
}

void
mlk_vfs_dir_finish(struct mlk_vfs_dir *dir)
{
	assert(dir);

	/* Kept for future use. */
	(void)dir;
}

size_t
mlk_vfs_dir_file_read(struct mlk_vfs_dir_file *file, void *buf, size_t bufsz)
{
	assert(file);
	assert(buf);

	size_t rv;

	rv = fread(buf, 1, bufsz, file->handle);

	if (ferror(file->handle))
		return -1;

	return rv;
}

size_t
mlk_vfs_dir_file_write(struct mlk_vfs_dir_file *file, const void *buf, size_t bufsz)
{
	assert(file);
	assert(buf);

	size_t rv;

	rv = fwrite(buf, 1, bufsz, file->handle);

	if (ferror(file->handle))
		return -1;

	return rv;
}

int
mlk_vfs_dir_file_flush(struct mlk_vfs_dir_file *file)
{
	assert(file);

	return fflush(file->handle) == EOF ? -1 : 0;
}

void
mlk_vfs_dir_file_finish(struct mlk_vfs_dir_file *file)
{
	assert(file);

	fclose(file->handle);
	mlk_alloc_free(file);
}
