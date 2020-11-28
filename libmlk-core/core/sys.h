/*
 * sys.h -- system routines
 *
 * Copyright (c) 2020 David Demelier <markand@malikania.fr>
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

#ifndef MOLKO_CORE_SYS_H
#define MOLKO_CORE_SYS_H

/**
 * \file sys.h
 * \brief System routines.
 * \ingroup basics
 */

#include <stdarg.h>
#include <stdbool.h>

/**
 * \brief Kind of special directories.
 */
enum sys_dir {
	SYS_DIR_BIN,            /*!< Path to binaries. */
	SYS_DIR_DATA,           /*!< Directory containing data. */
	SYS_DIR_LOCALE,         /*!< Path to NLS catalogs. */
	SYS_DIR_SAVE,           /*!< User directory for save databases. */
};

/**
 * Initialize the system.
 *
 * This function is automatically called from \ref core_init and thus not
 * necessary from user.
 *
 * \pre organization != NULL
 * \pre name != NULL
 * \param organization the name of the organization
 * \param name the game name
 * \return False on error.
 */
bool
sys_init(const char *organization, const char *name);

/**
 * Get a system or user directory preferred for this platform.
 *
 * \pre kind must be valid
 * \param kind kind of special directory
 * \return A non-NULL pointer to a static storage path.
 */
const char *
sys_dir(enum sys_dir kind);

/**
 * Close the system.
 *
 * This function is automatically called from \ref core_finish and thus not
 * necessary from user.
 */
void
sys_finish(void);

#endif /* !MOLKO_CORE_SYS_H */
