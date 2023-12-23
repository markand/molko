/*
 * js-texture.c -- basic texture management (Javascript bindings)
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

#include <mlk/core/alloc.h>
#include <mlk/core/err.h>
#include <mlk/core/image.h>
#include <mlk/core/texture.h>
#include <mlk/core/vfs.h>

#include "js-texture.h"
#include "js.h"

#define SYMBOL DUK_HIDDEN_SYMBOL("mlk::texture")
#define PROTO  DUK_HIDDEN_SYMBOL("mlk::texture::proto")

static struct mlk_texture *
mlk_js_texture_this(duk_context *ctx)
{
	struct mlk_texture *tex;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SYMBOL);
	tex = duk_to_pointer(ctx, -1);
	duk_pop_n(ctx, 2);

	if (!tex)
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "Not a Texture object");

	return tex;
}

static duk_ret_t
mlk_js_texture_getWidth(duk_context *ctx)
{
	duk_push_uint(ctx, mlk_js_texture_this(ctx)->w);

	return 1;
}

static duk_ret_t
mlk_js_texture_getHeight(duk_context *ctx)
{
	duk_push_uint(ctx, mlk_js_texture_this(ctx)->h);

	return 1;
}

static duk_ret_t
mlk_js_texture_setBlendMode(duk_context *ctx)
{
	struct mlk_texture *tex = mlk_js_texture_this(ctx);
	enum mlk_texture_blend blend = duk_require_int(ctx, 0);

	if (blend < 0 || blend >= MLK_TEXTURE_BLEND_MODULATE)
		return duk_error(ctx, DUK_ERR_ERROR, "invalid blend mode: %d", blend);
	if (mlk_texture_set_blend_mode(tex, blend) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());

	return 0;
}

static duk_ret_t
mlk_js_texture_setAlphaMod(duk_context *ctx)
{
	struct mlk_texture *tex = mlk_js_texture_this(ctx);
	unsigned int alpha = duk_require_uint(ctx, 0);

	if (alpha > 255)
		return duk_error(ctx, DUK_ERR_RANGE_ERROR, "%u is out of range [0..255]", alpha);
	if (mlk_texture_set_alpha_mod(tex, alpha) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());

	return 0;
}

static duk_ret_t
mlk_js_texture_setColorMod(duk_context *ctx)
{
	struct mlk_texture *tex = mlk_js_texture_this(ctx);
	unsigned long color = duk_require_number(ctx, 0);

	if (mlk_texture_set_color_mod(tex, color) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());

	return 0;
}

static duk_ret_t
mlk_js_texture_new(duk_context *ctx)
{
	struct mlk_texture tex;
	unsigned int w = duk_require_uint(ctx, 0);
	unsigned int h = duk_require_uint(ctx, 1);

	if (!duk_is_constructor_call(ctx))
		return duk_error(ctx, DUK_ERR_TYPE_ERROR, "Texture must be new-constructed");
	if (mlk_texture_init(&tex, w, h) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());

	duk_push_this(ctx);
	duk_push_pointer(ctx, mlk_alloc_dup(&tex, 1, sizeof (tex)));
	duk_put_prop_string(ctx, -2, SYMBOL);
	duk_pop(ctx);

	return 0;
}

static duk_ret_t
mlk_js_texture_draw(duk_context *ctx)
{
	struct mlk_texture *tex = mlk_js_texture_this(ctx);
	int x, y;

	duk_require_object(ctx, 0);
	duk_get_prop_string(ctx, 0, "x");
	x = duk_to_int(ctx, -1);
	duk_get_prop_string(ctx, 0, "y");
	y = duk_to_int(ctx, -1);
	duk_pop_n(ctx, 2);

	if (mlk_texture_draw(tex, x, y) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());

	return 0;
}

static duk_ret_t
mlk_js_texture_scale(duk_context *ctx)
{
	struct mlk_texture *tex = mlk_js_texture_this(ctx);
	int srcx, srcy, dstx, dsty;
	unsigned int srcw, srch, dstw, dsth;
	double angle;

	/* Source rectangle. */
	duk_require_object(ctx, 0);
	duk_get_prop_string(ctx, 0, "x");
	srcx = duk_to_int(ctx, -1);
	duk_get_prop_string(ctx, 0, "y");
	srcy = duk_to_int(ctx, -1);
	duk_get_prop_string(ctx, 0, "w");
	srcw = duk_is_number(ctx, -1) ? duk_to_uint(ctx, -1) : tex->w;
	duk_get_prop_string(ctx, 0, "h");
	srch = duk_is_number(ctx, -1) ? duk_to_uint(ctx, -1) : tex->h;
	duk_pop_n(ctx, 4);

	/* Destination rectangle. */
	duk_require_object(ctx, 1);
	duk_get_prop_string(ctx, 1, "x");
	dstx = duk_to_int(ctx, -1);
	duk_get_prop_string(ctx, 1, "y");
	dsty = duk_to_int(ctx, -1);
	duk_get_prop_string(ctx, 1, "w");
	dstw = duk_is_number(ctx, -1) ? duk_to_uint(ctx, -1) : tex->w;
	duk_get_prop_string(ctx, 1, "h");
	dsth = duk_is_number(ctx, -1) ? duk_to_uint(ctx, -1) : tex->h;
	duk_pop_n(ctx, 4);

	/* Angle. */
	angle = duk_is_number(ctx, 2) ? duk_to_number(ctx, 2) : 0;

	if (mlk_texture_scale(tex, srcx, srcy, srcw, srch, dstx, dsty, dstw, dsth, angle) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());

	return 0;
}

