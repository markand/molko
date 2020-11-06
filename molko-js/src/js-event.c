/*
 * js-event.c -- event management (Javascript bindings)
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

#include <assert.h>

#include <duktape.h>

#include <core/event.h>

#include "js.h"
#include "js-event.h"

static duk_ret_t
push(duk_context *ctx, const union event *ev)
{
	duk_push_object(ctx);
	duk_push_int(ctx, ev->type);
	duk_put_prop_string(ctx, -2, "type");

	switch (ev->type) {
	case EVENT_CLICKDOWN:
	case EVENT_CLICKUP:
		duk_push_int(ctx, ev->click.button);
		duk_put_prop_string(ctx, -2, "button");
		duk_push_int(ctx, ev->click.x);
		duk_put_prop_string(ctx, -2, "x");
		duk_push_int(ctx, ev->click.y);
		duk_put_prop_string(ctx, -2, "y");
		break;
	case EVENT_KEYDOWN:
	case EVENT_KEYUP:
		duk_push_int(ctx, ev->key.key);
		duk_put_prop_string(ctx, -2, "key");
		break;
	case EVENT_MOUSE:
		duk_push_int(ctx, ev->mouse.buttons);
		duk_put_prop_string(ctx, -2, "buttons");
		duk_push_int(ctx, ev->mouse.x);
		duk_put_prop_string(ctx, -2, "x");
		duk_push_int(ctx, ev->mouse.y);
		duk_put_prop_string(ctx, -2, "y");
		break;
	default:
		break;
	}

	return 1;
}

static duk_ret_t
js_event_poll(duk_context *ctx)
{
	union event ev;

	if (!event_poll(&ev))
		return 0;

	return push(ctx, &ev);
}

static const duk_number_list_entry types[] = {
	{ "CLICK_DOWN", EVENT_CLICKDOWN },
	{ "CLICK_UP",   EVENT_CLICKUP   },
	{ "KEY_DOWN",   EVENT_KEYDOWN   },
	{ "KEY_UP",     EVENT_KEYUP     },
	{ "MOUSE",      EVENT_MOUSE     },
	{ "QUIT",       EVENT_QUIT      },
	{ NULL,         0               }
};

static const duk_function_list_entry functions[] = {
	{ "poll",       js_event_poll,  0 },
	{ NULL,         NULL,           0 }
};

void
js_event_load(struct js *js)
{
	assert(js);

	duk_push_global_object(js->handle);
	duk_get_prop_string(js->handle, -1, "Molko");
	duk_push_object(js->handle);
	duk_put_number_list(js->handle, -1, types);
	duk_put_function_list(js->handle, -1, functions);
	duk_put_prop_string(js->handle, -2, "Event");
	duk_pop_n(js->handle, 1);
}
