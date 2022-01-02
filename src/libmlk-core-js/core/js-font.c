/*
 * js-font.c -- core texture binding
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

#include <core/alloc.h>
#include <core/error.h>
#include <core/font.h>
#include <core/texture.h>
#include <core/vfs.h>

#include "js-core.h"
#include "js-font.h"
#include "js-texture.h"

#define SIGNATURE DUK_HIDDEN_SYMBOL("Mlk.Font")

static inline struct font *
self(duk_context *ctx)
{
	struct font *font;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SIGNATURE);
	font = duk_to_pointer(ctx, -1);
	duk_pop_2(ctx);

	if (!font)
		return (void)duk_error(ctx, DUK_ERR_TYPE_ERROR, "not a Font object"), NULL;

	return font;
}

static duk_ret_t
Font_getStyle(duk_context *ctx)
{
	duk_push_int(ctx, self(ctx)->style);

	return 1;
}

static duk_ret_t
Font_setStyle(duk_context *ctx)
{
	const int style = duk_require_int(ctx, 0);

	if (style < 0 || style >= FONT_STYLE_LAST)
		return duk_error(ctx, DUK_ERR_ERROR, "invalid style");

	self(ctx)->style = style;

	return 0;
}

static duk_ret_t
Font_getHeight(duk_context *ctx)
{
	duk_push_uint(ctx, font_height(self(ctx)));

	return 1;
}

static duk_ret_t
Font_new(duk_context *ctx)
{
	const char *entry = duk_require_string(ctx, 0);
	const unsigned int size = duk_require_uint(ctx, 1);
	struct vfs_file file;
	struct font *font;

	if (vfs_open(js_core_global_vfs(ctx), &file, entry, "r") < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	font = alloc_new0(sizeof (*font));

	if (font_openvfs(font, &file, size) < 0) {
		free(font);
		vfs_file_finish(&file);

		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());
	}

	vfs_file_finish(&file);

	duk_push_this(ctx);
	duk_push_pointer(ctx, font);
	duk_put_prop_string(ctx, -2, SIGNATURE);
	duk_push_string(ctx, "style");
	duk_push_c_function(ctx, Font_getStyle, 0);
	duk_push_c_function(ctx, Font_setStyle, 1);
	duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);
	duk_push_string(ctx, "height");
	duk_push_c_function(ctx, Font_getHeight, 0);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_GETTER);
	duk_pop(ctx);

	return 0;
}

static duk_ret_t
Font_render(duk_context *ctx)
{
	const char *text = duk_require_string(ctx, 0);
	unsigned long color = duk_require_uint(ctx, 1);
	struct texture *tex;

	tex = alloc_new0(sizeof (*tex));

	if (font_render(self(ctx), tex, text, color) < 0) {
		free(tex);
		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());
	}

	js_texture_push(ctx, tex);

	return 1;
}

static duk_ret_t
Font_query(duk_context *ctx)
{
	const char *text = duk_require_string(ctx, 0);
	unsigned int w, h;

	if (font_query(self(ctx), text, &w, &h) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	duk_push_object(ctx);
	duk_push_uint(ctx, w);
	duk_put_prop_string(ctx, -2, "w");
	duk_push_uint(ctx, h);
	duk_put_prop_string(ctx, -2, "h");

	return 1;
}

static duk_ret_t
Font_destructor(duk_context *ctx)
{
	struct font *font;

	duk_get_prop_string(ctx, 0, SIGNATURE);

	if ((font = duk_to_pointer(ctx, -1)))
		font_finish(font);

	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SIGNATURE);

	return 0;
}

static const struct duk_function_list_entry methods[] = {
	{ "render",     Font_render,    2 },
	{ "query",      Font_query,     1 },
	{ NULL,         NULL,           0 }
};

static const struct duk_number_list_entry styles[] = {
	{ "ANTIALIASED",        FONT_STYLE_ANTIALIASED, },
	{ "NONE",               FONT_STYLE_NONE         },
	{ NULL,                 0                       }
};

void
js_font_bind(duk_context *ctx)
{
	assert(ctx);

	duk_push_c_function(ctx, Font_new, 2);
	duk_push_object(ctx);
	duk_put_number_list(ctx, -1, styles);
	duk_put_prop_string(ctx, -2, "Style");
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, methods);
	duk_push_c_function(ctx, Font_destructor, 1);
	duk_set_finalizer(ctx, -2);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_global_string(ctx, "Font");
}
