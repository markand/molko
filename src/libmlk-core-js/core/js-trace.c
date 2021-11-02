/*
 * js-trace.c -- core trace binding
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

#include <core/trace.h>

#include "js-trace.h"

#define TRACE_CB DUK_HIDDEN_SYMBOL("Mlk.traceHandler")

static void
handler(const char *msg)
{
	duk_context *ctx = trace_data;

	duk_push_global_stash(ctx);
	duk_get_prop_string(ctx, -1, TRACE_CB);
	duk_remove(ctx, -2);

	if (duk_is_callable(ctx, -1)) {
		duk_push_string(ctx, msg);
		duk_call(ctx, 1);
		duk_pop(ctx);
	} else {
		fprintf(stderr, "abort: javascript panic handler not callable\n");
		exit(1);
	}
}

static duk_ret_t
Mlk_trace(duk_context *ctx)
{
	tracef("%s", duk_require_string(ctx, 0));

	return 0;
}

static duk_ret_t
Mlk_setTrace(duk_context *ctx)
{
	if (!duk_is_callable(ctx, 0))
		return duk_error(ctx, DUK_ERR_TYPE_ERROR, "handler not callable");

	duk_push_global_stash(ctx);
	duk_dup(ctx, 0);
	duk_put_prop_string(ctx, -2, TRACE_CB);
	duk_pop(ctx);

	trace_handler = handler;
	trace_data = ctx;

	return 0;
}

static const duk_function_list_entry functions[] = {
	{ "trace",              Mlk_trace,              1       },
	{ "setTrace",           Mlk_setTrace,           1       },
	{ NULL,                 NULL,                   0       }
};

void
js_trace_bind(duk_context *ctx)
{
	assert(ctx);

	duk_get_global_string(ctx, "Mlk");
	duk_put_function_list(ctx, -1, functions);
	duk_pop(ctx);
}
