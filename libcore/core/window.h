/*
 * window.h -- basic window management
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

#ifndef MOLKO_WINDOW_H
#define MOLKO_WINDOW_H

/**
 * \file window.h
 * \brief Basic window management.
 * \ingroup drawing
 */

#include <stdbool.h>

/**
 * \brief Global exposed window structure.
 */
struct window {
	unsigned int w;         /*!< (RO) Window width. */
	unsigned int h;         /*!< (RO) Window height. */
	void *handle;           /*!< (RO) Native handle. */
};

/**
 * \brief Access to global window structure.
 */
extern struct window window;

/**
 * Initialize window.
 *
 * \pre title != NULL
 * \param title the window title
 * \param width the desired width
 * \param height the desired height
 * \return true on success
 */
bool
window_init(const char *title, unsigned int width, unsigned int height);

/**
 * Close the window and destroy associated resources.
 */
void
window_finish(void);

#endif /* !MOLKO_WINDOW_H */