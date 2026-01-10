/*
 * notify.h -- in game notifications
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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

#ifndef MLK_UI_NOTIFY_H
#define MLK_UI_NOTIFY_H

/**
 * \file mlk/ui/notify.h
 * \brief In game notifications.
 */

#include <stddef.h>

struct mlk_font;
struct mlk_texture;

/**
 * \struct mlk_notify
 * \brief Structure describing a unique notification.
 */
struct mlk_notify {
	/**
	 * (read-only, borrowed, optional)
	 *
	 * An associated icon.
	 */
	const struct mlk_texture *icon;

	/**
	 * (read-only, borrowed, optional)
	 *
	 * Notification title.
	 */
	const char *title;

	/**
	 * (read-only, borrowed, optional)
	 *
	 * Notification content.
	 */
	const char *body;

	/** \cond MLK_PRIVATE_DECLS */
	unsigned int elapsed;
	/** \endcond MLK_PRIVATE_DECLS */
};

/**
 * \struct mlk_notify_style
 * \brief Notification system style.
 */
struct mlk_notify_style {
	/**
	 * (read-write, borrowed, optional)
	 *
	 * Font to use to draw title and body.
	 *
	 * If NULL, uses ::MLK_UI_FONT_INTERFACE.
	 */
	struct mlk_font *font;

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
	 * Notification lifetime in milliseconds.
	 */
	unsigned int timeout;

	/**
	 * (read-write)
	 *
	 * Padding between top corner and notification elements.
	 */
	unsigned int padding;

	/**
	 * (read-write, borrowed, optional)
	 *
	 * Array where to store notifications.
	 *
	 * If NULL, a static array is used with a limited numbers of
	 * notifications.
	 */
	struct mlk_notify *stack;

	/**
	 * (read-write)
	 *
	 * Number of available notifications in ::mlk_notify_style::stack.
	 */
	size_t stacksz;

	/**
	 * (read-write)
	 *
	 * Current number of notifications in the ::mlk_notify_style::stack.
	 */
	size_t length;

	/**
	 * (read-write, optional)
	 *
	 * Update the notifications and remove outdated ones.
	 *
	 * \param ticks frame ticks
	 */
	void (*update)(unsigned int ticks);

	/**
	 * (read-write, optional)
	 *
	 * \param n the notification to draw
	 * \param index notification number
	 */
	void (*draw)(const struct mlk_notify *notif, size_t index);
};

/**
 * \brief Global notification style.
 */
extern struct mlk_notify_style mlk_notify_style;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Push a new notification in the queue.
 *
 * \pre body != NULL
 * \param icon an optional icon to show (borrowed)
 * \param title an optional title to show (borrowed)
 * \param body notification text content (borrowed)
 */
void
mlk_notify(const struct mlk_texture *icon, const char *title, const char *body);

/**
 * Clear up every notifications.
 */
void
mlk_notify_clear(void);

/**
 * Invoke ::mlk_notify_style::update
 */
void
mlk_notify_update(unsigned int ticks);

/**
 * Invoke ::mlk_notify_style::draw
 */
void
mlk_notify_draw(void);

#if defined(__cplusplus)
}
#endif

#endif /*! MLK_UI_NOTIFY_H */
