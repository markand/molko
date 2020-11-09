/*
 * js-clock.c -- track elapsed time (Javascript bindings)
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
#include <stdlib.h>

#include <duktape.h>

#include <core/alloc.h>
#include <core/clock.h>

#include "js.h"
#include "js-window.h"

#define SYMBOL DUK_HIDDEN_SYMBOL("molko::clock")

static struct clock *
js_clock_this(duk_context *ctx)
{
	struct clock *clock;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SYMBOL);
	clock = duk_to_pointer(ctx, -1);
	duk_pop_n(ctx, 2);

	if (!clock)
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "Not a Clock object");

	return clock;
}

static duk_ret_t
js_clock_getElapsed(duk_context *ctx)
{
	duk_push_uint(ctx, clock_elapsed(js_clock_this(ctx)));

	return 1;
}

static duk_ret_t
js_clock_new(duk_context *ctx)
{
	if (!duk_is_constructor_call(ctx))
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "Clock must be new-constructed");

	duk_push_this(ctx);
	duk_push_pointer(ctx, alloc_zero(1, sizeof (struct clock)));
	duk_put_prop_string(ctx, -2, SYMBOL);
	duk_push_string(ctx, "elapsed");
	duk_push_c_function(ctx, js_clock_getElapsed, 0);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_GETTER);
	duk_pop(ctx);

	return 0;
}

static duk_ret_t
js_clock_start(duk_context *ctx)
{
	clock_start(js_clock_this(ctx));

	return 0;
}

static duk_ret_t
js_clock_finish(duk_context *ctx)
{
	duk_get_prop_string(ctx, 0, SYMBOL);
	free(duk_to_pointer(ctx, -1));
	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SYMBOL);

	return 0;
}

static const duk_function_list_entry methods[] = {
	{ "start",      js_clock_start,         0 },
	{ NULL,         NULL,                   0 }
};

void
js_clock_load(duk_context *ctx)
{
	assert(ctx);

	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1, "Molko");
	duk_push_c_function(ctx, js_clock_new, 0);
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, methods);
	duk_push_c_function(ctx, js_clock_finish, 1);
	duk_set_finalizer(ctx, -2);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_prop_string(ctx, -2, "Clock");
	duk_pop_n(ctx, 2);
}
