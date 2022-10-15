/*
 * window.h -- basic window management
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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

#ifndef MLK_CORE_WINDOW_H
#define MLK_CORE_WINDOW_H

#include "core.h"

struct window {
	unsigned int w;
	unsigned int h;
	unsigned int framerate;
	void *handle;
};

enum window_cursor {
	WINDOW_CURSOR_ARROW,
	WINDOW_CURSOR_EDIT,
	WINDOW_CURSOR_WAIT,
	WINDOW_CURSOR_CROSSHAIR,
	WINDOW_CURSOR_SIZE,
	WINDOW_CURSOR_NO,
	WINDOW_CURSOR_HAND,
	WINDOW_CURSOR_LAST
};

extern struct window window;

CORE_BEGIN_DECLS

int
window_open(const char *, unsigned int, unsigned int);

void
window_set_cursor(enum window_cursor);

void
window_finish(void);

CORE_END_DECLS

#endif /* !MLK_CORE_WINDOW_H */
