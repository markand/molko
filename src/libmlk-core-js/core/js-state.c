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

struct self {
	duk_context *ctx;
	void *ptr;
	struct state state;
	unsigned int refc;
};

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

static void
start(struct state *state)
{
	duk_context *ctx;

	if (callable(state->data, "start", &ctx)) {
		duk_call(ctx, 0);
		duk_pop(ctx);
	}
}

static void
handle(struct state *state, const union event *ev)
{
	duk_context *ctx;

	if (callable(state->data, "handle", &ctx)) {
		js_event_push(ctx, ev);
		duk_call(ctx, 1);
		duk_pop(ctx);
	}
}

static void
update(struct state *state, unsigned int ticks)
{
	duk_context *ctx;

	if (callable(state->data, "update", &ctx)) {
		duk_push_uint(ctx, ticks);
		duk_call(ctx, 1);
		duk_pop(ctx);
	}
}

static void
draw(struct state *state)
{
	duk_context *ctx;

	if (callable(state->data, "draw", &ctx)) {
		duk_call(ctx, 0);
		duk_pop(ctx);
	}
}

static void
suspend(struct state *state)
{
	duk_context *ctx;

	if (callable(state->data, "suspend", &ctx)) {
		duk_call(ctx, 0);
		duk_pop(ctx);
	}
}

static void
resume(struct state *state)
{
	duk_context *ctx;

	if (callable(state->data, "resume", &ctx)) {
		duk_call(ctx, 0);
		duk_pop(ctx);
	}
}

static void
end(struct state *state)
{
	duk_context *ctx;

	if (callable(state->data, "end", &ctx)) {
		duk_call(ctx, 0);
		duk_pop(ctx);
	}
}

static void
finish(struct state *state)
{
	struct self *self = state->data;

	if (!--self->refc)
		free(self);
}

static duk_ret_t
State_constructor(duk_context *ctx)
{
	struct self *self;

	self = alloc_new0(sizeof (*self));
	self->ctx = ctx;
	self->refc = 1;
	self->state.data = self;
	self->state.start = start;
	self->state.handle = handle;
	self->state.update = update;
	self->state.draw = draw;
	self->state.suspend = suspend;
	self->state.resume = resume;
	self->state.end = end;
	self->state.finish = finish;

	duk_push_this(ctx);
	self->ptr = duk_get_heapptr(ctx, -1);
	duk_push_pointer(ctx, self);
	duk_put_prop_string(ctx, -2, SIGNATURE);
	duk_pop(ctx);

	return 0;
}

static duk_ret_t
State_destructor(duk_context *ctx)
{
	struct self *self;

	duk_get_prop_string(ctx, 0, SIGNATURE);

	if ((self = duk_to_pointer(ctx, -1))) {
		self->ptr = NULL;
		state_finish(&self->state);
	}

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

struct state *
js_state_require(duk_context *ctx, duk_idx_t idx)
{
	struct self *self = NULL;

	if (duk_is_object(ctx, idx)) {
		duk_get_prop_string(ctx, idx, SIGNATURE);
		self = duk_to_pointer(ctx, -1);
		duk_pop(ctx);
	}

	if (!self)
		return (void)duk_error(ctx, DUK_ERR_TYPE_ERROR, "not a State object"), NULL;

	self->refc++;

	return &self->state;
}
