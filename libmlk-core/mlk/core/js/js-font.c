/*
 * js-font.c -- basic font management (Javascript bindings)
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

#include <duktape.h>

#include <mlk/core/alloc.h>
#include <mlk/core/err.h>
#include <mlk/core/font.h>
#include <mlk/core/texture.h>

#include "js-font.h"
#include "js-texture.h"
#include "js.h"

#define SYMBOL  DUK_HIDDEN_SYMBOL("mlk::font")
#define PROTO   DUK_HIDDEN_SYMBOL("mlk::font::prototype")

static struct mlk_font *
mlk_js_font_this(duk_context *ctx)
{
	struct mlk_font *font;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SYMBOL);
	font = duk_to_pointer(ctx, -1);
	duk_pop_n(ctx, 2);

	if (!font)
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "Not a Font object");

	return font;
}

static duk_ret_t
mlk_js_font_getHeight(duk_context *ctx)
{
	duk_push_uint(ctx, mlk_font_height(mlk_js_font_this(ctx)));

	return 1;
}

static duk_ret_t
mlk_js_font_fromPath(duk_context *ctx)
{
	struct mlk_font font;
	const char *path = duk_require_string(ctx, 0);
	unsigned int size = duk_require_uint(ctx, 1);

	if (!mlk_font_open(&font, path, size))
		duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());

	duk_push_object(ctx);
	duk_push_global_stash(ctx);
	duk_get_prop_string(ctx, -1, PROTO);
	duk_remove(ctx, -2);
	duk_push_pointer(ctx, mlk_alloc_dup(&font, 1, sizeof (font)));
	duk_put_prop_string(ctx, -3, SYMBOL);
	duk_set_prototype(ctx, -2);
	duk_push_string(ctx, "height");
	duk_push_c_function(ctx, mlk_js_font_getHeight, 0);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_GETTER);
	
	return 1;
}

static duk_ret_t
mlk_js_font_render(duk_context *ctx)
{
	struct mlk_font *font = mlk_js_font_this(ctx);
	struct mlk_texture tex;
	const char *text = duk_require_string(ctx, 0);
	unsigned long color = duk_require_number(ctx, 1);

	if (mlk_font_render(font, &tex, text, color) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());

	mlk_js_texture_push(ctx, &tex);

	return 1;
}

static duk_ret_t
mlk_js_font_query(duk_context *ctx)
{
	struct mlk_font *font = mlk_js_font_this(ctx);
	const char *text = duk_require_string(ctx, 0);
	unsigned int w, h;

	if (mlk_font_query(font, text, &w, &h) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());

	duk_push_object(ctx);
	duk_push_uint(ctx, w);
	duk_put_prop_string(ctx, -2, "w");
	duk_push_uint(ctx, h);
	duk_put_prop_string(ctx, -2, "h");

	return 1;
}

static duk_ret_t
mlk_js_font_finish(duk_context *ctx)
{
	struct mlk_font *font;

	duk_get_prop_string(ctx, 0, SYMBOL);
	font = duk_to_pointer(ctx, -1);
	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SYMBOL);

	if (font) {
		mlk_font_finish(font);
		mlk_alloc_free(font);
	}

	return 0;
}

static const duk_function_list_entry functions[] = {
	{ "fromPath",   mlk_js_font_fromPath,   2 },
	{ NULL,         NULL,                   0 }
};

static const duk_function_list_entry methods[] = {
	{ "render",     mlk_js_font_render,     2 },
	{ "query",      mlk_js_font_query,      1 },
	{ NULL, NULL, 0 }
};

void
mlk_js_font_load(duk_context *ctx)
{
	assert(ctx);

	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1, "Mlk");
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, functions);
	duk_put_prop_string(ctx, -2, "Font");
	duk_push_global_stash(ctx);
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, methods);
	duk_push_c_function(ctx, mlk_js_font_finish, 1);
	duk_set_finalizer(ctx, -2);
	duk_put_prop_string(ctx, -2, PROTO);
	duk_pop_n(ctx, 3);
}
