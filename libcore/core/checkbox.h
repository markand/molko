/*
 * checkbox.h -- GUI checkbox
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

#ifndef MOLKO_CHECKBOX_H
#define MOLKO_CHECKBOX_H

/**
 * \file checkbox.h
 * \brief GUI checkbox.
 */

#include <stdbool.h>

union event;

/**
 * \brief GUI checkbox.
 */
struct checkbox {
	int x;                  /*!< (RW) Position in x. */
	int y;                  /*!< (RW) Position in y. */
	unsigned int w;         /*!< (RW) Width. */
	unsigned int h;         /*!< (RW) Height. */
	const char *label;      /*!< (RW, ref) Text to show. */
	bool checked;           /*!< (RW) Is activated? */
	struct theme *theme;    /*!< (RW, ref, optional) Theme to use. */
};

/**
 * Draw the checkbox.
 *
 * \pre cb != NULL
 * \pre ev != NULL
 * \param cb the checkbox
 * \param ev the event
 */
void
checkbox_handle(struct checkbox *cb, const union event *ev);

/**
 * Draw the checkbox.
 *
 * \pre cb != NULL
 * \param cb the checkbox
 */
void
checkbox_draw(const struct checkbox *cb);

#endif /* !MOLKO_CHECKBOX_H */
