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

#ifndef MOLKO_UI_FRAME_H
#define MOLKO_UI_FRAME_H

/**
 * \file frame.h
 * \brief GUI frame.
 * \ingroup ui
 */

#include <core/core.h>

struct action;
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
	int x;                          /*!< (+) Position in x. */
	int y;                          /*!< (+) Position in y. */
	unsigned int w;                 /*!< (+) Width. */
	unsigned int h;                 /*!< (+) Height. */
	enum frame_style style;         /*!< (+) Frame style. */
	const struct theme *theme;      /*!< (+&?) Theme to use. */
};

CORE_BEGIN_DECLS

/**
 * Default drawing function.
 *
 * \pre t != NULL
 * \pre frame != NULL
 * \param t the theme
 * \param frame the frame
 */
void
frame_draw_default(const struct theme *t, const struct frame *frame);

/**
 * Draw the frame.
 *
 * \pre frame != NULL
 * \param frame the frame to draw
 */
void
frame_draw(const struct frame *frame);

/**
 * Convert the frame into an action.
 *
 * The following field will be set into the action:
 *
 * - act->data: points to frame (reference),
 * - act->draw: a wrapper to checkbox_draw.
 *
 * The frame being an UI element is considered to never completes, as such
 * you will need to handle this case or to use a custom update function.
 *
 * \pre frame != NULL
 * \pre act != NULL
 * \param frame the frame to reference
 * \param act the action to fill
 */
void
frame_action(struct frame *frame, struct action *act);

CORE_END_DECLS

#endif /* !MOLKO_UI_FRAME_H */
