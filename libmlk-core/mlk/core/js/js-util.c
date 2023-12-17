/*
 * js-util.c -- utilities (Javascript bindings)
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

#include <mlk/core/util.h>

#include "js-util.h"
#include "js.h"

static duk_ret_t
mlk_js_util_sleep(duk_context *ctx)
{
	mlk_util_sleep(duk_require_uint(ctx, 0));

	return 0;
}

static const duk_function_list_entry functions[] = {
	{ "sleep",      mlk_js_util_sleep,      1 },
	{ NULL,         NULL,                   0 }
};

void
mlk_js_util_load(duk_context *ctx)
{
	assert(ctx);

	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1, "Mlk");
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, functions);
	duk_put_prop_string(ctx, -2, "Util");
	duk_pop_n(ctx, 2);
}
