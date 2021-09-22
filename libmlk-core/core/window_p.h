/*
 * window.c -- (PRIVATE) basic window management
 *
 * Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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

#ifndef MOLKO_CORE_WINDOW_P_H
#define MOLKO_CORE_WINDOW_P_H

#include <SDL.h>

struct window_handle {
	SDL_Window *win;
	SDL_Renderer *renderer;
};

/* Convenient macros to access the native handle from global window object. */
#define WINDOW()        (((struct window_handle *)window.handle)->win)
#define RENDERER()      (((struct window_handle *)window.handle)->renderer)

#endif /* !MOLKO_CORE_WINDOW_P_H */
