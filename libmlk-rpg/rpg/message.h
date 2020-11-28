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

#ifndef MOLKO_RPG_MESSAGE_H
#define MOLKO_RPG_MESSAGE_H

/**
 * \file message.h
 * \brief Message dialog.
 * \ingroup actions
 * \ingroup drawing
 *
 * This module's purpose is to show a dialog box into the screen to show text
 * and optionally ask the user a question. It is similar to what you're used to
 * see in many RPGs.
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
 * 	// This indicates this message is a question.
 * 	.flags = MESSAGE_FLAGS_QUESTION
 * };
 * \endcode
 *
 * For performance reasons, flexibility and simplicity, the message box does
 * not try to be clever about positions of lines. It will simply create an
 * animation for opening the box, drawing the lines to the position according
 * to the theme padding and spacing and interact with user. For convenience
 * though, the \ref message_query can be used to determine dimensions required
 * for a better final result.
 *
 * ## Example, computing the dimensions:
 *
 * \code
 * // We create a message that we put on the center of the screen.
 * struct message msg = {
 *     .text = {
 *         "Hi, have you tried turning it off and on again?"
 *     }
 * };
 *
 * message_query(&msg, &msg.w, &msg.h);
 * align(ALIGN_CENTER, &msg.x, &msg.y, msg.w, msg.h, 0, 0, window.w, window.h);
 * \endcode
 */

#include <stdbool.h>

#include <core/texture.h>

struct action;
struct font;
struct theme;

union event;

/**
 * \brief Default animation speed in milliseconds.
 */
#define MESSAGE_DELAY_DEFAULT   (150)

/**
 * \brief Default timeout in milliseconds for automatic messages.
 */
#define MESSAGE_TIMEOUT_DEFAULT (5000)

/**
 * \brief Maximum number of lines allowed in the message.
 */
#define MESSAGE_LINES_MAX       (3)

/**
 * \brief Message flags.
 */
enum message_flags {
	MESSAGE_FLAGS_AUTOMATIC         = (1 << 0),     /*!< Will automatically change state by itself. */
	MESSAGE_FLAGS_QUESTION          = (1 << 1),     /*!< The message is a question. */
	MESSAGE_FLAGS_FADEIN            = (1 << 2),     /*!< Animate opening. */
	MESSAGE_FLAGS_FADEOUT           = (1 << 3)      /*!< Animate closing. */
};

/**
 * \brief Message state.
 */
enum message_state {
	MESSAGE_STATE_NONE,             /*!< Message hasn't start yet or is finished */
	MESSAGE_STATE_OPENING,          /*!< Message animation is opening */
	MESSAGE_STATE_SHOWING,          /*!< Message is displaying */
	MESSAGE_STATE_HIDING            /*!< Message animation for hiding */
};

/**
 * \brief Message object.
 */
struct message {
	int x;                                  /*!< (+) Position in x. */
	int y;                                  /*!< (+) Position in y. */
	unsigned int w;                         /*!< (+) Width. */
	unsigned int h;                         /*!< (+) Height. */
	unsigned int spacing;                   /*!< (+) Spacing between lines. */
	unsigned int delay;                     /*!< (+) Delay for animations. */
	unsigned int timeout;                   /*!< (+) Timeout in milliseconds. */
	const char *text[MESSAGE_LINES_MAX];    /*!< (+) Lines of text to show. */
	unsigned int index;                     /*!< (+) Line selected */
	enum message_flags flags;               /*!< (+) Message flags */
	enum message_state state;               /*!< (-) Current state */
	const struct theme *theme;              /*!< (+&?) Theme to use. */
	unsigned int elapsed;                   /*!< (-) Time elapsed. */
	double scale;                           /*!< (-) Current scale [0-1]. */
};

/**
 * Start opening the message. This function will reset the message state and
 * elapsed time.
 *
 * \pre msg != NULL
 * \pre msg->delay > 0 if msg->flags contains MESSAGE_FLAGS_FADEIN or
 *      MESSAGE_FLAGS_FADEOUT
 * \param msg the message
 */
void
message_start(struct message *msg);

/**
 * Compute the minimal message dimensions required.
 *
 * \pre msg != NULL
 * \param msg the message to query
 * \param w the pointer to width (may be NULL)
 * \param h the pointer to height (may be NULL)
 */
void
message_query(const struct message *msg, unsigned int *w, unsigned int *h);

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
message_draw(const struct message *msg);

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
 * \pre act != NULL
 * \param msg the message to reference
 * \param act the action to fill
 * \post act->data contains msg
 * \post act->handle invokes message_handle
 * \post act->update invokes message_update
 * \post act->draw invokes message_draw
 */
void
message_action(struct message *msg, struct action *act);

#endif /* !MOLKO_RPG_MESSAGE_H */