static duk_ret_t
mlk_js_texture_finish(duk_context *ctx)
{
	struct mlk_texture *tex;

	duk_get_prop_string(ctx, 0, SYMBOL);
	tex = duk_to_pointer(ctx, -1);
	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SYMBOL);

	if (tex) {
		mlk_texture_finish(tex);
		mlk_alloc_free(tex);
	}

	return 0;
}

static duk_ret_t
mlk_js_texture_fromImage(duk_context *ctx)
{
	const char *path = duk_require_string(ctx, 0);
	struct mlk_texture tex;

	if (mlk_image_open(&tex, path) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());

	mlk_js_texture_push(ctx, &tex);

	return 1;
}

static duk_ret_t
mlk_js_texture_fromVFS(duk_context *ctx)
{
	const char *path = duk_require_string(ctx, 0);
	struct mlk_vfs *vfs = mlk_js_require(ctx)->vfs;
	struct mlk_vfs_file *file;
	struct mlk_texture tex;
	int rv;

	if (!(file = mlk_vfs_open(vfs, path, "r")))
		return duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());

	rv = mlk_image_openvfs(&tex, file);
	mlk_vfs_file_finish(file);

	if (rv < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());

	mlk_js_texture_push(ctx, &tex);

	return 1;
}

static const duk_function_list_entry methods[] = {
	{ "draw",       mlk_js_texture_draw,            1 },
	{ "scale",      mlk_js_texture_scale,           3 },
	{ NULL,         NULL,                           0 }
};

static const duk_function_list_entry functions[] = {
	{ "fromImage",  mlk_js_texture_fromImage,       1 },
	{ "fromVFS",    mlk_js_texture_fromVFS,         1 },
	{ NULL,         NULL,                           0 }
};

struct mlk_texture *
mlk_js_texture_require(duk_context *ctx, duk_idx_t index)
{
	struct mlk_texture *tex;

	duk_get_prop_string(ctx, index, SYMBOL);
	tex = duk_to_pointer(ctx, -1);
	duk_pop(ctx);

	if (!tex)
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "Texture expected on argument #%u", index);

	return tex;
}

void
mlk_js_texture_push(duk_context *ctx, const struct mlk_texture *tex)
{
	assert(ctx);
	assert(tex);

	duk_push_object(ctx);
	duk_push_global_stash(ctx);
	duk_get_prop_string(ctx, -1, PROTO);
	duk_remove(ctx, -2);
	duk_push_pointer(ctx, mlk_alloc_dup(tex, 1, sizeof (*tex)));
	duk_put_prop_string(ctx, -3, SYMBOL);
	duk_set_prototype(ctx, -2);

	/* Put some properties. */
	duk_push_string(ctx, "width");
	duk_push_c_function(ctx, mlk_js_texture_getWidth, 0);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_GETTER);
	duk_push_string(ctx, "height");
	duk_push_c_function(ctx, mlk_js_texture_getHeight, 0);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_GETTER);
	duk_push_string(ctx, "blendMode");
	duk_push_c_function(ctx, mlk_js_texture_setBlendMode, 1);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_SETTER);
	duk_push_string(ctx, "alphaMod");
	duk_push_c_function(ctx, mlk_js_texture_setAlphaMod, 1);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_SETTER);
	duk_push_string(ctx, "colorMod");
	duk_push_c_function(ctx, mlk_js_texture_setColorMod, 1);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_SETTER);
}

void
mlk_js_texture_load(duk_context *ctx)
{
	assert(ctx);

	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1, "Mlk");
	duk_push_c_function(ctx, mlk_js_texture_new, 2);
	duk_put_function_list(ctx, -1, functions);
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, methods);
	duk_push_c_function(ctx, mlk_js_texture_finish, 1);
	duk_set_finalizer(ctx, -2);
	duk_push_global_stash(ctx);
	duk_dup(ctx, -2);
	duk_put_prop_string(ctx, -2, PROTO);
	duk_pop(ctx);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_prop_string(ctx, -2, "Texture");
	duk_pop_n(ctx, 2);
}
