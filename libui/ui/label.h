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
	LABEL_FLAGS_NONE,                       /*!< No flags. */
	LABEL_FLAGS_SHADOW      = (1 << 0),     /*!< Enable shadow. */
};

/**
 * \brief Label alignment in bounding box.
 *
 * The alignment is described as following:
 *
 * ```
 * +---------------------+
 * | 1        2        3 |
 * |                     |
 * | 8        0        4 |
 * |                     |
 * | 7        6        5 |
 * +---------------------+
 * ```
 */
enum label_align {
	LABEL_ALIGN_CENTER,             /*!< Align to the center (default). */
	LABEL_ALIGN_TOP_LEFT,           /*!< Top left. */
	LABEL_ALIGN_TOP,                /*!< Top center (aligned horizontally). */
	LABEL_ALIGN_TOP_RIGHT,          /*!< Top right. */
	LABEL_ALIGN_RIGHT,              /*!< Right (aligned vertically). */
	LABEL_ALIGN_BOTTOM_RIGHT,       /*!< Bottom right. */
	LABEL_ALIGN_BOTTOM,             /*!< Bottom (aligned horizontally). */
	LABEL_ALIGN_BOTTOM_LEFT,        /*!< Bottom left. */
	LABEL_ALIGN_LEFT                /*!< Left (aligned vertically). */
};

/**
 * \brief GUI label.
 */
struct label {
	int x;                          /*!< (+) Position in x. */
	int y;                          /*!< (+) Position in y. */
	unsigned int w;                 /*!< (+) Width. */
	unsigned int h;                 /*!< (+) Height. */
	const char *text;               /*!< (+&) Text to show. */
	enum label_flags flags;         /*!< (+) Optional flags. */
	enum label_align align;         /*!< (+) How to positionate label. */
	struct theme *theme;            /*!< (+&?) Theme to use. */
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
