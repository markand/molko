/*
 * js-event.c -- event management (Javascript bindings)
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

#include <assert.h>

#include <duktape.h>

#include <mlk/core/event.h>

#include "js-event.h"
#include "js.h"

static duk_ret_t
mlk_js_event_poll(duk_context *ctx)
{
	union mlk_event ev;

	if (!mlk_event_poll(&ev))
		return 0;

	return mlk_js_event_push(ctx, &ev);
}

static const duk_number_list_entry types[] = {
	{ "CLICK_DOWN", MLK_EVENT_CLICKDOWN },
	{ "CLICK_UP",   MLK_EVENT_CLICKUP   },
	{ "KEY_DOWN",   MLK_EVENT_KEYDOWN   },
	{ "KEY_UP",     MLK_EVENT_KEYUP     },
	{ "MOUSE",      MLK_EVENT_MOUSE     },
	{ "QUIT",       MLK_EVENT_QUIT      },
	{ NULL,         0               }
};

static const duk_function_list_entry functions[] = {
	{ "poll",       mlk_js_event_poll,      0 },
	{ NULL,         NULL,                   0 }
};

duk_ret_t
mlk_js_event_push(duk_context *ctx, const union mlk_event *ev)
{
	assert(ctx);
	assert(ev);

	duk_push_object(ctx);
	duk_push_int(ctx, ev->type);
	duk_put_prop_string(ctx, -2, "type");

	switch (ev->type) {
	case MLK_EVENT_CLICKDOWN:
	case MLK_EVENT_CLICKUP:
		duk_push_int(ctx, ev->click.button);
		duk_put_prop_string(ctx, -2, "button");
		duk_push_int(ctx, ev->click.x);
		duk_put_prop_string(ctx, -2, "x");
		duk_push_int(ctx, ev->click.y);
		duk_put_prop_string(ctx, -2, "y");
		break;
	case MLK_EVENT_KEYDOWN:
	case MLK_EVENT_KEYUP:
		duk_push_int(ctx, ev->key.key);
		duk_put_prop_string(ctx, -2, "key");
		break;
	case MLK_EVENT_MOUSE:
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

void
mlk_js_event_load(duk_context *ctx)
{
	assert(ctx);

	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1, "Mlk");
	duk_push_object(ctx);
	duk_put_number_list(ctx, -1, types);
	duk_put_function_list(ctx, -1, functions);
	duk_put_prop_string(ctx, -2, "Event");
	duk_pop_n(ctx, 1);
}
