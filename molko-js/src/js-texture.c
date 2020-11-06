/*
 * js-texture.c -- basic texture management (Javascript bindings)
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
#include <core/image.h>
#include <core/texture.h>

#include "js.h"
#include "js_p.h"
#include "js-texture.h"

#define SYMBOL DUK_HIDDEN_SYMBOL("molko::texture")
#define PROTO  DUK_HIDDEN_SYMBOL("molko::texture::proto")

static struct texture *
js_texture_this(duk_context *ctx)
{
	struct texture *tex;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SYMBOL);
	tex = duk_to_pointer(ctx, -1);
	duk_pop_n(ctx, 2);

	if (!tex)
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "Not a Texture object");

	return tex;
}

static duk_ret_t
js_texture_new(duk_context *ctx)
{
	struct texture tex;
	unsigned int w = duk_require_uint(ctx, 0);
	unsigned int h = duk_require_uint(ctx, 1);

	if (!duk_is_constructor_call(ctx))
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "Texture must be new-constructed");

	if (!texture_new(&tex, w, h))
		duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	duk_push_this(ctx);
	duk_push_pointer(ctx, alloc_dup(&tex, sizeof (tex)));
	duk_put_prop_string(ctx, -2, SYMBOL);
	duk_pop(ctx);

	return 0;
}

static duk_ret_t
js_texture_draw(duk_context *ctx)
{
	struct texture *tex = js_texture_this(ctx);
	int x, y;

	duk_require_object(ctx, 0);
	duk_get_prop_string(ctx, 0, "x");
	x = duk_to_int(ctx, -1);
	duk_get_prop_string(ctx, 0, "y");
	y = duk_to_int(ctx, -1);
	duk_pop_n(ctx, 2);

	texture_draw(tex, x, y);

	return 0;
}

static duk_ret_t
js_texture_scale(duk_context *ctx)
{
	struct texture *tex = js_texture_this(ctx);
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
	angle = duk_to_number(ctx, 2);

	texture_scale(tex, srcx, srcy, srcw, srch, dstx, dsty, dstw, dsth, angle);

	return 0;
}

static duk_ret_t
js_texture_finish(duk_context *ctx)
{
	struct texture *tex;

	duk_get_prop_string(ctx, 0, SYMBOL);
	tex = duk_to_pointer(ctx, -1);
	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SYMBOL);

	if (tex) {
		texture_finish(tex);
		free(tex);
	}

	return 0;
}

static duk_ret_t
js_texture_fromImage(duk_context *ctx)
{
	const char *path = duk_require_string(ctx, 0);
	struct texture tex;

	if (!image_open(&tex, path))
		duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	js_texture_push(js_self(ctx), &tex);

	return 1;
}

static const duk_function_list_entry methods[] = {
	{ "draw",       js_texture_draw,        1 },
	{ "scale",      js_texture_scale,       3 },
	{ NULL,         NULL,                   0 }
};

static const duk_function_list_entry functions[] = {
	{ "fromImage",  js_texture_fromImage,   1 },
	{ NULL,         NULL,                   0 }
};

struct texture *
js_texture_require(struct js *js, unsigned int index)
{
	struct texture *tex;

	duk_get_prop_string(js->handle, index, SYMBOL);
	tex = duk_to_pointer(js->handle, -1);
	duk_pop(js->handle);

	if (!tex)
		duk_error(js->handle, DUK_ERR_TYPE_ERROR, "Texture expected on argument #%u", index);

	return tex;
}

void
js_texture_push(struct js *js, const struct texture *tex)
{
	assert(js);
	assert(texture_ok(tex));

	duk_push_object(js->handle);
	duk_push_global_stash(js->handle);
	duk_get_prop_string(js->handle, -1, PROTO);
	duk_remove(js->handle, -2);
	duk_push_pointer(js->handle, alloc_dup(tex, sizeof (*tex)));
	duk_put_prop_string(js->handle, -3, SYMBOL);
	duk_set_prototype(js->handle, -2);
}

void
js_texture_load(struct js *js)
{
	assert(js);

	duk_push_global_object(js->handle);
	duk_get_prop_string(js->handle, -1, "Molko");
	duk_push_c_function(js->handle, js_texture_new, 2);
	duk_put_function_list(js->handle, -1, functions);
	duk_push_object(js->handle);
	duk_put_function_list(js->handle, -1, methods);
	duk_push_c_function(js->handle, js_texture_finish, 1);
	duk_set_finalizer(js->handle, -2);
	duk_push_global_stash(js->handle);
	duk_dup(js->handle, -2);
	duk_put_prop_string(js->handle, -2, PROTO);
	duk_pop(js->handle);
	duk_put_prop_string(js->handle, -2, "prototype");
	duk_put_prop_string(js->handle, -2, "Texture");
	duk_pop_n(js->handle, 2);
}
