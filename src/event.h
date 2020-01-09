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

#ifndef MOLKO_EVENT_H
#define MOLKO_EVENT_H

/**
 * \file event.h
 * \brief Event management.
 */

#include <stdbool.h>
#include <stdint.h>

#include "key.h"
#include "mouse.h"

/**
 * \brief Kind of event.
 */
enum event_type {
	EVENT_CLICKDOWN,        /*!< Mouse click down */
	EVENT_CLICKUP,          /*!< Mouse click released */
	EVENT_KEYDOWN,          /*!< Single key down */
	EVENT_KEYUP,            /*!< Single key released */
	EVENT_MOUSE,            /*!< Mouse moved */
	EVENT_QUIT,             /*!< Quit request */
};

/**
 * \brief Store events.
 */
union event {
	enum event_type type;                   /*!< Which kind of event */

        /**
         * Store key down/up event.
         */
	struct {
		enum event_type type;           /*!< EVENT_KEYDOWN or EVENT_KEYUP */
		enum key key;                   /*!< Which key */
	} key;

        /**
         * Store mouse motion event.
         */
	struct {
		enum event_type type;           /*!< EVENT_MOUSE */
		enum mouse_button buttons;      /*!< OR'ed buttons that are pressed */
		int32_t x;                      /*!< Mouse position in x */
		int32_t y;                      /*!< Mouse position in y */
	} mouse;

        /**
         * Store mouse click event.
         */
	struct {
		enum event_type type;           /*!< EVENT_CLICKDOWN or EVENT_CLICKUP */
		enum mouse_button button;       /*!< Unique button that was pressed */
		int32_t x;                      /*!< Mouse position in x */
		int32_t y;                      /*!< Mouse position in y */
	} click;
};

/**
 * Fetch the next event or return false if there are not.
 *
 * \param ev the event
 * \return true if the event was filled or false otherwise
 */
bool
event_poll(union event *ev);

#endif /* !MOLKO_EVENT_H */
