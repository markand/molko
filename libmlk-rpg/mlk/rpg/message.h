/*
 * message.h -- message dialog
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

#ifndef MLK_RPG_MESSAGE_H
#define MLK_RPG_MESSAGE_H

/**
 * \file mlk/rpg/message.h
 * \brief Message dialog.
 *
 * This module offers a message box dialog that shows several lines similar to
 * old school RPG games.
 *
 * The dialog can be animated while appearing and hiding using special
 * ::MLK_MESSAGE_FLAGS_FADEIN and ::MLK_MESSAGE_FLAGS_FADEOUT which aren't set
 * by default.
 *
 * Lines can be selected by specifying a line mask as a bitmask, see
 * ::mlk_message::selectable field for more information.
 *
 * Example of message dialog:
 *
 * \image html example-message.png
 */

#include <stddef.h>

/**
 * \brief Default message animation speed.
 */
#define MLK_MESSAGE_SPEED_DEFAULT       (150)

/**
 * \brief Default message automatic close duration.
 */
#define MLK_MESSAGE_TIMEOUT_DEFAULT     (5000)

struct mlk_font;
struct mlk_message;
struct mlk_message_style;

union mlk_event;

/**
 * \enum mlk_message_flags
 * \brief Message flags
 */
enum mlk_message_flags {
	/**
	 * Message closes automatically after style's duration.
	 */
	MLK_MESSAGE_FLAGS_AUTOMATIC     = (1 << 0),

	/**
	 * Add fade in animation.
	 */
	MLK_MESSAGE_FLAGS_FADEIN        = (1 << 2),

	/**
	 * Add fade out animation.
	 */
	MLK_MESSAGE_FLAGS_FADEOUT       = (1 << 3)
};

/**
 * \enum mlk_message_state
 * \brief Describe current message state
 */
enum mlk_message_state {
	/**
	 * Message is not ready yet.
	 */
	MLK_MESSAGE_STATE_NONE,

	/**
	 * Animation opening.
	 */
	MLK_MESSAGE_STATE_OPENING,

	/**
	 * Normal message run.
	 */
	MLK_MESSAGE_STATE_SHOWING,

	/**
	 * Hiding animation.
	 */
	MLK_MESSAGE_STATE_HIDING
};

/**
 * \struct mlk_message
 * \brief Message box.
 */
struct mlk_message {
	/**
	 * (read-write)
	 *
	 * Position in x.
	 */
	int x;

	/**
	 * (read-write)
	 *
	 * Position in y.
	 */
	int y;

	/**
	 * (read-write)
	 *
	 * Menu frame width.
	 */
	unsigned int w;

	/**
	 * (read-write)
	 *
	 * Menu frame height.
	 */
	unsigned int h;

	/**
	 * (read-write, borrowed)
	 *
	 * Lines to show.
	 */
	const char * const *lines;

	/**
	 * (read-write)
	 *
	 * Number of lines in ::mlk_message::lines.
	 */
	size_t linesz;

	/**
	 * (read-write)
	 *
	 * Tells which lines can be selected as bitmask, right most bit being
	 * the first line in the array.
	 *
	 * Example:
	 *
	 * [0] Would you open this chest?
	 * [1] Yes.
	 * [2] No I prefer run away.
	 *
	 * Selectables optiong being 1 and 2, this field must be set to
	 * `(1 << 1) | (1 << 2)`.
	 */
	size_t selectable;

	/**
	 * (read-write)
	 *
	 * Selected item from the user.
	 */
	size_t selected;

	/**
	 * (read-write)
	 *
	 * Optional message flags.
	 */
	enum mlk_message_flags flags;

	/**
	 * (read-only)
	 *
	 * Current message state.
	 */
	enum mlk_message_state state;

	/**
	 * (read-write, borrowed, optional)
	 *
	 * Style to use for drawing this message.
	 */
	struct mlk_message_style *style;

