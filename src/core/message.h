/*
 * message.h -- message dialog
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

#ifndef MOLKO_MESSAGE_H
#define MOLKO_MESSAGE_H

/**
 * \file message.h
 * \brief Message dialog.
 * \ingroup actions
 * \ingroup drawing
 *
 * This module's purpose is to show a dialog box into the screen to show text
 * and optionally ask the user a question.
 *
 * By itself, it is very low level and does not prevent other parts of the game
 * to use the input so you probably need to inhibit input if your dialog is
 * meant to be displayed on a map.
 *
 * To use it use the following procedure:
 *
 * 1. Create a struct message object and set required properties,
 * 2. Call \ref message_start to reset the state,
 * 3. Call \ref message_handle and \ref message_update with appropriate values,
 * 4. Call \ref message_draw to render the dialog.
 *
 * Depending on message flags or user input, step 3 may return true in this
 * case you should stop using the message as it has completed rendering.
 *
 * \note All properties must exist until the object is no longer used.
 *
 * \code
 * struct message msg = {
 * 	// You can show up to 6 lines.
 * 	.text = {
 * 		"Hello, what's up?"
 * 	},
 * 	// This image will be shown on the left as user face.
 * 	.avatar = mysuperavatar,
 * 	// This should point to a image that is used as background.
 * 	.frame = mysuperframe,
 * 	// The first color is normal text, the second is for selected text
 * 	// in case of question.
 * 	.colors = { 0xffffffff, 0x0000ffff },
 * 	// This indicates this message is a question.
 * 	.flags = MESSAGE_QUESTION
 * };
 * \endcode
 */

#include <stdbool.h>

#include "texture.h"

struct action;
struct font;
struct theme;

union event;

/**
 * \brief Message flags.
 */
enum message_flags {
	MESSAGE_AUTOMATIC       = (1 << 0),     /*!< Will automatically change state by itself. */
	MESSAGE_QUESTION        = (1 << 1),     /*!< The message is a question. */
	MESSAGE_QUICK           = (1 << 2),     /*!< Avoid animations. */
};

/**
 * \brief Message state.
 */
enum message_state {
	MESSAGE_NONE,           /*!< Message hasn't start yet or is finished */
	MESSAGE_OPENING,        /*!< Message animation is opening */
	MESSAGE_SHOWING,        /*!< Message is displaying */
	MESSAGE_HIDING          /*!< Message animation for hiding */
};

/**
 * \brief Message object.
 *
 * This structure is used to display a message into the screen. It does not own
 * any user properties and therefore must exist while using it.
 */
struct message {
	const char *text[6];            /*!< (RW) Lines of text to show. */
	struct texture *frame;          /*!< (RW, ref) Frame to use. */
	struct texture *avatar;         /*!< (RW, ref, optional) Avatar face. */
	unsigned int index;             /*!< (RW) Line selected */
	enum message_flags flags;       /*!< (RW) Message flags */
	enum message_state state;       /*!< (RO) Current state */
	struct theme *theme;            /*!< (RW, ref, optional) Theme to use. */
	unsigned int elapsed;           /*!< (RO) Time elapsed. */
};

/**
 * Start opening the message. This function will reset the message state and
 * elapsed time.
 *
 * \pre msg != NULL
 * \param msg the message
 */
void
message_start(struct message *msg);

/**
 * Handle input events.
 *
 * This function will alter state of the message and change its selection in
 * case of question.
 *
 * \pre msg != NULL
 * \pre ev != NULL
 * \param msg the message
 * \param ev the event which occured
 */
void
message_handle(struct message *msg, const union event *ev);

/**
 * Update the message state and elapsed time..
 *
 * \pre msg != NULL
 * \param msg the message
 * \param ticks the elapsed delay since last frame
 * \return true if it has finished
 */
bool
message_update(struct message *msg, unsigned int ticks);

/**
 * Draw the message into the screen.
 *
 * \pre msg != NULL
 * \param msg the message
 */
void
message_draw(struct message *msg);

/**
 * Start hiding the message.
 *
 * \pre msg != NULL
 * \param msg the message
 * \note You should still continue to draw the message as the animation is not
 *       finished!
 */
void
message_hide(struct message *msg);

/**
 * Convert message into an action.
 *
 * \pre msg != NULL
 * \pre action != NULL
 * \param msg the message to copy from
 * \param action the action to fill
 */
void
message_action(const struct message *msg, struct action *action);

#endif /* !MOLKO_MESSAGE_H */
