/*
 * js-panic.c -- core panic binding
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

#include <core/panic.h>

#include "js-panic.h"

static duk_ret_t
Mlk_panic(duk_context *ctx)
{
	if (duk_get_top(ctx) >= 1)
		panicf("%s", duk_to_string(ctx, 0));
	else
		panic();

	return 0;
}

static const duk_function_list_entry functions[] = {
	{ "panic",              Mlk_panic,              DUK_VARARGS     },
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