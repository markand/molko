/*
 * frame.h -- GUI frame
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

#ifndef MOLKO_FRAME_H
#define MOLKO_FRAME_H

/**
 * \file frame.h
 * \brief GUI frame.
 */

struct theme;

/**
 * \brief Frame style.
 */
enum frame_style {
	FRAME_STYLE_NORMAL,
	FRAME_STYLE_BOX
};

/**
 * \brief GUI frame.
 */
struct frame {
	int x;                  /*!< (RW) Position in x. */
	int y;                  /*!< (RW) Position in y. */
	unsigned int w;         /*!< (RW) Width. */
	unsigned int h;         /*!< (RW) Height. */
	enum frame_style style; /*!< (RW) Frame style. */
	struct theme *theme;    /*!< (RW, ref, optional) Theme to use. */
};

/**
 * Draw the frame.
 *
 * \pre frame != NULL
 * \param frame the frame to draw
 */
void
frame_draw(const struct frame *frame);

#endif /* !MOLKO_FRAME_H */
