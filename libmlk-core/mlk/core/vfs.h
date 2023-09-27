/*
 * vfs.h -- virtual file system abstraction
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

#ifndef MLK_CORE_VFS_H
#define MLK_CORE_VFS_H

/**
 * \file mlk/core/vfs.h
 * \brief Virtual file system abstraction.
 *
 * This module offers an abstract interface to load files and perform read/write
 * operation on them. This can be useful for games that are designed to load
 * large assets from compressed archives.
 *
 * The molko frameworks comes with two implementations:
 *
 * | module             | support     | remarks                            |
 * |--------------------|-------------|------------------------------------|
 * | mlk/core/vfs-dir.h | read, write | opens file relative to a directory |
 * | mlk/core/vfs-zip.h | read        | zip archive files extractor        |
 *
 * ## Initialize a VFS
 *
 * To use this module, you must first open a VFS interface. It is usually implemented
 * using the ::MLK_CONTAINER_OF macro.
 *
 * Example with mlk/core/vfs-dir.h
 *
 * ```c
 * struct mlk_vfs_dir dir;
 *
 * mlk_vfs_directory_init(&dir, "/usr/share/mario");
 * ```
 *
 * The abstract interface will be located in the `vfs` field for each
 * implementation.
 *
 * ## Opening files
 *
 * Once your VFS module is initialized, you can call ::mlk_vfs_open to load an
 * entry from it:
 *
 * ```c
 * struct mlk_vfs_file *file;
 * char content[1024];
 * size_t len;
 *
 * file = mlk_vfs_open(vfs, "block.png");
 *
 * if (!file)
 *     handle_failure();
 *
 * // The function does not append a NUL terminator.
 * len = mlk_vfs_file_read(file, content, sizeof (content) - 1);
 *
 * if (len == (size_t)-1)
 *     handle_failure();
 *
 * // Use content freely...
 * ```
 *
 * ## Cleaning up resources
 *
 * Opened files SHOULD be destroyed before the VFS itself because depending on
 * the underlying implementation, it is possible that those opened files have
 * direct references to the VFS module.
 *
 * ```c
 * mlk_vfs_file_free(file);
 * mlk_vfs_finish(&dir.vfs);
 * ```
 */

#include <stddef.h>

struct mlk_vfs_file;

/**
 * \struct mlk_vfs
 * \brief Abstract VFS loader.
 */
struct mlk_vfs {
	/**
	 * (read-write, borrowed, optional)
	 *
	 * Arbitrary user data for callbacks.
	 */
	void *data;

	/**
	 * (read-write)
	 *
	 * Open the file from the VFS.
	 *
	 * The mode argument must contain the following letters:
	 *
	 * - `+`: truncate file if exists
	 * - `r`: open for reading
	 * - `w`: open for writing
	 * - `x`: open exclusive mode
	 *
	 * \pre self != NULL
	 * \pre entry != NULL
	 * \pre mode != NULL
	 * \param self this vfs
	 * \param entry the entry filename
	 * \param mode open mode as described above
	 * \return a VFS file or NULL on failure
	 */
	struct mlk_vfs_file * (*open)(struct mlk_vfs *self,
	                              const char *entry,
	                              const char *mode);

	/**
	 * (read-write)
	 *
	 * Cleanup resources.
	 *
	 * \pre self != NULL
	 * \param self this vfs
	 */
	void (*finish)(struct mlk_vfs *self);
};

/**
 * \struct mlk_vfs_file
 * \brief Abstract VFS file.
 */
struct mlk_vfs_file {
	/**
	 * (read-write, borrowed, optional)
	 *
	 * Arbitrary user data for callbacks.
	 */
	void *data;

	/**
	 * (read-write, optional)
	 *
	 * Attempt to read file entry into the buffer destination. The function
	 * can read less bytes than requested.
	 *
	 * If the function is NULL, an error is returned with an error string
	 * telling that the operation is not supported.
	 *
	 * \pre self != NULL
	 * \pre buf != NULL
	 * \param self this VFS file
	 * \param buf the buffer destination
	 * \param bufsz maximum buffer size to read
	 * \return the number of bytes read or -1 on error
	 */
	size_t (*read)(struct mlk_vfs_file *self, void *buf, size_t bufsz);

	/**
	 * (read-write, optional)
	 *
	 * Attempt to write file entry from the buffer source. The function can
	 * write less bytes than requested.
	 *
	 * If the function is NULL, an error is returned with an error string
	 * telling that the operation is not supported.
	 *
	 * \pre self != NULL
	 * \pre buf != NULL
	 * \param self this VFS file
	 * \param buf the buffer source
	 * \param bufsz the buffer length
	 * \return the number of bytes written or -1 on error
	 */
	size_t (*write)(struct mlk_vfs_file *self, const void *buf, size_t bufsz);

	/**
	 * (read-write, optional)
	 *
	 * Attempt to flush pending input/output on the underlying interface.
	 *
	 * If the function is NULL, it is considered success.
	 *
	 * \pre self != NULL
	 * \param self this VFS file
	 * \return 0 on success or -1 on error
	 */
	int (*flush)(struct mlk_vfs_file *self);

	/**
	 * (read-write, optional)
	 *
	 * Cleanup resources for this VFS file.
	 *
	 * \pre self != NULL
	 * \param self this VFS file
	 */
	void (*free)(struct mlk_vfs_file *self);
};

#if defined(__cplusplus)
extern "C"
#endif

/**
 * Wrapper of ::mlk_vfs::open if not NULL.
 */
struct mlk_vfs_file *
mlk_vfs_open(struct mlk_vfs *vfs,
             const char *entry,
             const char *mode);

/**
 * Wrapper of ::mlk_vfs::finish if not NULL.
 */
void
mlk_vfs_finish(struct mlk_vfs *vfs);

/**
 * Wrapper of ::mlk_vfs_file::read if not NULL.
 */
size_t
mlk_vfs_file_read(struct mlk_vfs_file *file, void *buf, size_t bufsz);

/**
 * Convenient function to read an entire file content using repeated calls to
 * ::mlk_vfs_file_read function until end of file is reached.
 *
 * The returned string is dynamically allocated and must be free'd using
 * ::mlk_alloc_free function.
 *
 * \pre file != NULL
 * \param file this VFS file
 * \param len pointer receiving the number of bytes read (can be NULL)
 * \return the string content
 */
char *
mlk_vfs_file_aread(struct mlk_vfs_file *file, size_t *len);

/**
 * Wrapper of ::mlk_vfs::finish if not NULL.
 */
size_t
mlk_vfs_file_write(struct mlk_vfs_file *file, void *buf, size_t bufsz);

/**
 * Wrapper of ::mlk_vfs::finish if not NULL.
 */
int
mlk_vfs_file_flush(struct mlk_vfs_file *file);

/**
 * Wrapper of ::mlk_vfs::finish if not NULL.
 */
void
mlk_vfs_file_free(struct mlk_vfs_file *file);

#if defined(__cplusplus)
}
#endif

#endif /* MLK_CORE_VFS_H */
