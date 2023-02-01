/*
 * align.h -- user interface alignment
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

#ifndef MLK_UI_ALIGN_H
#define MLK_UI_ALIGN_H

#include <mlk/core/core.h>

enum align {
	ALIGN_NONE,
	ALIGN_CENTER,
	ALIGN_TOP_LEFT,
	ALIGN_TOP,
	ALIGN_TOP_RIGHT,
	ALIGN_RIGHT,
	ALIGN_BOTTOM_RIGHT,
	ALIGN_BOTTOM,
	ALIGN_BOTTOM_LEFT,
	ALIGN_LEFT
};

CORE_BEGIN_DECLS

void
align(enum align,
      int *,
      int *,
      unsigned int,
      unsigned int,
      int,
      int,
      unsigned int,
      unsigned int);

CORE_END_DECLS

#endif /* !MLK_UI_ALIGN_H */
