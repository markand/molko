/*
 * event.h -- event management
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

#ifndef MLK_CORE_EVENT_H
#define MLK_CORE_EVENT_H

/**
 * \file mlk/core/event.h
 * \brief Event management
 *
 * The Molko's Engine framework pushes input events into a queue that the main
 * loop should pull out in the main game loop. For maximum reactivity, the
 * events should be pull all at once in the beginning of the loop.
 *
 * An event is described as an union (::mlk_event) storing every event
 * supported by the framework. The union has a special ::mlk_event::type field
 * describing the type and which underlying structure to use.
 *
 * See the enumeration constants for more details.
 */

#include "key.h"
#include "mouse.h"
#include "gamepad.h"
#include "window.h"

/**
 * \enum mlk_event_type
 * \brief Describe an event
 */
enum mlk_event_type {
	/**
	 * Mouse click press.
	 *
	 * Access through ::mlk_event::click.
	 */
	MLK_EVENT_CLICKDOWN,

	/**
	 * Mouse click release.
	 *
	 * Access through ::mlk_event::click.
	 */
	MLK_EVENT_CLICKUP,

	/**
	 * Keyboard key press.
	 *
	 * Access through ::mlk_event::key.
	 */
	MLK_EVENT_KEYDOWN,

	/**
	 * Keyboard key release.
	 *
	 * Access through ::mlk_event::key.
	 */
	MLK_EVENT_KEYUP,

	/**
	 * Mouse motion.
	 *
	 * Access through ::mlk_event::mouse.
	 */
	MLK_EVENT_MOUSE,

	/**
	 * Game controller D-Pad/button press.
	 *
	 * Access through ::mlk_event::pad.
	 */
	MLK_EVENT_PADUP,

	/**
	 * Game controller D-Pad/button release.
	 *
	 * Access through ::mlk_event::pad.
	 */
	MLK_EVENT_PADDOWN,

	/**
	 * Game controller joystick axis motion.
	 *
	 * Access through ::mlk_event::axis.
	 */
	MLK_EVENT_AXIS,

	/**
	 * Operating system theme changed.
	 */
	MLK_EVENT_THEME,
	
	/**
	 * Window manager quit event.
	 *
	 * No value.
	 */
	MLK_EVENT_QUIT,
};

/**
 * \struct mlk_event_key
 * \brief Keyboard event press/release
 */
struct mlk_event_key {
	/**
	 * Set to ::MLK_EVENT_KEYDOWN or ::MLK_EVENT_KEYUP.
	 */
	enum mlk_event_type type;

	/**
	 * Which key constant has been pressed/released.
	 */
	enum mlk_key key;
};

/**
 * \struct mlk_event_mouse
 * \brief Mouse motion event
 */
struct mlk_event_mouse {
	/**
	 * Set to MLK_EVENT_MOUSE.
	 */
	enum mlk_event_type type;

	/**
	 * Buttons currently pressed as a bitmask.
	 */
	enum mlk_mouse_button buttons;

	/**
	 * New mouse position in x.
	 */
	int x;

	/**
	 * New mouse position in y.
	 */
	int y;
};

/**
 * \struct mlk_event_click
 * \brief Mouse button click
 */
struct mlk_event_click {
	/**
	 * Set to ::MLK_EVENT_CLICKDOWN or ::MLK_EVENT_CLICKUP.
	 */
	enum mlk_event_type type;

	/**
	 * Which button has been pressed/released.
	 */
	enum mlk_mouse_button button;

	/**
	 * Position in x.
	 */
	int x;

	/**
	 * Position in y.
	 */
	int y;

	/**
	 * Number of clicks if supported.
	 */
	unsigned int clicks;
};

/**
 * \struct mlk_event_pad
 * \brief Game controller button press/release
 */
struct mlk_event_pad {
	/**
	 * Set to ::MLK_EVENT_PADDOWN or ::MLK_EVENT_PADUP.
	 */
	enum mlk_event_type type;

	/**
	 * Game controller button that has been pressed/released
	 */
	enum mlk_gamepad_button button;
};

/**
 * \struct mlk_event_axis
 * \brief Game controller joystick axis event
 */
struct mlk_event_axis {
	/**
	 * Set to ::MLK_EVENT_AXIS.
	 */
	enum mlk_event_type type;

	/**
	 * Which axis.
	 */
	enum mlk_gamepad_axis axis;

	/**
	 * The new value.
	 */
	int value;
};

/**
 * \struct mlk_event_theme
 * \brief Operating system theme changed
 */
struct mlk_event_theme {
	/**
	 * Set to ::MLK_EVENT_THEME.
	 */
	enum mlk_event_type type;

	/**
	 * New effective theme ::MLK_WINDOW_THEME_DARK or
	 * ::MLK_WINDOW_THEME_LIGHT.
	 */
	enum mlk_window_theme theme;
};

/**
 * \union mlk_event
 * \brief Generic input event
 */
union mlk_event {
	/**
	 * Present for all substructures to be present in the union itself.
	 */
	enum mlk_event_type type;

	/**
	 * For ::MLK_EVENT_KEYDOWN, ::MLK_EVENT_KEYUP.
	 */
	struct mlk_event_key key;

	/**
	 * For ::MLK_EVENT_MOUSE.
	 */
	struct mlk_event_mouse mouse;

	/**
	 * For ::MLK_EVENT_CLICKDOWN, ::MLK_EVENT_CLICKUP.
	 */
	struct mlk_event_click click;

	/**
	 * For ::MLK_EVENT_PADDOWN, ::MLK_EVENT_PADUP.
	 */
	struct mlk_event_pad pad;

	/**
	 * For ::MLK_EVENT_AXIS.
	 */
	struct mlk_event_axis axis;

	/**
	 * For ::MLK_EVENT_THEME.
	 */
	struct mlk_event_theme theme;
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Get the next event in the queue.
 *
 * \param event the event to fill
 * \return 1 if an event was found, 0 otherwise
 */
int
mlk_event_poll(union mlk_event *event);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_EVENT_H */
