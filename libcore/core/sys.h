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

#ifndef MOLKO_SYS_H
#define MOLKO_SYS_H

/**
 * \file sys.h
 * \brief System routines.
 * \ingroup basics
 */

#include <stdarg.h>
#include <stdbool.h>

/**
 * Initialize the system, should be called in the beginning of the main.
 */
bool
sys_init(void);

/**
 * Get the base system directory path.
 *
 * \return the path where the executable lives
 */
const char *
sys_datadir(void);

/**
 * Construct path to assets directory using printf-like format.
 *
 * \param fmt the format string
 * \return the path to the file
 * \note This function returns pointer to static string.
 */
const char *
sys_datapath(const char *fmt, ...);

/**
 * Similar to \a sys_datapath.
 *
 * \param fmt the format string
 * \param ap the variadic arguments pointer
 * \return the path to the file
 * \note This function returns pointer to static string.
 */
const char *
sys_datapathv(const char *fmt, va_list ap);

/**
 * Compute the path to the save file for the given game state.
 *
 * \param idx the save number
 * \return the path to the database file
 * \note This only compute the path, it does not check the presence of the file
 * \post The returned value will never be NULL
 */
const char *
sys_savepath(unsigned int idx);

/**
 * Close the system.
 */
void
sys_finish(void);

#endif /* !MOLKO_SYS_H */
