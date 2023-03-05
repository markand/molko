/*
 * rpg.h -- libmlk-rpg main entry
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

#ifndef MLK_RPG_RPG_H
#define MLK_RPG_RPG_H

/**
 * \file mlk/rpg/rpg.h
 * \brief libmlk-rpg main entry
 */

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Initialize the RPG library.
 *
 * \return 0 on success or any error code instead
 */
int
mlk_rpg_init(void);

/**
 * Cleanup resources allocated by the library.
 */
void
mlk_rpg_finish(void);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_RPG_H */
