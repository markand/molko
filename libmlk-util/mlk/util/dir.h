/*
 * dir.h -- portable directory iterator
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

#ifndef MLK_UTIL_DIR_H
#define MLK_UTIL_DIR_H

/**
 * \file dir.h
 * \brief Portable directory iterator
 *
 * This module provides a convenient portable directory iterator.
 *
 * How to use:
 *
 * 1. Use ::mlk_dir_open on your directory.
 * 2. Call ::mlk_dir_next in a loop.
 *
 * The iterator is destroyed when the last iterator value has been read
 * otherwise ::mlk_dir_finish can be used in case the loop was terminated early
 *
 * ```c
 * struct mlk_dir iter;
 *
 * if (mlk_dir_open(&iter, "/tmp") < 0) {
 *     fprintf(stderr, "unable to open the directory\n");
 *     return -1;
 * }
 *
 * while (mlk_dir_next(&iter)) {
 *     printf("-> %s\n", iter.entry);
 * }
 *
 * // At this step the directory iterator is already destroyed for convenience.
 * ```
 */

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * \struct mlk_dir
 * \brief Directory iterator structure
 */
struct mlk_dir {
	/**
	 * (read-only, borrowed)
	 *
	 * Relative directory file entry name.
	 */
	const char *entry;

	/** \cond MLK_PRIVATE_DECLS */
	void *handle;
	/** \endcond MLK_PRIVATE_DECLS */
};

/**
 * Open the directory iterator specified by path.
 *
 * \pre iter != NULL
 * \pre path != NULL
 * \param iter the iterator to initialize
 * \param path the path to the directory
 * \return 0 on success or -1 on error
 */
int
mlk_dir_open(struct mlk_dir *iter, const char *path);

/**
 * Read the next directory entry.
 *
 * \pre iter != NULL
 * \param iter the directory iterator
 * \return 1 on read or 0 on EOF
 */
int
mlk_dir_next(struct mlk_dir *iter);

/**
 * Cleanup directory iterator resources.
 *
 * This function is only required if you didn't complete the loop using
 * ::mlk_dir_next function.
 *
 * \pre iter != NULL
 * \param iter the directory iterator
 */
void
mlk_dir_finish(struct mlk_dir *iter);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_UTIL_DIR_H */
