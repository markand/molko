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

/**
 * \file mlk/core/vfs-dir.h
 * \brief VFS subsystem for directories.
 *
 * This module can be used to read files relative to a filesystem directory path
 * using mlk/core/vfs.h abstract VFS module.
 *
 * It is implemented using the ::MLK_UTIL_CONTAINER_OF macro which means you can
 * use it and derive from it to add or modify its functions.
 *
 * ## Members used
 *
 * The following VFS members are used:
 *
 * - ::mlk_vfs::finish
 * - ::mlk_vfs::open
 *
 * The following VFS file member are used:
 *
 * - ::mlk_vfs_file::finish
 * - ::mlk_vfs_file::flush
 * - ::mlk_vfs_file::read
 * - ::mlk_vfs_file::write
 */

#include <mlk/util/util.h>

#include "vfs.h"

/**
 * \struct mlk_vfs_dir_file
 * \brief VFS file implementation for filesystem files.
 */
struct mlk_vfs_dir_file {
	/**
	 * (read-only)
	 *
	 * Path to the opened file.
	 */
	char path[MLK_PATH_MAX];

	/**
	 * (read-write)
	 *
	 * Abstract VFS file to implement.
	 */
	struct mlk_vfs_file file;

	/** \cond MLK_PRIVATE_DECLS */
	void *handle;
	/** \endcond MLK_PRIVATE_DECLS */
};

/**
 * \struct mlk_vfs_zip
 * \brief VFS implementation for filesystem directories.
 */
struct mlk_vfs_dir {
	/**
	 * (read-only)
	 *
	 * Path to the directory.
	 */
	char path[MLK_PATH_MAX];

	/**
	 * (read-write)
	 *
	 * Abstract VFS to implement.
	 */
	struct mlk_vfs vfs;
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Initialize the directory object and its underlying VFS module.
 *
 * \pre dir != NULL
 * \pre path != NULL
 * \param dir the dir implementation to initialize
 * \param path the path to the directory
 */
void
mlk_vfs_dir_init(struct mlk_vfs_dir *dir, const char *path);

/**
 * Implements ::mlk_vfs::open virtual function.
 */
struct mlk_vfs_file *
mlk_vfs_dir_open(struct mlk_vfs_dir *dir, const char *entry, const char *mode);

/**
 * Implements ::mlk_vfs::finish virtual function.
 */
void
mlk_vfs_dir_finish(struct mlk_vfs_dir *dir);

/**
 * Implements ::mlk_vfs_file::read virtual function.
 */
size_t
mlk_vfs_dir_file_read(struct mlk_vfs_dir_file *file, void *buf, size_t bufsz);

/**
 * Implements ::mlk_vfs_file::write virtual function.
 */
size_t
mlk_vfs_dir_file_write(struct mlk_vfs_dir_file *file, const void *buf, size_t bufsz);

/**
 * Implements ::mlk_vfs_file::flush virtual function.
 */
int
mlk_vfs_dir_file_flush(struct mlk_vfs_dir_file *file);

/**
 * Implements ::mlk_vfs_file::finish virtual function.
 */
void
mlk_vfs_dir_file_finish(struct mlk_vfs_dir_file *file);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_VFS_DIR_H */
