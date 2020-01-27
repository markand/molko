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
 */

#include <stdbool.h>

struct texture;
struct font;

union event;

/**
 * \brief Message flags.
 */
enum message_flags {
	MESSAGE_AUTOMATIC       = (1 << 0),     /*!< Will automatically change state by itself. */
	MESSAGE_QUESTION        = (1 << 1)      /*!< The message is a question. */
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
	const char *text[6];            /*!< (RW) Lines of text to show */
	struct texture *frame;          /*!< (RW) Frame to use */
	struct texture *avatar;         /*!< (RW) Optional avatar */
	struct font *font;              /*!< (RW) Font to use */
	unsigned long colors[2];        /*!< (RW) Normal/selected colors */
	unsigned int index;             /*!< (RW) Line selected */
	enum message_flags flags;       /*!< (RW) Message flags */
	enum message_state state;       /*!< (RO) Current state */

	/*! \cond PRIVATE */

	struct texture *textures[12];
	unsigned int elapsed;
	int height[2];

	/*! \endcond */
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
 * Destroy owned resources.
 *
 * \pre msg != NULL
 * \param msg the message
 */
void
message_finish(struct message *msg);

#endif /* !MOLKO_MESSAGE_H */