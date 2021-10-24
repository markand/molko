/*
 * js-drawable-stack.h -- core drawable_stack binding
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

#include "js-drawable-stack.h"
#include "js-drawable.h"

#define SIGNATURE DUK_HIDDEN_SYMBOL("Mlk.DrawableStack")

static inline struct drawable_stack *
self(duk_context *ctx)
{
	struct drawable_stack *sf = NULL;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SIGNATURE);
	sf = duk_to_pointer(ctx, -1);
	duk_pop_2(ctx);

	if (!sf)
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "not a DrawableStack object");

	return sf;
}

static duk_ret_t
DrawableStack_constructor(duk_context *ctx)
{
	duk_push_this(ctx);
	duk_push_pointer(ctx, alloc_new0(sizeof (struct drawable_stack)));
	duk_put_prop_string(ctx, -2, SIGNATURE);
	duk_pop(ctx);

	return 0;
}

static duk_ret_t
DrawableStack_destructor(duk_context *ctx)
{
	struct drawable_stack *st;

	duk_get_prop_string(ctx, 0, SIGNATURE);
	
	if ((st = duk_to_pointer(ctx, -1))) {
		drawable_stack_finish(st);
		free(st);
	}
		
	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SIGNATURE);

	return 0;
}

static duk_ret_t
DrawableStack_add(duk_context *ctx)
{
	struct drawable_stack *st = self(ctx);
	struct drawable *dw = js_drawable_require(ctx, 0);

	if (drawable_stack_add(st, dw) < 0)
		drawable_finish(dw);

	return 0;
}

static duk_ret_t
DrawableStack_update(duk_context *ctx)
{
	struct drawable_stack *st = self(ctx);
	const unsigned int ticks = duk_require_uint(ctx, 0);

	duk_push_uint(ctx, drawable_stack_update(st, ticks));

	return 1;
}

static duk_ret_t
DrawableStack_draw(duk_context *ctx)
{
	drawable_stack_draw(self(ctx));

	return 0;
}

static duk_function_list_entry methods[] = {
	{ "add",        DrawableStack_add,      1       },
	{ "update",     DrawableStack_update,   1       },
	{ "draw",       DrawableStack_draw,     0       },
	{ NULL,         NULL,                   0       }
};

void
js_drawable_stack_bind(duk_context *ctx)
{
	assert(ctx);

	duk_push_c_function(ctx, DrawableStack_constructor, 0);
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, methods);
	duk_push_c_function(ctx, DrawableStack_destructor, 1);
	duk_set_finalizer(ctx, -2);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_global_string(ctx, "DrawableStack");
}
