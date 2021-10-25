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

struct self {
	duk_context *ctx;
	void *ptr;
	struct drawable dw;
	unsigned int refc;
};

static inline struct self *
self(duk_context *ctx)
{
	struct self *sf = NULL;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SIGNATURE);
	sf = duk_to_pointer(ctx, -1);
	duk_pop_2(ctx);

	if (!sf)
		return (void)duk_error(ctx, DUK_ERR_TYPE_ERROR, "not a Drawable object"), NULL;

	return sf;
}

static inline int
callable(struct self *s, const char *prop, duk_context **ctx)
{
	int callable;

	if (!s->ptr)
		return 0;

	duk_push_heapptr(s->ctx, s->ptr);
	duk_get_prop_string(s->ctx, -1, prop);
	duk_remove(s->ctx, -2);

	if (duk_is_callable(s->ctx, -1)) {
		*ctx = s->ctx;
		callable = 1;
	} else {
		*ctx = NULL;
		callable = 0;
		duk_pop(s->ctx);
	}

	return callable;
}

static int
update(struct drawable *dw, unsigned int ticks)
{
	duk_context *ctx;
	int ret = 0;

	if (callable(dw->data, "update", &ctx)) {
		duk_push_uint(ctx, ticks);
		duk_call(ctx, 1);
		ret = duk_to_int(ctx, -1);
	}

	return ret;
}

static void
draw(struct drawable *dw)
{
	duk_context *ctx;

	if (callable(dw->data, "draw", &ctx))
		duk_call(ctx, 0);
}

static void
end(struct drawable *dw)
{
	duk_context *ctx;

	if (callable(dw->data, "end", &ctx))
		duk_call(ctx, 0);
}

static void
finish(struct drawable *dw)
{
	struct self *sf = dw->data;

	if (!--sf->refc)
		free(sf);
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
	struct self *self;
	const int x = duk_require_int(ctx, 0);
	const int y = duk_require_int(ctx, 1);

	self = alloc_new0(sizeof (*self));
	self->refc = 1;
	self->ctx = ctx;
	self->dw.x = x;
	self->dw.y = y;
	self->dw.data = self;
	self->dw.update = update;
	self->dw.finish = finish;
	self->dw.draw = draw;
	self->dw.end = end;

	duk_push_this(ctx);
	self->ptr = duk_get_heapptr(ctx, -1);
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
	struct self *sf;

	duk_get_prop_string(ctx, 0, SIGNATURE);

	if ((sf = duk_to_pointer(ctx, -1))) {
		sf->ptr = NULL;
		drawable_finish(&sf->dw);
	}

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

struct drawable *
js_drawable_require(duk_context *ctx, duk_idx_t idx)
{
	struct self *sf = NULL;

	if (duk_is_object(ctx, idx)) {
		duk_get_prop_string(ctx, idx, SIGNATURE);
		sf = duk_to_pointer(ctx, -1);
		duk_pop(ctx);
	}

	if (!sf)
		return (void)duk_error(ctx, DUK_ERR_TYPE_ERROR, "not a Drawable object"), NULL;

	sf->refc++;

	return &sf->dw;
}
