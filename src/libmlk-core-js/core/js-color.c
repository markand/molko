/*
 * js-color.c -- core color binding
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

#include <core/color.h>

#include "js-color.h"

static inline unsigned int
component(duk_context *ctx, duk_idx_t ind, const char *prop, unsigned int def)
{
	unsigned int value = 0;

	if (duk_is_object(ctx, 0)) {
		duk_get_prop_string(ctx, 0, prop);
		value = duk_get_uint_default(ctx, -1, def);
		duk_pop(ctx);
	} else
		value = duk_get_uint_default(ctx, ind, def);

	return value;
}

static duk_ret_t
Color_fromRGB(duk_context *ctx)
{
	const unsigned int r = component(ctx, 0, "r", 0);
	const unsigned int g = component(ctx, 1, "g", 0);
	const unsigned int b = component(ctx, 2, "b", 0);
	const unsigned int a = component(ctx, 3, "a", 255);

	duk_push_number(ctx, COLOR_HEX(r, g, b, a));

	return 1;
}

static duk_ret_t
Color_toRGB(duk_context *ctx)
{
	const unsigned long hex = duk_require_uint(ctx, 0);

	duk_push_object(ctx);
	duk_push_uint(ctx, COLOR_R(hex));
	duk_put_prop_string(ctx, -2, "r");
	duk_push_uint(ctx, COLOR_G(hex));
	duk_put_prop_string(ctx, -2, "g");
	duk_push_uint(ctx, COLOR_B(hex));
	duk_put_prop_string(ctx, -2, "b");
	duk_push_uint(ctx, COLOR_A(hex));
	duk_put_prop_string(ctx, -2, "a");

	return 1;
}

static const duk_function_list_entry functions[] = {
	{ "fromRGB",    Color_fromRGB,  DUK_VARARGS     },
	{ "toRGB",      Color_toRGB,    1               },
	{ NULL,         NULL,           0               }
};

void
js_color_bind(duk_context *ctx)
{
	assert(ctx);

	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, functions);
	duk_put_global_string(ctx, "Color");
}
