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

#ifndef MOLKO_UI_CHECKBOX_H
#define MOLKO_UI_CHECKBOX_H

/**
 * \file checkbox.h
 * \brief GUI checkbox.
 * \ingroup ui
 */

#include <stdbool.h>

union event;

struct action;
struct theme;

/**
 * \brief GUI checkbox.
 */
struct checkbox {
	int x;                          /*!< (+) Position in x. */
	int y;                          /*!< (+) Position in y. */
	unsigned int w;                 /*!< (+) Width. */
	unsigned int h;                 /*!< (+) Height. */
	bool checked;                   /*!< (+) Is activated? */
	const struct theme *theme;      /*!< (+&?) Theme to use. */
};

/**
 * Default drawing function.
 *
 * \pre t != NULL
 * \pre cb != NULL
 * \param t the theme
 * \param cb the checkbox
 */
void
checkbox_draw_default(const struct theme *t, const struct checkbox *cb);

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

/**
 * Convert the checkbox into an action.
 *
 * The following field will be set into the action:
 *
 * - act->data: points to cb (reference),
 * - act->handle: a wrapper to checkbox_handle,
 * - act->draw: a wrapper to checkbox_draw.
 *
 * The checkbox being an UI element is considered to never completes, as such
 * you will need to handle this case or to use a custom update function.
 *
 * \note You will still need to check the checkbox state and reset it at some
 *       point.
 * \pre cb != NULL
 * \pre act != NULL
 * \param cb the checkbox to reference
 * \param act the action to fill
 */
void
checkbox_action(struct checkbox *cb, struct action *act);

#endif /* !MOLKO_UI_CHECKBOX_H */
