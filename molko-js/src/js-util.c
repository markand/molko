/*
 * js-util.c -- utilities (Javascript bindings)
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

#include <core/util.h>

#include "js.h"
#include "js-util.h"

static duk_ret_t
js_util_delay(duk_context *ctx)
{
	delay(duk_require_uint(ctx, 0));

	return 0;
}

static const duk_function_list_entry functions[] = {
	{ "delay",      js_util_delay,  1       },
	{ NULL,         NULL,           0       }
};

void
js_util_load(struct js *js)
{
	assert(js);

	duk_push_global_object(js->handle);
	duk_get_prop_string(js->handle, -1, "Molko");
	duk_push_object(js->handle);
	duk_put_function_list(js->handle, -1, functions);
	duk_put_prop_string(js->handle, -2, "Util");
	duk_pop_n(js->handle, 2);
}
