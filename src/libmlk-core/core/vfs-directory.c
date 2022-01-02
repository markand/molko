/*
 * vfs-directory.c -- VFS subsystem for directories
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

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <port/port.h>

#include "alloc.h"
#include "error.h"
#include "vfs.h"

struct self {
	char base[PATH_MAX];
};

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
file_read(struct vfs_file *file, void *buf, size_t bufsz)
{
	return fread(buf, 1, bufsz, file->data);
}

static size_t
file_write(struct vfs_file *file, const void *buf, size_t bufsz)
{
	return fwrite(buf, 1, bufsz, file->data);
}

static int
file_flush(struct vfs_file *file)
{
	return fflush(file->data) == EOF ? -1 : 0;
}

static void
file_finish(struct vfs_file *file)
{
	fclose(file->data);
}

static int
dir_open(struct vfs *vfs, struct vfs_file *file, const char *entry, const char *mode)
{
	struct self *self = vfs->data;
	char path[PATH_MAX];

	snprintf(path, sizeof (path), "%s/%s", self->base, entry);

	if (!(file->data = fopen(path, mode)))
		return errorf("%s: %s", path, strerror(errno));

	file->read = file_read;
	file->write = file_write;
	file->flush = file_flush;
	file->finish = file_finish;

	return 0;
}

static void
dir_finish(struct vfs *vfs)
{
	free(vfs->data);
}

void
vfs_directory(struct vfs *vfs, const char *path)
{
	assert(vfs);
	assert(path);

	struct self *self;

	self = alloc_new(sizeof (*self));
	port_strlcpy(self->base, path, sizeof (self->base));

	/* Remove terminator and switch to UNIX paths. */
	normalize(self->base);

	vfs->data = self;
	vfs->open = dir_open;
	vfs->finish = dir_finish;
}
