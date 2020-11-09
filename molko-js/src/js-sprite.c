/*
 * js-sprite.c -- image sprites (Javascript bindings)
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
#include <core/sprite.h>
#include <core/texture.h>

#include "js-sprite.h"
#include "js-texture.h"
#include "js.h"
#include "js_p.h"

#define SYMBOL          DUK_HIDDEN_SYMBOL("molko::sprite")
#define TEXTURE_REF     DUK_HIDDEN_SYMBOL("molko::sprite::texture")

static struct sprite *
js_sprite_this(duk_context *ctx)
{
	struct sprite *sprite;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SYMBOL);
	sprite = duk_to_pointer(ctx, -1);
	duk_pop_n(ctx, 2);

	if (!sprite)
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "Not a Sprite object");

	return sprite;
}

static duk_ret_t
js_sprite_getRowCount(duk_context *ctx)
{
	duk_push_uint(ctx, js_sprite_this(ctx)->nrows);

	return 1;
}

static duk_ret_t
js_sprite_getColumnCount(duk_context *ctx)
{
	duk_push_uint(ctx, js_sprite_this(ctx)->ncols);

	return 1;
}

static duk_ret_t
js_sprite_new(duk_context *ctx)
{
	struct js *js = js_self(ctx);
	struct texture *tex;
	struct sprite sprite;
	unsigned int cellw, cellh;

	if (!duk_is_constructor_call(ctx))
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "Sprite must be new-constructed");

	tex = js_texture_require(js, 0);
	cellw = duk_require_int(ctx, 1);
	cellh = duk_require_int(ctx, 2);

	sprite_init(&sprite, tex, cellw, cellh);

	duk_push_this(ctx);
	duk_push_pointer(ctx, alloc_dup(&sprite, sizeof (sprite)));
	duk_put_prop_string(ctx, -2, SYMBOL);

	/*
	 * Put rowCount/columnCount properties.
	 */
	duk_push_string(ctx, "rowCount");
	duk_push_c_function(ctx, js_sprite_getRowCount, 0);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_GETTER);

	duk_push_string(ctx, "columnCount");
	duk_push_c_function(ctx, js_sprite_getColumnCount, 0);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_GETTER);

	/* We store the texture into the sprite to avoid being collected. */
	duk_dup(ctx, 0);
	duk_put_prop_string(ctx, -2, TEXTURE_REF);
	duk_pop(ctx);

	return 0;
}

static duk_ret_t
js_sprite_finish(duk_context *ctx)
{
	duk_get_prop_string(ctx, 0, SYMBOL);
	free(duk_to_pointer(ctx, -1));
	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SYMBOL);
	duk_del_prop_string(ctx, 0, TEXTURE_REF);

	return 0;
}

static duk_ret_t
js_sprite_draw(duk_context *ctx)
{
	struct sprite *sprite = js_sprite_this(ctx);
	unsigned int r, c;
	int x, y;

	if (duk_get_top(ctx) == 4) {
		r = duk_require_uint(ctx, 0);
		c = duk_require_uint(ctx, 1);
		x = duk_require_int(ctx, 2);
		y = duk_require_int(ctx, 3);
	} else if (duk_get_top(ctx) == 1) {
		duk_require_object(ctx, 0);
		duk_get_prop_string(ctx, 0, "row");
		r = duk_to_int(ctx, -1);
		duk_get_prop_string(ctx, 0, "column");
		c = duk_to_int(ctx, -1);
		duk_get_prop_string(ctx, 0, "x");
		x = duk_to_int(ctx, -1);
		duk_get_prop_string(ctx, 0, "y");
		y = duk_to_int(ctx, -1);
		duk_pop_n(ctx, 4);
	} else
		return duk_error(ctx, DUK_ERR_ERROR, "Object or 4 numbers expected");

	sprite_draw(sprite, r, c, x, y);
	
	return 0;
}

static const duk_function_list_entry methods[] = {
	{ "draw",       js_sprite_draw, DUK_VARARGS     },
	{ NULL,         NULL,           0               }
};

struct sprite *
js_sprite_require(struct js *js, unsigned int index)
{
	struct sprite *sp;

	duk_get_prop_string(js->handle, index, SYMBOL);
	sp = duk_to_pointer(js->handle, -1);
	duk_pop(js->handle);

	if (!sp)
		duk_error(js->handle, DUK_ERR_TYPE_ERROR, "Sprite expected on argument #%u", index);

	return sp;
}

void
js_sprite_load(struct js *js)
{
	assert(js);

	duk_push_global_object(js->handle);
	duk_get_prop_string(js->handle, -1, "Molko");
	duk_push_c_function(js->handle, js_sprite_new, 3);
	duk_push_object(js->handle);
	duk_put_function_list(js->handle, -1, methods);
	duk_push_c_function(js->handle, js_sprite_finish, 1);
	duk_set_finalizer(js->handle, -2);
	duk_put_prop_string(js->handle, -2, "prototype");
	duk_put_prop_string(js->handle, -2, "Sprite");
	duk_pop_n(js->handle, 2);
}
