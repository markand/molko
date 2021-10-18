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
#define FUNCTIONS DUK_HIDDEN_SYMBOL("Mlk.State.functions")

#define INVOKE(state, function, block)                          \
do {                                                            \
        struct statedata *data = state->data;                   \
                                                                \
        duk_push_heapptr(data->ctx, data->heapptr);             \
        duk_get_prop_index(data->ctx, -1, function);            \
        duk_remove(data->ctx, -2);                              \
                                                                \
        if (duk_is_callable(data->ctx, -1)) {                   \
                block                                           \
                duk_pop(data->ctx);                             \
        } else {                                                \
                duk_pop(data->ctx);                             \
        }                                                       \
} while (0)

#define SET(ctx, function)                                      \
do {                                                            \
        struct statedata *data = self(ctx);                     \
                                                                \
        duk_push_heapptr(ctx, data->heapptr);                   \
        duk_dup(ctx, 0);                                        \
        duk_put_prop_index(ctx, -2, function);                  \
        duk_pop(ctx);                                           \
                                                                \
        return 0;                                               \
} while (0)

enum statefunc {
	FUNC_START,
	FUNC_HANDLE,
	FUNC_UPDATE,
	FUNC_DRAW,
	FUNC_SUSPEND,
	FUNC_RESUME,
	FUNC_END
};

struct statedata {
	duk_context *ctx;
	void *heapptr;
	struct state state;
};

static void
start(struct state *state)
{
	INVOKE(state, FUNC_START, {
		duk_call(data->ctx, 0);
	});
}

static void
handle(struct state *state, const union event *ev)
{
	INVOKE(state, FUNC_HANDLE, {
		js_event_push(data->ctx, ev);
		duk_call(data->ctx, 1);
	});
}

static void
update(struct state *state, unsigned int ticks)
{
	INVOKE(state, FUNC_UPDATE, {
		duk_push_uint(data->ctx, ticks);
		duk_call(data->ctx, 1);
	});
}

static void
draw(struct state *state)
{
	INVOKE(state, FUNC_DRAW, {
		duk_call(data->ctx, 0);
	});
}

static void
suspend(struct state *state)
{
	INVOKE(state, FUNC_SUSPEND, {
		duk_call(data->ctx, 0);
	});
}

static void
resume(struct state *state)
{
	INVOKE(state, FUNC_RESUME, {
		duk_call(data->ctx, 0);
	});
}

static void
end(struct state *state)
{
	INVOKE(state, FUNC_END, {
		duk_call(data->ctx, 0);
	});
}

static void
finish(struct state *state)
{
	free(state->data);
}

static inline struct statedata *
self(duk_context *ctx)
{
	struct statedata *data = NULL;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SIGNATURE);
	data = duk_to_pointer(ctx, -1);
	duk_pop_2(ctx);

	if (!data)
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "not a State object");

	return data;
}

static duk_ret_t
State_setStart(duk_context *ctx)
{
	SET(ctx, FUNC_START);
}

static duk_ret_t
State_setHandle(duk_context *ctx)
{
	SET(ctx, FUNC_HANDLE);
}

static duk_ret_t
State_setUpdate(duk_context *ctx)
{
	SET(ctx, FUNC_UPDATE);
}

static duk_ret_t
State_setDraw(duk_context *ctx)
{
	SET(ctx, FUNC_DRAW);
}

static duk_ret_t
State_setSuspend(duk_context *ctx)
{
	SET(ctx, FUNC_SUSPEND);
}

static duk_ret_t
State_setResume(duk_context *ctx)
{
	SET(ctx, FUNC_RESUME);
}

static duk_ret_t
State_setEnd(duk_context *ctx)
{
	SET(ctx, FUNC_END);
}

static const struct {
	const char *property;
	duk_ret_t (*setter)(duk_context *ctx);
} properties[] = {
	{ "start",      State_setStart          },
	{ "handle",     State_setHandle         },
	{ "update",     State_setUpdate         },
	{ "draw",       State_setDraw           },
	{ "suspend",    State_setSuspend        },
	{ "resume",     State_setResume         },
	{ "end",        State_setEnd            },
	{ NULL,         NULL                    }
};

static duk_ret_t
State_constructor(duk_context *ctx)
{
	struct statedata *data;

	data = alloc_new0(sizeof (*data));
	data->ctx = ctx;
	data->state.data = data;
	data->state.start = start;
	data->state.handle = handle;
	data->state.update = update;
	data->state.draw = draw;
	data->state.suspend = suspend;
	data->state.resume = resume;
	data->state.end = end;
	data->state.finish = finish;

	duk_push_this(ctx);
	duk_push_pointer(ctx, data);
	duk_put_prop_string(ctx, -2, SIGNATURE);
	duk_push_array(ctx);
	data->heapptr = duk_get_heapptr(ctx, -1);
	duk_put_prop_string(ctx, -2, FUNCTIONS);

	/* Push every properties. */
	for (size_t i = 0; properties[i].property; ++i) {
		duk_push_string(ctx, properties[i].property);
		duk_push_c_function(ctx, properties[i].setter, 1);
		duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_SETTER);
	}

	duk_pop(ctx);

	return 0;
}

void
js_state_bind(duk_context *ctx)
{
	assert(ctx);

	duk_push_c_function(ctx, State_constructor, 1);
	duk_put_global_string(ctx, "State");
}

struct state *
js_state_require(duk_context *ctx, duk_idx_t idx)
{
	struct statedata *data = NULL;

	if (duk_is_object(ctx, idx)) {
		duk_get_prop_string(ctx, idx, SIGNATURE);
		data = duk_to_pointer(ctx, -1);
		duk_pop(ctx);
	}
	
	if (!data)
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "not a State object");

	return &data->state;
}
