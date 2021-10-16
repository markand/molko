/*
 * vfs.h -- virtual file system abstraction
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

#ifndef MLK_CORE_VFS_H
#define MLK_CORE_VFS_H

#include "core.h"

struct vfs_file;

struct vfs {
	void *data;
	int (*open)(struct vfs *, struct vfs_file *, const char *, const char *);
	void (*finish)(struct vfs *);
};

struct vfs_file {
	void *data;
	size_t (*read)(struct vfs_file *, void *, size_t);
	size_t (*write)(struct vfs_file *, const void *, size_t);
	int (*flush)(struct vfs_file *);
	void (*finish)(struct vfs_file *);
};

CORE_BEGIN_DECLS

/* vfs */

int
vfs_open(struct vfs *, struct vfs_file *, const char *, const char *);

int
vfs_ok(struct vfs *);

void
vfs_finish(struct vfs *);

/* vfs_file */

int
vfs_file_ok(struct vfs_file *);

size_t
vfs_file_read(struct vfs_file *, void *, size_t);

char *
vfs_file_aread(struct vfs_file *, size_t *);

size_t
vfs_file_write(struct vfs_file *, void *, size_t);

int
vfs_file_flush(struct vfs_file *);

void
vfs_file_finish(struct vfs_file *);

CORE_END_DECLS

#endif /* MLK_CORE_VFS_H */
