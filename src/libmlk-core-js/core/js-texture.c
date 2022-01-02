/*
 * js-texture.c -- core texture binding
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
#include <core/image.h>
#include <core/texture.h>
#include <core/vfs.h>

#include "js-core.h"
#include "js-texture.h"

#define SIGNATURE DUK_HIDDEN_SYMBOL("Mlk.Texture")
#define PROTOTYPE DUK_HIDDEN_SYMBOL("Mlk.Texture.Prototype")

static inline struct texture *
self(duk_context *ctx)
{
	struct texture *tex;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SIGNATURE);
	tex = duk_to_pointer(ctx, -1);
	duk_pop_2(ctx);

	if (!tex)
		return (void)duk_error(ctx, DUK_ERR_TYPE_ERROR, "not a Texture object"), NULL;

	return tex;
}

static duk_ret_t
Texture_constructor(duk_context *ctx)
{
	const unsigned int w = duk_require_uint(ctx, 0);
	const unsigned int h = duk_require_uint(ctx, 1);
	struct texture *tex;

	tex = alloc_new(sizeof (*tex));

	if (texture_new(tex, w, h) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	duk_push_this(ctx);
	duk_push_pointer(ctx, tex);
	duk_put_prop_string(ctx, -2, SIGNATURE);
	duk_pop(ctx);

	return 0;
}

static duk_ret_t
Texture_setBlendMode(duk_context *ctx)
{
	const int blend = duk_require_uint(ctx, 0);
	struct texture *tex = self(ctx);

	if (blend < 0 || blend >= TEXTURE_BLEND_LAST)
		return duk_error(ctx, DUK_ERR_ERROR, "invalid blend");
	if (texture_set_blend_mode(tex, blend) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	return 0;
}

static duk_ret_t
Texture_setAlphaMod(duk_context *ctx)
{
	const unsigned int alpha = duk_require_uint(ctx, 0);
	struct texture *tex = self(ctx);

	if (texture_set_alpha_mod(tex, alpha) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	return 0;
}

static duk_ret_t
Texture_setColorMod(duk_context *ctx)
{
	const unsigned long color = duk_require_uint(ctx, 0);
	struct texture *tex = self(ctx);

	if (texture_set_color_mod(tex, color) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	return 0;
}

static duk_ret_t
Texture_draw(duk_context *ctx)
{
	const int x = duk_require_int(ctx, 0);
	const int y = duk_require_int(ctx, 1);
	struct texture *tex = self(ctx);

	if (texture_draw(tex, x, y) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	return 0;
}

static duk_ret_t
Texture_scale(duk_context *ctx)
{
	const int srcx      = duk_require_int(ctx, 0);
	const int srcy      = duk_require_int(ctx, 1);
	const unsigned srcw = duk_require_uint(ctx, 2);
	const unsigned srch = duk_require_uint(ctx, 3);
	const int dstx      = duk_require_int(ctx, 4);
	const int dsty      = duk_require_int(ctx, 5);
	const unsigned dstw = duk_require_uint(ctx, 6);
	const unsigned dsth = duk_require_uint(ctx, 7);
	const double angle  = duk_get_number_default(ctx, 8, .0);
	struct texture *tex = self(ctx);

	if (texture_scale(tex, srcx, srcy, srcw, srch, dstx, dsty, dstw, dsth, angle) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	return 0;
}

static duk_ret_t
Texture_destructor(duk_context *ctx)
{
	struct texture *tex;

	duk_get_prop_string(ctx, 0, SIGNATURE);

	if ((tex = duk_to_pointer(ctx, -1)))
		texture_finish(tex);

	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SIGNATURE);

	return 0;
}

static duk_ret_t
Texture_fromImage(duk_context *ctx)
{
	const char *entry = duk_require_string(ctx, 0);
	struct texture *tex;
	struct vfs_file file;

	if (vfs_open(js_core_global_vfs(ctx), &file, entry, "r") < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	tex = alloc_new0(sizeof (*tex));

	if (image_openvfs(tex, &file) < 0) {
		free(tex);
		vfs_file_finish(&file);

		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());
	}

	vfs_file_finish(&file);
	js_texture_push(ctx, tex);

	return 1;
}

static const struct duk_function_list_entry methods[] = {
	{ "setBlendMode",       Texture_setBlendMode,   1               },
	{ "setAlphaMod",        Texture_setAlphaMod,    1               },
	{ "setColorMod",        Texture_setColorMod,    1               },
	{ "draw",               Texture_draw,           2               },
	{ "scale",              Texture_scale,          DUK_VARARGS     },
	{ NULL,                 NULL,                   0               }
};

static const struct duk_function_list_entry functions[] = {
	{ "fromImage",          Texture_fromImage,      1               },
	{ NULL,                 NULL,                   0               }
};

static const duk_number_list_entry blend[] = {
	{ "NONE",       TEXTURE_BLEND_NONE      },
	{ "BLEND",      TEXTURE_BLEND_BLEND     },
	{ "ADD",        TEXTURE_BLEND_ADD       },
	{ "MODULATE",   TEXTURE_BLEND_MODULATE  },
	{ NULL,         0                       }
};

void
js_texture_bind(duk_context *ctx)
{
	assert(ctx);

	duk_push_c_function(ctx, Texture_constructor, 2);
	duk_put_function_list(ctx, -1, functions);
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, methods);
	duk_push_c_function(ctx, Texture_destructor, 1);
	duk_set_finalizer(ctx, -2);
	duk_push_object(ctx);
	duk_put_number_list(ctx, -1, blend);
	duk_put_prop_string(ctx, -2, "Blend");
	duk_dup(ctx, -1);
	duk_put_global_string(ctx, PROTOTYPE);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_global_string(ctx, "Texture");
}

struct texture *
js_texture_require(duk_context *ctx, duk_idx_t idx)
{
	struct texture *tex;

	duk_get_prop_string(ctx, idx, SIGNATURE);
	tex = duk_to_pointer(ctx, -1);
	duk_pop(ctx);

	if (!tex)
		return (void)duk_error(ctx, DUK_ERR_TYPE_ERROR, "not a Texture object"), NULL;

	return tex;
}

void
js_texture_push(duk_context *ctx, struct texture *tex)
{
	assert(ctx);
	assert(tex);

	duk_push_object(ctx);
	duk_push_pointer(ctx, tex);
	duk_put_prop_string(ctx, -2, SIGNATURE);
	duk_get_global_string(ctx, PROTOTYPE);
	duk_set_prototype(ctx, -2);
}
