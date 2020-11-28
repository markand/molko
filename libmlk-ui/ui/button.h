/*
 * button.h -- GUI button
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

#ifndef MOLKO_UI_BUTTON_H
#define MOLKO_UI_BUTTON_H

/**
 * \file button.h
 * \brief GUI button.
 * \ingroup ui
 */

union event;

struct action;
struct theme;

/**
 * \brief Button state.
 */
enum button_state {
	BUTTON_STATE_NONE,              /*!< Button is inactive. */
	BUTTON_STATE_PRESSED,           /*!< Button is currently pressed. */
	BUTTON_STATE_ACTIVATED          /*!< Button is considered activated. */
};

/**
 * \brief GUI button.
 */
struct button {
	int x;                          /*!< (+) Position in x. */
	int y;                          /*!< (+) Position in y. */
	unsigned int w;                 /*!< (+) Width. */
	unsigned int h;                 /*!< (+) Height. */
	const char *text;               /*!< (+&) Text to draw. */
	enum button_state state;        /*!< (+) Button state. */
	const struct theme *theme;      /*!< (+&?) Theme to use. */
};

/**
 * Handle the event.
 *
 * You should always call this function even if the event is completely
 * unrelated.
 *
 * \pre button != NULL
 * \pre ev != NULL
 * \param button the button
 * \param ev the event
 */
void
button_handle(struct button *button, const union event *ev);

/**
 * Use this function once the button has been considered activated.
 *
 * \pre button != NULL
 * \param button the button
 */
void
button_reset(struct button *button);

/**
 * Default drawing function.
 *
 * \pre t != NULL
 * \pre frame != NULL
 * \param t the theme
 * \param button the button
 */
void
button_draw_default(const struct theme *t, const struct button *button);

/**
 * Draw the button.
 *
 * \pre button != NULL
 * \param button the button
 */
void
button_draw(const struct button *button);

/**
 * Convert the button into an action.
 *
 * The following field will be set into the action:
 *
 * - act->data: points to button (reference),
 * - act->handle: a wrapper to button_handle,
 * - act->draw: a wrapper to button_draw.
 *
 * The button being an UI element is considered to never completes, as such
 * you will need to handle this case or to use a custom update function.
 *
 * \note You will still need to check the button state and reset it at some
 *       point.
 * \pre button != NULL
 * \pre act != NULL
 * \param button the button to reference
 * \param act the action to fill
 */
void
button_action(struct button *button, struct action *act);

#endif /* !MOLKO_UI_BUTTON_H */
