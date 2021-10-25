/*
 * js-drawable.c -- core drawable binding
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

#include <core/alloc.h>
#include <core/drawable.h>

#include "js-drawable.h"

#define SIGNATURE DUK_HIDDEN_SYMBOL("Mlk.Drawable")

static inline struct js_drawable *
self(duk_context *ctx)
{
	struct js_drawable *data = NULL;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SIGNATURE);
	data = duk_to_pointer(ctx, -1);
	duk_pop_2(ctx);

	if (!data)
		return (void)duk_error(ctx, DUK_ERR_TYPE_ERROR, "not a Drawable object"), NULL;

	return data;
}

static inline duk_context *
callable(struct js_drawable *data, const char *prop)
{
	duk_context *ctx;

	if (!data->ptr)
		return NULL;

	duk_push_heapptr(data->ctx, data->ptr);
	duk_get_prop_string(data->ctx, -1, prop);
	duk_pull(data->ctx, -2);

	if (duk_is_callable(data->ctx, -2))
		ctx = data->ctx;
	else {
		ctx = NULL;
		duk_pop_n(data->ctx, 2);
	}

	return ctx;
}

static int
update(struct drawable *dw, unsigned int ticks)
{
	duk_context *ctx;
	int ret = 0;

	if ((ctx = callable(dw->data, "update"))) {
		duk_push_uint(ctx, ticks);
		duk_call_method(ctx, 1);
		ret = duk_to_int(ctx, -1);
		duk_pop(ctx);
	}

	return ret;
}

static void
draw(struct drawable *dw)
{
	duk_context *ctx;

	if ((ctx = callable(dw->data, "draw"))) {
		duk_call_method(ctx, 0);
		duk_pop(ctx);
	}
}

static void
end(struct drawable *dw)
{
	duk_context *ctx;

	if ((ctx = callable(dw->data, "end"))) {
		duk_call_method(ctx, 0);
		duk_pop(ctx);
	}
}

static void
finish(struct drawable *dw)
{
	struct js_drawable *data = dw->data;

	/* I must not be called anymore. */
	data->ptr = NULL;

	/* Remove myself from parent stack if any. */
	if (data->parent) {
		duk_push_heapptr(data->ctx, data->parent);
		duk_push_sprintf(data->ctx, "%p", data);
		duk_del_prop(data->ctx, -2);
		duk_pop(data->ctx);
		data->parent = NULL;
	}

	if (--data->refc == 0)
		free(data);
}

static duk_ret_t
Drawable_getX(duk_context *ctx)
{
	duk_push_uint(ctx, self(ctx)->dw.x);

	return 1;
}

static duk_ret_t
Drawable_setX(duk_context *ctx)
{
	self(ctx)->dw.x = duk_require_uint(ctx, 0);

	return 0;
}

static duk_ret_t
Drawable_getY(duk_context *ctx)
{
	duk_push_uint(ctx, self(ctx)->dw.y);

	return 1;
}

static duk_ret_t
Drawable_setY(duk_context *ctx)
{
	self(ctx)->dw.y = duk_require_uint(ctx, 0);

	return 0;
}

static duk_ret_t
Drawable_constructor(duk_context *ctx)
{
	struct js_drawable *data;
	const int x = duk_require_int(ctx, 0);
	const int y = duk_require_int(ctx, 1);

	data = alloc_new0(sizeof (*data));
	data->refc = 1;
	data->ctx = ctx;
	data->dw.x = x;
	data->dw.y = y;
	data->dw.data = self;
	data->dw.update = update;
	data->dw.finish = finish;
	data->dw.draw = draw;
	data->dw.end = end;

	duk_push_this(ctx);
	data->ptr = duk_get_heapptr(ctx, -1);
	duk_push_pointer(ctx, self);
	duk_put_prop_string(ctx, -2, SIGNATURE);
	duk_push_string(ctx, "x");
	duk_push_c_function(ctx, Drawable_getX, 0);
	duk_push_c_function(ctx, Drawable_setX, 1);
	duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);
	duk_push_string(ctx, "y");
	duk_push_c_function(ctx, Drawable_getY, 0);
	duk_push_c_function(ctx, Drawable_setY, 1);
	duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);
	duk_pop(ctx);

	return 0;
}

static duk_ret_t
Drawable_destructor(duk_context *ctx)
{
	struct js_drawable *data;

	duk_get_prop_string(ctx, 0, SIGNATURE);

	if ((data = duk_to_pointer(ctx, -1)))
		drawable_finish(&data->dw);

	duk_del_prop_string(ctx, 0, SIGNATURE);
	duk_pop(ctx);

	return 0;
}

void
js_drawable_bind(duk_context *ctx)
{
	assert(ctx);

	duk_push_c_function(ctx, Drawable_constructor, 2);
	duk_push_object(ctx);
	duk_push_c_function(ctx, Drawable_destructor, 1);
	duk_set_finalizer(ctx, -2);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_global_string(ctx, "Drawable");
}

struct js_drawable *
js_drawable_require(duk_context *ctx, duk_idx_t idx)
{
	struct js_drawable *data = NULL;

	if (duk_is_object(ctx, idx)) {
		duk_get_prop_string(ctx, idx, SIGNATURE);
		data = duk_to_pointer(ctx, -1);
		duk_pop(ctx);
	}

	if (!data)
		return (void)duk_error(ctx, DUK_ERR_TYPE_ERROR, "not a Drawable object"), NULL;

	data->refc++;

	return data;
}
