/*
 * core.h -- libmlk-core main entry
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

#ifndef MLK_CORE_CORE_H
#define MLK_CORE_CORE_H

/**
 * \file mlk/core/core.h
 * \brief libmlk-core main entry
 *
 * This is the main header for the libmlk-core file.
 */

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Initialize the libmlk-core library.
 *
 * The caller should specify an organization and a game name, they can be used
 * when retrieving home/special folder on some platforms.
 *
 * The organization is usually a reverse DNS name, e.g. com.example. The name
 * is just your game name.
 *
 * \pre organization != NULL
 * \pre name != NULL
 * \param organization the organization name
 * \param name the game name
 * \return 0 on success or any error code instead
 */
int
mlk_core_init(const char *organization, const char *name);

/**
 * Cleanup resources allocated by the library.
 */
void
mlk_core_finish(void);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_CORE_H */
