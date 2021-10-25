/*
 * js-state.c -- core state binding
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
#include <stdlib.h>

#include <core/alloc.h>
#include <core/state.h>

#include "js-event.h"
#include "js-state.h"

#define SIGNATURE DUK_HIDDEN_SYMBOL("Mlk.State")

static inline duk_context *
callable(struct js_state *data, const char *prop)
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

static void
start(struct state *state)
{
	duk_context *ctx;

	if ((ctx = callable(state->data, "start"))) {
		duk_call_method(ctx, 0);
		duk_pop(ctx);
	}
}

static void
handle(struct state *state, const union event *ev)
{
	duk_context *ctx;

	if ((ctx = callable(state->data, "handle"))) {
		js_event_push(ctx, ev);
		duk_call_method(ctx, 1);
		duk_pop(ctx);
	}
}

static void
update(struct state *state, unsigned int ticks)
{
	duk_context *ctx;

	if ((ctx = callable(state->data, "update"))) {
		duk_push_uint(ctx, ticks);
		duk_call_method(ctx, 1);
		duk_pop(ctx);
	}
}

static void
draw(struct state *state)
{
	duk_context *ctx;

	if ((ctx = callable(state->data, "draw"))) {
		duk_call_method(ctx, 0);
		duk_pop(ctx);
	}
}

static void
suspend(struct state *state)
{
	duk_context *ctx;

	if ((ctx = callable(state->data, "suspend"))) {
		duk_call_method(ctx, 0);
		duk_pop(ctx);
	}
}

static void
resume(struct state *state)
{
	duk_context *ctx;

	if ((ctx = callable(state->data, "resume"))) {
		duk_call_method(ctx, 0);
		duk_pop(ctx);
	}
}

static void
end(struct state *state)
{
	duk_context *ctx;

	if ((ctx = callable(state->data, "end"))) {
		duk_call_method(ctx, 0);
		duk_pop(ctx);
	}
}

static void
finish(struct state *state)
{
	struct js_state *data = state->data;

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
State_constructor(duk_context *ctx)
{
	struct js_state *data;

	data = alloc_new0(sizeof (*data));
	data->ctx = ctx;
	data->refc = 1;
	data->st.data = data;
	data->st.start = start;
	data->st.handle = handle;
	data->st.update = update;
	data->st.draw = draw;
	data->st.suspend = suspend;
	data->st.resume = resume;
	data->st.end = end;
	data->st.finish = finish;

	duk_push_this(ctx);
	data->ptr = duk_get_heapptr(ctx, -1);
	duk_push_pointer(ctx, data);
	duk_put_prop_string(ctx, -2, SIGNATURE);
	duk_pop(ctx);

	return 0;
}

static duk_ret_t
State_destructor(duk_context *ctx)
{
	struct js_state *data;

	duk_get_prop_string(ctx, 0, SIGNATURE);

	if ((data = duk_to_pointer(ctx, -1)))
		state_finish(&data->st);

	duk_pop(ctx);

	return 0;
}

void
js_state_bind(duk_context *ctx)
{
	assert(ctx);

	duk_push_c_function(ctx, State_constructor, 1);
	duk_push_object(ctx);
	duk_push_c_function(ctx, State_destructor, 1);
	duk_set_finalizer(ctx, -2);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_global_string(ctx, "State");
}

struct js_state *
js_state_require(duk_context *ctx, duk_idx_t idx)
{
	struct js_state *data = NULL;

	if (duk_is_object(ctx, idx)) {
		duk_get_prop_string(ctx, idx, SIGNATURE);
		data = duk_to_pointer(ctx, -1);
		duk_pop(ctx);
	}

	if (!data)
		return (void)duk_error(ctx, DUK_ERR_TYPE_ERROR, "not a State object"), NULL;

	data->refc++;

	return data;
}
