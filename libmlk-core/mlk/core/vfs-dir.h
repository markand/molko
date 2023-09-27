/*
 * vfs-dir.h -- VFS subsystem for directories
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

#ifndef MLK_CORE_VFS_DIR_H
#define MLK_CORE_VFS_DIR_H

#include <mlk/util/util.h>

#include "vfs.h"

struct mlk_vfs_dir_file {
	char path[MLK_PATH_MAX];

	struct mlk_vfs_file file;

	/** \cond MLK_PRIVATE_DECLS */
	void *handle;
	/** \endcond MLK_PRIVATE_DECLS */
};

struct mlk_vfs_dir {
	/**
	 * (read-only)
	 *
	 * Path to the directory.
	 */
	char path[MLK_PATH_MAX];

	struct mlk_vfs vfs;
};

#if defined(__cplusplus)
extern "C" {
#endif

void
mlk_vfs_dir_init(struct mlk_vfs_dir *dir, const char *path);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_VFS_DIR_H */
