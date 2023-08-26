/*
 * align.h -- user interface alignment helpers
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

#ifndef MLK_UI_ALIGN_H
#define MLK_UI_ALIGN_H

/**
 * \file mlk/ui/align.h
 * \brief User interface alignment helpers
 */

/**
 * \enum mlk_align
 * \brief Alignment selector
 */
enum mlk_align {
	/**
	 * No alignment, set coordinates to the parent coordinates.
	 */
	MLK_ALIGN_NONE,

	/**
	 * Align to the center.
	 */
	MLK_ALIGN_CENTER,

	/**
	 * Top left corner.
	 */
	MLK_ALIGN_TOP_LEFT,

	/**
	 * Top center.
	 */
	MLK_ALIGN_TOP,

	/**
	 * Top right corner.
	 */
	MLK_ALIGN_TOP_RIGHT,

	/**
	 * Right center (vertical).
	 */
	MLK_ALIGN_RIGHT,

	/**
	 * Bottom right corner.
	 */
	MLK_ALIGN_BOTTOM_RIGHT,

	/**
	 * Bottom center.
	 */
	MLK_ALIGN_BOTTOM,

	/**
	 * Bottom left corner.
	 */
	MLK_ALIGN_BOTTOM_LEFT,
	
	/**
	 * Left center (vertical).
	 */
	MLK_ALIGN_LEFT
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Adjust x, y coordinates according to the object dimensions inside the
 * bounding parent box.
 *
 * \param align alignment selector
 * \param x x coordinate position to adjust (can be NULL)
 * \param y y coordinate position to adjust (can be NULL)
 * \param w the object width
 * \param h the object height
 * \param px the parent rectangle x coordinate
 * \param py the parent rectangle y coordinate
 * \param pw the parent rectangle width
 * \param ph the parent rectangle height
 */
void
mlk_align(enum mlk_align align,
          int *x,
          int *y,
          unsigned int w,
          unsigned int h,
          int px,
          int py,
          unsigned int pw,
          unsigned int ph);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_UI_ALIGN_H */
