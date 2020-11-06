/*
 * js-font.c -- basic font management (Javascript bindings)
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

#include <core/alloc.h>
#include <core/error.h>
#include <core/font.h>
#include <core/texture.h>

#include "js-font.h"
#include "js-texture.h"
#include "js.h"
#include "js_p.h"

#define SYMBOL  DUK_HIDDEN_SYMBOL("molko::font")
#define PROTO   DUK_HIDDEN_SYMBOL("molko::font::prototype")

static struct font *
js_font_this(duk_context *ctx)
{
	struct font *font;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SYMBOL);
	font = duk_to_pointer(ctx, -1);
	duk_pop_n(ctx, 2);

	if (!font)
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "Not a Font object");

	return font;
}

static duk_ret_t
js_font_getHeight(duk_context *ctx)
{
	duk_push_uint(ctx, font_height(js_font_this(ctx)));

	return 1;
}

static duk_ret_t
js_font_fromPath(duk_context *ctx)
{
	struct font font;
	const char *path = duk_require_string(ctx, 0);
	unsigned int size = duk_require_uint(ctx, 1);

	if (!font_open(&font, path, size))
		duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	duk_push_object(ctx);
	duk_push_global_stash(ctx);
	duk_get_prop_string(ctx, -1, PROTO);
	duk_remove(ctx, -2);
	duk_push_pointer(ctx, alloc_dup(&font, sizeof (font)));
	duk_put_prop_string(ctx, -3, SYMBOL);
	duk_set_prototype(ctx, -2);
	duk_push_string(ctx, "height");
	duk_push_c_function(ctx, js_font_getHeight, 0);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_GETTER);
	
	return 1;
}

static duk_ret_t
js_font_render(duk_context *ctx)
{
	struct font *font = js_font_this(ctx);
	struct texture tex;
	const char *text = duk_require_string(ctx, 0);
	unsigned long color = duk_require_number(ctx, 1);

	if (!font_render(font, &tex, text, color))
		duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	js_texture_push(js_self(ctx), &tex);

	return 1;
}

static duk_ret_t
js_font_query(duk_context *ctx)
{
	struct font *font = js_font_this(ctx);
	const char *text = duk_require_string(ctx, 0);
	unsigned int w, h;

	if (!font_query(font, text, &w, &h))
		duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	duk_push_object(ctx);
	duk_push_uint(ctx, w);
	duk_put_prop_string(ctx, -2, "w");
	duk_push_uint(ctx, h);
	duk_put_prop_string(ctx, -2, "h");

	return 1;
}

static duk_ret_t
js_font_finish(duk_context *ctx)
{
	struct font *font;

	duk_get_prop_string(ctx, 0, SYMBOL);
	font = duk_to_pointer(ctx, -1);
	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SYMBOL);

	if (font) {
		font_finish(font);
		free(font);
	}

	return 0;
}

static const duk_function_list_entry functions[] = {
	{ "fromPath",   js_font_fromPath,       2 },
	{ NULL,         NULL,                   0 }
};

static const duk_function_list_entry methods[] = {
	{ "render",     js_font_render,         2 },
	{ "query",      js_font_query,          1 },
	{ NULL, NULL, 0 }
};

void
js_font_load(struct js *js)
{
	assert(js);

	duk_push_global_object(js->handle);
	duk_get_prop_string(js->handle, -1, "Molko");
	duk_push_object(js->handle);
	duk_put_function_list(js->handle, -1, functions);
	duk_put_prop_string(js->handle, -2, "Font");
	duk_push_global_stash(js->handle);
	duk_push_object(js->handle);
	duk_put_function_list(js->handle, -1, methods);
	duk_push_c_function(js->handle, js_font_finish, 1);
	duk_set_finalizer(js->handle, -2);
	duk_put_prop_string(js->handle, -2, PROTO);
	duk_pop_n(js->handle, 3);
}
