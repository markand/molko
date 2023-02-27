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

#include "core.h"
#include "key.h"
#include "mouse.h"
#include "gamepad.h"

enum mlk_event_type {
	MLK_EVENT_CLICKDOWN,
	MLK_EVENT_CLICKUP,
	MLK_EVENT_KEYDOWN,
	MLK_EVENT_KEYUP,
	MLK_EVENT_MOUSE,
	MLK_EVENT_PADUP,
	MLK_EVENT_PADDOWN,
	MLK_EVENT_AXIS,
	MLK_EVENT_QUIT,
	MLK_EVENT_NUM
};

struct mlk_event_key {
	enum mlk_event_type type;
	enum mlk_key key;
};

struct mlk_event_mouse {
	enum mlk_event_type type;
	enum mlk_mouse_button buttons;
	int x;
	int y;
};

struct mlk_event_click {
	enum mlk_event_type type;
	enum mlk_mouse_button button;
	int x;
	int y;
	unsigned int clicks;
};

struct mlk_event_pad {
	enum mlk_event_type type;
	enum mlk_gamepad_button button;
};

struct mlk_event_axis {
	enum mlk_event_type type;
	enum mlk_gamepad_axis axis;
	int value;
};

union mlk_event {
	enum mlk_event_type type;
	struct mlk_event_key key;
	struct mlk_event_mouse mouse;
	struct mlk_event_click click;
	struct mlk_event_pad pad;
	struct mlk_event_axis axis;
};

MLK_CORE_BEGIN_DECLS

int
mlk_event_poll(union mlk_event *);

MLK_CORE_END_DECLS

#endif /* !MLK_CORE_EVENT_H */
