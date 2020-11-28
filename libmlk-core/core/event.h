/*
 * event.h -- event management
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

#ifndef MOLKO_CORE_EVENT_H
#define MOLKO_CORE_EVENT_H

/**
 * \file event.h
 * \brief Event management.
 * \ingroup input
 */

#include <stdbool.h>

#include "key.h"
#include "mouse.h"

/**
 * \brief Kind of event.
 */
enum event_type {
	EVENT_CLICKDOWN,        /*!< Mouse click down (\ref event_click) */
	EVENT_CLICKUP,          /*!< Mouse click released (\ref event_click) */
	EVENT_KEYDOWN,          /*!< Single key down (\ref event_key) */
	EVENT_KEYUP,            /*!< Single key released (\ref event_key) */
	EVENT_MOUSE,            /*!< Mouse moved (\ref event_mouse) */
	EVENT_QUIT,             /*!< Quit request */
};

/**
 * \brief Key event.
 */
struct event_key {
	enum event_type type;           /*!< (+) EVENT_KEYDOWN or EVENT_KEYUP */
	enum key key;                   /*!< (+) Which key */
};

/**
 * \brief Mouse motion event.
 */
struct event_mouse {
	enum event_type type;           /*!< (+) EVENT_MOUSE */
	enum mouse_button buttons;      /*!< (+) OR'ed buttons that are pressed */
	int x;                          /*!< (+) Mouse position in x */
	int y;                          /*!< (+) Mouse position in y */
};

/**
 * \brief Mouse click event.
 */
struct event_click {
	enum event_type type;           /*!< (+) EVENT_CLICKDOWN or EVENT_CLICKUP */
	enum mouse_button button;       /*!< (+) Unique button that was pressed */
	int x;                          /*!< (+) Mouse position in x */
	int y;                          /*!< (+) Mouse position in y */
};

/**
 * \brief Store events.
 */
union event {
	enum event_type type;           /*!< (+) Which kind of event */
	struct event_key key;           /*!< (+) Key event */
	struct event_mouse mouse;       /*!< (+) Mouse motion event */
	struct event_click click;       /*!< (+) Mouse click event */
};

/**
 * Fetch the next event or return false if there are not.
 *
 * \param ev the event
 * \return True if the event was filled or false otherwise.
 */
bool
event_poll(union event *ev);

#endif /* !MOLKO_CORE_EVENT_H */
