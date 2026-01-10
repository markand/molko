/*
 * vfs-zip.h -- VFS subsystem for zip archives
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

#ifndef MLK_CORE_VFS_ZIP_H
#define MLK_CORE_VFS_ZIP_H

/**
 * \file mlk/core/vfs-zip.h
 * \brief VFS subsystem for zip archives.
 *
 * This module can be used to read file from a ZIP archives using the
 * mlk/core/vfs.h abstract VFS module.
 *
 * It is implemented using the ::MLK_UTIL_CONTAINER_OF macro which means you can
 * use it and derive from it to add or modify its functions.
 *
 * \note It currently supports reading files but not writing.
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
 * - ::mlk_vfs_file::read
 */

#include "sysconfig.h"
#include "vfs.h"

#if defined(MLK_WITH_ZIP)

/**
 * \struct mlk_vfs_zip_file
 * \brief VFS file implementation for ZIP files.
 */
struct mlk_vfs_zip_file {
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
 * \brief VFS implementation for ZIP files.
 */
struct mlk_vfs_zip {
	/**
	 * (read-write)
	 *
	 * Abstract VFS to implement.
	 */
	struct mlk_vfs vfs;

	/** \cond MLK_PRIVATE_DECLS */
	void *handle;
	/** \endcond MLK_PRIVATE_DECLS */
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Initialize the ZIP object and its underlying VFS module.
 *
 * \pre zip != NULL
 * \pre file != NULL
 * \pre mode != NULL
 * \param zip the zip implementation to initialize
 * \param path the path to the ZIP file
 * \param mode the open mode
 * \return 0 on success or -1 on error
 */
int
mlk_vfs_zip_init(struct mlk_vfs_zip *zip, const char *path, const char *mode);

/**
 * Implements ::mlk_vfs::open virtual function.
 */
struct mlk_vfs_file *
mlk_vfs_zip_open(struct mlk_vfs_zip *zip, const char *entry, const char *mode);

/**
 * Implements ::mlk_vfs::open virtual function.
 */
void
mlk_vfs_zip_finish(struct mlk_vfs_zip *zip);

/**
 * Implements ::mlk_vfs_file::read virtual function.
 */
size_t
mlk_vfs_zip_file_read(struct mlk_vfs_zip_file *file, void *buf, size_t bufsz);

/**
 * Implements ::mlk_vfs_file::finish virtual function.
 */
void
mlk_vfs_zip_file_finish(struct mlk_vfs_zip_file *file);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_WITH_ZIP */

#endif /* !MLK_CORE_VFS_ZIP_H */
