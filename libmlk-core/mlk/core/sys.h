/*
 * sys.h -- system routines
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

#ifndef MLK_CORE_SYS_H
#define MLK_CORE_SYS_H

#include <stdarg.h>

enum mlk_sys_dir {
	MLK_SYS_DIR_SAVE
};

#if defined(__cplusplus)
extern "C" {
#endif

int
mlk_sys_init(const char *, const char *);

const char *
mlk_sys_dir(enum mlk_sys_dir);

int
mlk_sys_mkdir(const char *);

void
mlk_sys_finish(void);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_SYS_H */
