/*
 * splashscreen_state.h -- splash screen state
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

#ifndef MOLKO_SPLASHSCREEN_ADVENTURE_H
#define MOLKO_SPLASHSCREEN_ADVENTURE_H

/**
 * \file splashscreen_state.h
 * \brief Splash screen state.
 */

#include <core/texture.h>

/**
 * \brief Data for splashscreen.
 */
extern struct splashscreen_state_data {
	struct texture text;            /*!< (RW) Texture for the text. */
	int x;                          /*!< (RW) Position in x. */
	int y;                          /*!< (RW) Position in y. */
	unsigned int elapsed;           /*!< (RW) Time elapsed. */
} splashscreen_state_data;              /*!< (RW) Global state data. */

/**
 * \brief Splash screen state.
 */
extern struct state splashscreen_state;

#endif /* !MOLKO_SPLASHSCREEN_ADVENTURE_H */
