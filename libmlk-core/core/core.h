/*
 * core.c -- libcore main entry
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

#ifndef MOLKO_CORE_CORE_H
#define MOLKO_CORE_CORE_H

#include <stdbool.h>

#if defined(__cplusplus)
#       define CORE_BEGIN_DECLS extern "C" {
#       define CORE_END_DECLS   }
#else
#       define CORE_BEGIN_DECLS
#       define CORE_END_DECLS
#endif

CORE_BEGIN_DECLS

bool
core_init(const char *organization, const char *name);

void
core_finish(void);

CORE_END_DECLS

#endif /* !MOLKO_CORE_CORE_H */