	/** \cond MLK_PRIVATE_DECLS */
	unsigned int elapsed;   /* elapsed time in the state */
	double scale;           /* texture scaling */
	/** \endcond MLK_PRIVATE_DECLS */
};

/**
 * \struct mlk_message_style
 * \brief Message style.
 */
struct mlk_message_style {
	/**
	 * (read-write)
	 *
	 * Background color.
	 */
	unsigned long background;

	/**
	 * (read-write)
	 *
	 * Border color.
	 */
	unsigned long border;

	/**
	 * (read-write)
	 *
	 * Border size.
	 */
	unsigned int border_size;

	/**
	 * (read-write)
	 *
	 * Text color.
	 */
	unsigned long color;

	/**
	 * (read-write)
	 *
	 * Selected color.
	 */
	unsigned long color_selected;

	/**
	 * (read-write)
	 *
	 * Left-right padding.
	 */
	unsigned int padding;

	/**
	 * (read-write, borrowed, optional)
	 *
	 * Font for drawing text.
	 */
	struct mlk_font *font;

	/**
	 * (read-write)
	 *
	 * Message timeout in milliseconds when set to automatic.
	 */
	unsigned int timeout;

	/**
	 * (read-write)
	 *
	 * Opening/hideing animation speed in milliseconds.
	 */
	unsigned int speed;

	/**
	 * (read-write, borrowed, optional)
	 *
	 * Arbitrary user data.
	 */
	void *data;

	/**
	 * (read-write, optional)
	 *
	 * Check the optimal dimensions for this message.
	 *
	 * \param self this style
	 * \param message the message to query dimensions
	 * \param w destination width (can be NULL)
	 * \param h destination height (can be NULL)
	 * \return 0 on success or -1 on error
	 */
	int (*query)(struct mlk_message_style *self,
	             struct mlk_message *message,
	             unsigned int *w,
	             unsigned int *h);

	/**
	 * (read-write, optional)
	 *
	 * Update this message.
	 *
	 * \param self this style
	 * \param message the message to update
	 * \param ticks frame ticks
	 */
	void (*update)(struct mlk_message_style *self,
	               struct mlk_message *message,
	               unsigned int ticks);

	/**
	 * (read-write, optional)
	 *
	 * Draw the message.
	 *
	 * \param self this style
	 * \param message the message to draw
	 */
	void (*draw)(struct mlk_message_style *self,
	             struct mlk_message *message);
};

/**
 * \brief Dark default style for message.
 */
extern struct mlk_message_style mlk_message_style_dark;

/**
 * \brief Dark default style for message.
 */
extern struct mlk_message_style mlk_message_style_light;

/**
 * \brief Default style for all messages.
 */
extern struct mlk_message_style *mlk_message_style;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Reset the message state.
 *
 * This function should be called at least once or when its fields change.
 *
 * \pre message != NULL
 * \param message the message to start
 */
void
mlk_message_start(struct mlk_message *message);

/**
 * Invoke ::mlk_message_style::query.
 */
int
mlk_message_query(struct mlk_message *message, unsigned int *w, unsigned int *h);

/**
 * Handle an event.
 *
 * \pre message != NULL
 * \pre ev != NULL
 * \param message the message
 * \param ev the event
 */
void
mlk_message_handle(struct mlk_message *message, const union mlk_event *ev);

/**
 * Invoke ::mlk_message_style::update.
 *
 * \return non-zero if the message has ended
 */
int
mlk_message_update(struct mlk_message *message, unsigned int ticks);

int
mlk_message_complete(const struct mlk_message *message);

/**
 * Invoke ::mlk_message_style::draw.
 */
void
mlk_message_draw(struct mlk_message *message);

/**
 * Start hiding the message.
 *
 * This should be called when the message has fade-out flags and the animation
 * to close should start, keep updating and drawing until it completes.
 *
 * \pre message
 * \param message the message to hide
 */
void
mlk_message_hide(struct mlk_message *message);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_RPG_MESSAGE_H */
