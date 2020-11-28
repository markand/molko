/*
 * mouse.h -- mouse definitions
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

#ifndef MOLKO_CORE_MOUSE_H
#define MOLKO_CORE_MOUSE_H

/**
 * \file mouse.h
 * \brief Mouse definitions.
 * \ingroup input
 */

/**
 * \brief Buttons from mouse.
 *
 * This enumeration is used as both flags or constants. For example when the
 * user press one button on the mouse it generates one constant event. On the
 * other hand, while moving the mouse the user may have one or more buttons
 * pressed, thus the OR'ed combination.
 */
enum mouse_button {
	MOUSE_BUTTON_UNKNOWN    = 0,            /*!< No buttons pressed */
	MOUSE_BUTTON_LEFT       = (1 << 0),     /*!< Left button pressed */
	MOUSE_BUTTON_MIDDLE     = (1 << 1),     /*!< Middle button pressed */
	MOUSE_BUTTON_RIGHT      = (1 << 2)      /*!< Right button pressed */
};

#endif /* !MOLKO_CORE_MOUSE_H */
