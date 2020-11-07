/*
 * js-window.c -- basic window management (Javascript bindings)
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

#include <core/error.h>
#include <core/window.h>

#include "js.h"
#include "js-window.h"

static duk_ret_t
js_window_setCursor(duk_context *ctx)
{
	enum window_cursor cursor = duk_require_int(ctx, 0);

	if (cursor < 0 || cursor > WINDOW_CURSOR_LAST)
		return duk_error(ctx, DUK_ERR_TYPE_ERROR, "invalid cursor type: %d", cursor);

	window_set_cursor(cursor);

	return 0;
}

static duk_ret_t
js_window_new(duk_context *ctx)
{
	if (!duk_is_constructor_call(ctx))
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "Window must be new-constructed");

	const char *str = duk_require_string(ctx, 0);
	unsigned int w = duk_require_int(ctx, 1);
	unsigned int h = duk_require_int(ctx, 2);

	if (!window_open(str, w, h))
		duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	duk_push_this(ctx);
	duk_push_string(ctx, "cursor");
	duk_push_c_function(ctx, js_window_setCursor, 1);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_SETTER);
	duk_pop(ctx);

	return 0;
}

static const duk_number_list_entry cursors[] = {
	{ "ARROW",      WINDOW_CURSOR_ARROW             },
	{ "EDIT",       WINDOW_CURSOR_EDIT              },
	{ "WAIT",       WINDOW_CURSOR_WAIT              },
	{ "CROSS_HAIR", WINDOW_CURSOR_CROSSHAIR         },
	{ "SIZE",       WINDOW_CURSOR_SIZE              },
	{ "NO",         WINDOW_CURSOR_NO                },
	{ "HAND",       WINDOW_CURSOR_HAND              },
	{ NULL,         0                               }
};

void
js_window_load(struct js *js)
{
	assert(js);

	duk_push_global_object(js->handle);                  // [g]
	duk_get_prop_string(js->handle, -1, "Molko");        // [g] [Molko]
	duk_push_c_function(js->handle, js_window_new, 3);   // [g] [Molko] [Window]
	duk_push_object(js->handle);                         // [g] [Molko] [Window] [Cursor]
	duk_put_number_list(js->handle, -1, cursors);
	duk_put_prop_string(js->handle, -2, "Cursor");       // [g] [Molko] [Window]
	duk_put_prop_string(js->handle, -2, "Window");
	duk_pop_n(js->handle, 2);
}
