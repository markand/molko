/*
 * label.h -- GUI label
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

#ifndef MOLKO_LABEL_H
#define MOLKO_LABEL_H

/**
 * \file label.h
 * \brief GUI label.
 */

struct theme;

/**
 * \brief Label flags.
 */
enum label_flags {
	LABEL_NONE,                             /*!< No flags. */
	LABEL_NO_SHADOW         = (1 << 0),     /*!< Disable shadow. */
	LABEL_NO_VCENTER        = (1 << 1),     /*!< Disable vertical centering. */
	LABEL_NO_HCENTER        = (1 << 2)      /*!< Disable horizontal centering. */
};

/**
 * \brief GUI label.
 */
struct label {
	int x;                  /*!< (RW) Position in x. */
	int y;                  /*!< (RW) Position in y. */
	unsigned int w;         /*!< (RW) Width. */
	unsigned int h;         /*!< (RW) Height. */
	const char *text;       /*!< (RW, ref) Text to show. */
	enum label_flags flags; /*!< (RW) Optional flags. */
	unsigned long color;    /*!< (RW) Color to use (0 = use theme). */
	struct theme *theme;    /*!< (RW, ref, optional) Theme to use. */
};

/**
 * Draw the label.
 *
 * \pre label != NULL
 * \param label the label to draw
 */
void
label_draw(const struct label *label);

#endif /* !MOLKO_LABEL_H */
