/*
 * js-window.c -- core window binding
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

#include <assert.h>

#include <core/error.h>
#include <core/window.h>

#include "js-window.h"

#define SIGNATURE DUK_HIDDEN_SYMBOL("Mlk.Window")

static duk_ret_t
Window_constructor(duk_context *ctx)
{
	const char *title = duk_require_string(ctx, 0);
	const unsigned int w = duk_require_uint(ctx, 1);
	const unsigned int h = duk_require_uint(ctx, 2);

	if (window_open(title, w, h) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	duk_push_this(ctx);
	duk_push_boolean(ctx, 1);
	duk_put_prop_string(ctx, -2, SIGNATURE);
	duk_pop(ctx);

	return 0;
}

static duk_ret_t
Window_setCursor(duk_context *ctx)
{
	const unsigned int cursor = duk_require_uint(ctx, 0);

	if (cursor >= WINDOW_CURSOR_LAST)
		return duk_error(ctx, DUK_ERR_RANGE_ERROR, "invalid cursor");

	window_set_cursor(cursor);

	return 0;
}

static duk_ret_t
Window_destructor(duk_context *ctx)
{
	duk_get_prop_string(ctx, 0, SIGNATURE);

	if (duk_to_boolean(ctx, -1))
		window_finish();

	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SIGNATURE);

	return 0;
}

static const duk_function_list_entry methods[] = {
	{ "setCursor",  Window_setCursor,       1 },
	{ NULL,         NULL,                   0 }
};

void
js_window_bind(duk_context *ctx)
{
	assert(ctx);

	duk_push_c_function(ctx, Window_constructor, 3);
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, methods);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_push_c_function(ctx, Window_destructor, 1);
	duk_set_finalizer(ctx, -2);
	duk_put_global_string(ctx, "Window");
}
