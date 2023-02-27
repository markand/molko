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

#include "core.h"

MLK_CORE_BEGIN_DECLS

enum mlk_gamepad_button {
	MLK_GAMEPAD_BUTTON_UNKNOWN,
	MLK_GAMEPAD_BUTTON_A,
	MLK_GAMEPAD_BUTTON_B,
	MLK_GAMEPAD_BUTTON_X,
	MLK_GAMEPAD_BUTTON_Y,
	MLK_GAMEPAD_BUTTON_BACK,
	MLK_GAMEPAD_BUTTON_LOGO,
	MLK_GAMEPAD_BUTTON_START,
	MLK_GAMEPAD_BUTTON_LTHUMB,
	MLK_GAMEPAD_BUTTON_RTHUMB,
	MLK_GAMEPAD_BUTTON_LSHOULDER,
	MLK_GAMEPAD_BUTTON_RSHOULDER,
	MLK_GAMEPAD_BUTTON_UP,
	MLK_GAMEPAD_BUTTON_DOWN,
	MLK_GAMEPAD_BUTTON_LEFT,
	MLK_GAMEPAD_BUTTON_RIGHT
};

enum mlk_gamepad_axis {
	MLK_GAMEPAD_AXIS_UNKNOWN,
	MLK_GAMEPAD_AXIS_LX,
	MLK_GAMEPAD_AXIS_LY,
	MLK_GAMEPAD_AXIS_RX,
	MLK_GAMEPAD_AXIS_RY,
	MLK_GAMEPAD_AXIS_LTRIGGER,
	MLK_GAMEPAD_AXIS_RTRIGGER
};

struct mlk_gamepad {
	const char *name;
	void *handle;
};

struct mlk_gamepad_iter {
	const char *name;
	int idx;
	int end;
};

int
mlk_gamepad_open(struct mlk_gamepad *pad, int idx);

void
mlk_gamepad_finish(struct mlk_gamepad *pad);

int
mlk_gamepad_iter_begin(struct mlk_gamepad_iter *);

int
mlk_gamepad_iter_next(struct mlk_gamepad_iter *);

MLK_CORE_END_DECLS

#endif /* !MLK_CORE_GAMEPAD_H */
