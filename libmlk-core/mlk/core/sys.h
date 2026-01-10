/*
 * sys.h -- system routines
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

#ifndef MLK_CORE_SYS_H
#define MLK_CORE_SYS_H

/**
 * \file mlk/core/sys.h
 * \brief System routines
 */

#include <stdarg.h>

/**
 * \enum mlk_sys_dir
 * \brief Special directory constants
 */
enum mlk_sys_dir {
	/**
	 * Path to the preferred save directory.
	 */
	MLK_SYS_DIR_SAVE,

	/**
	 * Path to NLS catalogs.
	 */
	MLK_SYS_DIR_LOCALES,

	/**
	 * Unused sentinel value.
	 */
	MLK_SYS_DIR_LAST
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Initialize system, takes same arguments as ::mlk_core_init.
 *
 * \note This function is already called by ::mlk_core_init and usually not
 *       required by the user.
 */
int
mlk_sys_init(const char *organization, const char *name);

/**
 * Obtain a path for the given special directory.
 *
 * \param directory directory type
 * \return a path to a thread-local static array
 */
const char *
mlk_sys_dir(enum mlk_sys_dir directory);

/**
 * Try to create a directory recursively specified by the path.
 *
 * \pre path != NULL
 * \param path the path to the directory
 * \return 0 on success or an error code on failure
 */
int
mlk_sys_mkdir(const char *path);

/**
 * Cleanup system resources.
 *
 * \note This function is already called by ::mlk_core_finish and usually not
 *       required by the user.
 */
void
mlk_sys_finish(void);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_SYS_H */
