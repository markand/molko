/*
 * js-panic.c -- core panic binding
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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
#include <stdio.h>

#include <core/panic.h>

#include "js-panic.h"

#define PANIC_CB DUK_HIDDEN_SYMBOL("Mlk.panicHandler")

static void
handler(void)
{
	duk_context *ctx = panic_data;

	duk_push_global_stash(ctx);
	duk_get_prop_string(ctx, -1, PANIC_CB);
	duk_remove(ctx, -2);

	if (duk_is_callable(ctx, -1)) {
		duk_call(ctx, 0);
		duk_pop(ctx);
	} else {
		fprintf(stderr, "abort: javascript panic handler not callable\n");
		exit(1);
	}
}

static duk_ret_t
Mlk_panic(duk_context *ctx)
{
	if (duk_get_top(ctx) >= 1)
		panicf("%s", duk_to_string(ctx, 0));
	else
		panic();

	return 0;
}

static duk_ret_t
Mlk_setPanic(duk_context *ctx)
{
	if (!duk_is_callable(ctx, 0))
		return duk_error(ctx, DUK_ERR_TYPE_ERROR, "handler not callable");

	duk_push_global_stash(ctx);
	duk_dup(ctx, 0);
	duk_put_prop_string(ctx, -2, PANIC_CB);
	duk_pop(ctx);

	panic_handler = handler;
	panic_data = ctx;

	return 0;
}

static const duk_function_list_entry functions[] = {
	{ "panic",              Mlk_panic,              DUK_VARARGS     },
	{ "setPanic",           Mlk_setPanic,           1               },
	{ NULL,                 NULL,                   0               }
};

void
js_panic_bind(duk_context *ctx)
{
	assert(ctx);

	duk_get_global_string(ctx, "Mlk");
	duk_put_function_list(ctx, -1, functions);
	duk_pop(ctx);
}
