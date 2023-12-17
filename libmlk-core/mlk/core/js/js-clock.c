/*
 * js-clock.c -- track elapsed time (Javascript bindings)
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
#include <stdlib.h>

#include <mlk/core/alloc.h>
#include <mlk/core/clock.h>

#include "js-window.h"
#include "js.h"

#define SYMBOL DUK_HIDDEN_SYMBOL("mlk::clock")

static struct mlk_clock *
mlk_js_clock_this(duk_context *ctx)
{
	struct mlk_clock *clock;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SYMBOL);
	clock = duk_to_pointer(ctx, -1);
	duk_pop_n(ctx, 2);

	if (!clock)
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "Not a Clock object");

	return clock;
}

static duk_ret_t
mlk_js_clock_getElapsed(duk_context *ctx)
{
	duk_push_uint(ctx, mlk_clock_elapsed(mlk_js_clock_this(ctx)));

	return 1;
}

static duk_ret_t
mlk_js_clock_new(duk_context *ctx)
{
	if (!duk_is_constructor_call(ctx))
		return duk_error(ctx, DUK_ERR_TYPE_ERROR, "Clock must be new-constructed");

	duk_push_this(ctx);
	duk_push_pointer(ctx, mlk_alloc_new0(1, sizeof (struct mlk_clock)));
	duk_put_prop_string(ctx, -2, SYMBOL);
	duk_push_string(ctx, "elapsed");
	duk_push_c_function(ctx, mlk_js_clock_getElapsed, 0);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_GETTER);
	duk_pop(ctx);

	return 0;
}

static duk_ret_t
mlk_js_clock_start(duk_context *ctx)
{
	mlk_clock_start(mlk_js_clock_this(ctx));

	return 0;
}

static duk_ret_t
mlk_js_clock_finish(duk_context *ctx)
{
	duk_get_prop_string(ctx, 0, SYMBOL);
	mlk_alloc_free(duk_to_pointer(ctx, -1));
	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SYMBOL);

	return 0;
}

static const duk_function_list_entry methods[] = {
	{ "start",      mlk_js_clock_start,     0 },
	{ NULL,         NULL,                   0 }
};

void
mlk_js_clock_load(duk_context *ctx)
{
	assert(ctx);

	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1, "Mlk");
	duk_push_c_function(ctx, mlk_js_clock_new, 0);
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, methods);
	duk_push_c_function(ctx, mlk_js_clock_finish, 1);
	duk_set_finalizer(ctx, -2);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_prop_string(ctx, -2, "Clock");
	duk_pop_n(ctx, 2);
}
