/*
 * gamepad.h -- game controller support
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

#ifndef MLK_CORE_GAMEPAD_H
#define MLK_CORE_GAMEPAD_H

/**
 * \file mlk/core/gamepad.h
 * \brief Game controller support
 *
 * This module abstract the gamepad support using a predefined gamepad layout
 * similar to the XBox 360 controller.
 *
 * Once a gamepad is opened, it will automatically generate events depending on
 * the device.
 */

/**
 * \enum mlk_gamepad_button
 * \brief Describe a gamepad button
 */
enum mlk_gamepad_button {
	/**
	 * Unknown button.
	 */
	MLK_GAMEPAD_BUTTON_UNKNOWN,

	/**
	 * Button A.
	 */
	MLK_GAMEPAD_BUTTON_A,

	/**
	 * Button B.
	 */
	MLK_GAMEPAD_BUTTON_B,

	/**
	 * Button X.
	 */
	MLK_GAMEPAD_BUTTON_X,

	/**
	 * Button Y.
	 */
	MLK_GAMEPAD_BUTTON_Y,

	/**
	 * Button back/select depending on the gamepad.
	 */
	MLK_GAMEPAD_BUTTON_BACK,

	/**
	 * Button logo/brand or usually in the middle of the gamepad.
	 */
	MLK_GAMEPAD_BUTTON_LOGO,

	/**
	 * Button start.
	 */
	MLK_GAMEPAD_BUTTON_START,

	/**
	 * Left thumb click.
	 */
	MLK_GAMEPAD_BUTTON_LTHUMB,

	/**
	 * Right thumb click.
	 */
	MLK_GAMEPAD_BUTTON_RTHUMB,

	/**
	 * Left shoulder.
	 */
	MLK_GAMEPAD_BUTTON_LSHOULDER,

	/**
	 * Right shoulder.
	 */
	MLK_GAMEPAD_BUTTON_RSHOULDER,

	/**
	 * D-Pad up.
	 */
	MLK_GAMEPAD_BUTTON_UP,

	/**
	 * D-Pad down.
	 */
	MLK_GAMEPAD_BUTTON_DOWN,

	/**
	 * D-Pad left.
	 */
	MLK_GAMEPAD_BUTTON_LEFT,

	/**
	 * D-Pad right.
	 */
	MLK_GAMEPAD_BUTTON_RIGHT
};

/**
 * \enum mlk_gamepad_axis
 * \brief Describe a joystick axis.
 */
enum mlk_gamepad_axis {
	/**
	 * Unknown axis.
	 */
	MLK_GAMEPAD_AXIS_UNKNOWN,

	/**
	 * Left thumb horizontal axis.
	 */
	MLK_GAMEPAD_AXIS_LX,

	/**
	 * Left thumb vertical axis.
	 */
	MLK_GAMEPAD_AXIS_LY,

	/**
	 * Right thumb horizontal axis.
	 */
	MLK_GAMEPAD_AXIS_RX,

	/**
	 * Right thumb vertical axis.
	 */
	MLK_GAMEPAD_AXIS_RY,

	/**
	 * Left shoulder axis.
	 */
	MLK_GAMEPAD_AXIS_LTRIGGER,

	/**
	 * Right shoulder axis.
	 */
	MLK_GAMEPAD_AXIS_RTRIGGER
};

/**
 * \struct mlk_gamepad
 * \brief Gamepad structure
 */
struct mlk_gamepad {
	/**
	 * (read-only, borrowed)
	 *
	 * Gamepad name.
	 */
	const char *name;

	/** \cond MLK_PRIVATE_DECLS */
	void *handle;
	/** \endcond MLK_PRIVATE_DECLS */
};

#if 0

/**
 * \struct mlk_gamepad_iter
 * \brief Iterate over all plugged gamepads.
 */
struct mlk_gamepad_iter {
	/**
	 * (read-only, borrowed)
	 *
	 * Gamepad name.
	 */
	const char *name;

	/**
	 * (read-only)
	 *
	 * Device index on the system, can be used to open a device using
	 * ::mlk_gamepad_open.
	 */
	int index;

	/** \cond MLK_PRIVATE_DECLS */
	int end;
	/** \endcond MLK_PRIVATE_DECLS */
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Open the gamepad at the given device index.
 *
 * \pre pad != NULL
 * \param pad the gamepad to initialize
 * \param index the device index
 * \return 0 on success or any error code instead
 */
int
mlk_gamepad_open(struct mlk_gamepad *pad, int index);

/**
 * Close this gamepad.
 *
 * \pre pad != NULL
 * \param pad the gamepad
 */
void
mlk_gamepad_finish(struct mlk_gamepad *pad);

/**
 * Start iterating gamepad devices.
 *
 * On success, call ::mlk_gamepad_iter_next to retrieve the next device
 * (including the first one) in a loop.
 *
 * \pre iter the iter to start
 * \return 0 on success or any error code instead
 */
int
mlk_gamepad_iter_begin(struct mlk_gamepad_iter *iter);

/**
 * Retrieve the next device.
 *
 * This function should be called in a loop to retrieve all devices available
 * on the system.
 *
 * \pre iter the iter to start
 * \return 1 if there are still devices available
 */
int
mlk_gamepad_iter_next(struct mlk_gamepad_iter *iter);

#endif

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_GAMEPAD_H */
