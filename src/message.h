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
#include <stdint.h>

struct sprite;
struct font;

/**
 * \brief Message flags.
 */
enum message_flags {
	MESSAGE_AUTOMATIC = (1 << 0)    /*!< Message will automatically close */
};

/**
 * \brief Message state.
 */
enum message_state {
	MESSAGE_OPENING,                /*!< Message animation is opening */
	MESSAGE_SHOWING,                /*!< Message is displaying */
	MESSAGE_HIDING                  /*!< Message animation for hiding */
};

/**
 * \brief Message object.
 */
struct message {
	const char *text[3];            /*!< (RW) lines of text to show */
	struct sprite *theme;           /*!< (RW) sprite to use for the frame */
	struct texture *avatar;         /*!< (RW) optional avatar */
        struct font *font;              /*!< (RW) font to use */
	enum message_flags flags;       /*!< (RW) message flags */
	enum message_state state;       /*!< (RO) current state */
	uint16_t elapsed;               /*!< (RW) elapsed time while displaying */
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
 * Update the message state and elapsed time..
 *
 * \pre msg != NULL
 * \param msg the message
 * \param ticks the elapsed delay since last frame
 */
void
message_update(struct message *msg, unsigned ticks);

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
 * Tells if the message is complete.
 *
 * \pre msg != NULL
 * \param msg the message
 */
bool
message_is_complete(struct message *msg);

#endif /* !MOLKO_MESSAGE_H */
