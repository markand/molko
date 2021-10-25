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

#include <core/action.h>
#include <core/action-stack.h>
#include <core/alloc.h>

#include "js-event.h"
#include "js-action-stack.h"
#include "js-action.h"

#define SIGNATURE DUK_HIDDEN_SYMBOL("Mlk.ActionStack")

static inline struct action_stack *
self(duk_context *ctx)
{
	struct action_stack *sf = NULL;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SIGNATURE);
	sf = duk_to_pointer(ctx, -1);
	duk_pop_2(ctx);

	if (!sf)
		return (void)duk_error(ctx, DUK_ERR_TYPE_ERROR, "not a ActionStack object"), NULL;

	return sf;
}

static duk_ret_t
ActionStack_constructor(duk_context *ctx)
{
	duk_push_this(ctx);
	duk_push_pointer(ctx, alloc_new0(sizeof (struct action_stack)));
	duk_put_prop_string(ctx, -2, SIGNATURE);
	duk_pop(ctx);

	return 0;
}

static duk_ret_t
ActionStack_destructor(duk_context *ctx)
{
	struct action_stack *st;

	duk_get_prop_string(ctx, 0, SIGNATURE);

	if ((st = duk_to_pointer(ctx, -1))) {
		action_stack_finish(st);
		free(st);
	}

	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SIGNATURE);

	return 0;
}

static duk_ret_t
ActionStack_add(duk_context *ctx)
{
	struct action_stack *st = self(ctx);
	struct js_action *act = js_action_require(ctx, 0);

	if (action_stack_add(st, &act->act) < 0)
		action_finish(&act->act);
	else {
		duk_push_this(ctx);
		act->parent = duk_get_heapptr(ctx, -1);
		duk_push_sprintf(ctx, "%p", act);
		duk_dup(ctx, 0);
		duk_put_prop(ctx, -3);
		duk_pop(ctx);
	}

	return 0;
}

static duk_ret_t
ActionStack_handle(duk_context *ctx)
{
	action_stack_handle(self(ctx), js_event_require(ctx, 0));

	return 0;
}

static duk_ret_t
ActionStack_update(duk_context *ctx)
{
	struct action_stack *st = self(ctx);
	const unsigned int ticks = duk_require_uint(ctx, 0);

	duk_push_uint(ctx, action_stack_update(st, ticks));

	return 1;
}

static duk_ret_t
ActionStack_draw(duk_context *ctx)
{
	action_stack_draw(self(ctx));

	return 0;
}

static duk_function_list_entry methods[] = {
	{ "add",        ActionStack_add,        1       },
	{ "handle",     ActionStack_handle,     1       },
	{ "update",     ActionStack_update,     1       },
	{ "draw",       ActionStack_draw,       0       },
	{ NULL,         NULL,                   0       }
};

void
js_action_stack_bind(duk_context *ctx)
{
	assert(ctx);

	duk_push_c_function(ctx, ActionStack_constructor, 0);
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, methods);
	duk_push_c_function(ctx, ActionStack_destructor, 1);
	duk_set_finalizer(ctx, -2);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_global_string(ctx, "ActionStack");
}
