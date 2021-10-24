/*
 * event.h -- event management
 *
 * Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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

enum event_type {
	EVENT_CLICKDOWN,
	EVENT_CLICKUP,
	EVENT_KEYDOWN,
	EVENT_KEYUP,
	EVENT_MOUSE,
	EVENT_QUIT,
};

struct event_key {
	enum event_type type;
	enum key key;
};

struct event_mouse {
	enum event_type type;
	enum mouse_button buttons;
	int x;
	int y;
};

struct event_click {
	enum event_type type;
	enum mouse_button button;
	int x;
	int y;
	unsigned int clicks;
};

union event {
	enum event_type type;
	struct event_key key;
	struct event_mouse mouse;
	struct event_click click;
};

CORE_BEGIN_DECLS

int
event_poll(union event *);

CORE_END_DECLS

#endif /* !MLK_CORE_EVENT_H */
