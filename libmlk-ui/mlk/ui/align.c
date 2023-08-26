/*
 * align.c -- user interface alignment helpers
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

#include "align.h"

#define SET(p, v) if (p) *p = v
#define CENTER(p, c, rs, es) if (p) (*p) = c + (rs / 2) - (es / 2)

void
mlk_align(enum mlk_align align,
          int *x,
          int *y,
          unsigned int w,
          unsigned int h,
          int px,
          int py,
          unsigned int pw,
          unsigned int ph)
{
	switch (align) {
	case MLK_ALIGN_CENTER:
		CENTER(x, px, pw, w);
		CENTER(y, py, ph, h);
		break;
	case MLK_ALIGN_TOP_LEFT:
		SET(x, px);
		SET(y, py);
		break;
	case MLK_ALIGN_TOP:
		CENTER(x, px, pw, w);
		SET(y, py);
		break;
	case MLK_ALIGN_TOP_RIGHT:
		SET(x, px + pw - w);
		SET(y, py);
		break;
	case MLK_ALIGN_RIGHT:
		SET(x, px + pw - w);
		CENTER(y, py, ph, h);
		break;
	case MLK_ALIGN_BOTTOM_RIGHT:
		SET(x, px + pw - w);
		SET(y, py + ph - h);
		break;
	case MLK_ALIGN_BOTTOM:
		CENTER(x, px, pw, w);
		SET(y, py + ph - h);
		break;
	case MLK_ALIGN_BOTTOM_LEFT:
		SET(x, px);
		SET(y, py + ph - h);
		break;
	case MLK_ALIGN_LEFT:
		CENTER(y, py, ph, h);
		SET(x, px);
		break;
	default:
		SET(x, px);
		SET(y, py);
		break;
	}
}
