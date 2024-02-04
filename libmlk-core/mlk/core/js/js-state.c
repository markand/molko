/*
 * js-state.c -- abstract game loop state (Javascript bindings)
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
#include <mlk/core/panic.h>
#include <mlk/core/state.h>
#include <mlk/core/util.h>

#include "js-event.h"
#include "js-state.h"

#define SYMBOL DUK_HIDDEN_SYMBOL("mlk::state")

/*
 * Cache virtual functions inside the object structure for performance reasons
 * to avoid permanent lookup.
 */
#define SET(obj, name)                                                  \
do {                                                                    \
        duk_push_this(obj->ctx);                                        \
        duk_get_prototype(obj->ctx, -1);                                \
        duk_get_prop_string(obj->ctx, -1, #name);                       \
                                                                        \
        if (duk_is_callable(obj->ctx, -1)) {                            \
                obj->ref_##name = duk_get_heapptr(obj->ctx, -1);        \
                obj->state.name = mlk_js_state__##name;                 \
        }                                                               \
                                                                        \
        duk_pop_n(obj->ctx, 3);                                         \
} while (0);

struct object {
	duk_context *ctx;
	void *ref_this;
	void *ref_start;
	void *ref_handle;
	void *ref_update;
	void *ref_draw;
	void *ref_suspend;
	void *ref_resume;
	void *ref_end;
	struct mlk_state state;
};

#if 0
static struct object *
mlk_js_state__this(duk_context *ctx)
{
	struct object *obj;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SYMBOL);
	obj = duk_to_pointer(ctx, -1);
	duk_pop_n(ctx, 2);

	if (!obj)
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "Not a State object");

	return obj;
}
#endif

static void
mlk_js_state__start(struct mlk_state *self)
{
	struct object *obj = MLK_CONTAINER_OF(self, struct object, state);

	duk_push_heapptr(obj->ctx, obj->ref_start);
	duk_push_heapptr(obj->ctx, obj->ref_this);

	if (duk_pcall_method(obj->ctx, 0) != 0)
		mlk_panicf("%s", duk_to_string(obj->ctx, -1));

	duk_pop(obj->ctx);
}

static void
mlk_js_state__handle(struct mlk_state *self, const union mlk_event *event)
{
	struct object *obj = MLK_CONTAINER_OF(self, struct object, state);

	duk_push_heapptr(obj->ctx, obj->ref_handle);
	duk_push_heapptr(obj->ctx, obj->ref_this);
	mlk_js_event_push(obj->ctx, event);

	if (duk_pcall_method(obj->ctx, 1) != 0)
		mlk_panicf("%s", duk_to_string(obj->ctx, -1));

	duk_pop(obj->ctx);
}

static void
mlk_js_state__update(struct mlk_state *self, unsigned int ticks)
{
	struct object *obj = MLK_CONTAINER_OF(self, struct object, state);

	duk_push_heapptr(obj->ctx, obj->ref_update);
	duk_push_heapptr(obj->ctx, obj->ref_this);
	duk_push_uint(obj->ctx, ticks);

	if (duk_pcall_method(obj->ctx, 1) != 0)
		mlk_panicf("%s", duk_to_string(obj->ctx, -1));

	duk_pop(obj->ctx);
}

static void
mlk_js_state__draw(struct mlk_state *self)
{
	struct object *obj = MLK_CONTAINER_OF(self, struct object, state);

	duk_push_heapptr(obj->ctx, obj->ref_draw);
	duk_push_heapptr(obj->ctx, obj->ref_this);

	if (duk_pcall_method(obj->ctx, 0) != 0)
		mlk_panicf("%s", duk_to_string(obj->ctx, -1));

	duk_pop(obj->ctx);
}

static void
mlk_js_state__suspend(struct mlk_state *self)
{
	struct object *obj = MLK_CONTAINER_OF(self, struct object, state);

	duk_push_heapptr(obj->ctx, obj->ref_suspend);
	duk_push_heapptr(obj->ctx, obj->ref_this);

	if (duk_pcall_method(obj->ctx, 0) != 0)
		mlk_panicf("%s", duk_to_string(obj->ctx, -1));

	duk_pop(obj->ctx);
}

static void
mlk_js_state__resume(struct mlk_state *self)
{
	struct object *obj = MLK_CONTAINER_OF(self, struct object, state);

	if (!obj->ref_resume)
		return;

	duk_push_heapptr(obj->ctx, obj->ref_resume);
	duk_push_heapptr(obj->ctx, obj->ref_this);

	if (duk_pcall_method(obj->ctx, 0) != 0)
		mlk_panicf("%s", duk_to_string(obj->ctx, -1));

	duk_pop(obj->ctx);
}

static void
mlk_js_state__end(struct mlk_state *self)
{
	struct object *obj = MLK_CONTAINER_OF(self, struct object, state);

	duk_push_heapptr(obj->ctx, obj->ref_end);
	duk_push_heapptr(obj->ctx, obj->ref_this);

	if (duk_pcall_method(obj->ctx, 0) != 0)
		mlk_panicf("%s", duk_to_string(obj->ctx, -1));

	duk_pop(obj->ctx);
}

static duk_ret_t
mlk_js_state__finish(duk_context *ctx)
{
	struct object *obj;

	printf("FINALIZING STATE\n");
	duk_get_prop_string(ctx, 0, SYMBOL);
	obj = duk_to_pointer(ctx, -1);
	mlk_alloc_free(obj);
	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SYMBOL);

	return 0;
}

static duk_ret_t
mlk_js_state__new(duk_context *ctx)
{
	struct object *obj;

	obj = mlk_alloc_new(1, sizeof (*obj));
	obj->ctx = ctx;
	obj->state.name = duk_require_string(ctx, 0);

	/* Cache virtual functions for performance reasons. */
	SET(obj, start);
	SET(obj, handle);
	SET(obj, update);
	SET(obj, draw);
	SET(obj, suspend);
	SET(obj, resume);
	SET(obj, end);

	duk_push_this(ctx);
	obj->ref_this = duk_get_heapptr(ctx, -1);
	printf("%p\n", obj->ref_this);
	duk_push_pointer(ctx, obj);
	duk_put_prop_string(ctx, -2, SYMBOL);
	duk_push_c_function(ctx, mlk_js_state__finish, 1);
	duk_set_finalizer(ctx, -2);
	duk_pop(ctx);

	return 0;
}

struct mlk_state *
mlk_js_state_require(duk_context *ctx, duk_idx_t index)
{
	struct object *obj;

	duk_get_prop_string(ctx, index, SYMBOL);
	obj = duk_to_pointer(ctx, -1);
	duk_pop(ctx);

	if (!obj)
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "State expected on argument #%u", index);

	return &obj->state;
}

void
mlk_js_state_load(duk_context *ctx)
{
	assert(ctx);

	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1, "Mlk");
	duk_push_c_function(ctx, mlk_js_state__new, 1);
	duk_push_object(ctx);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_prop_string(ctx, -2, "State");
	duk_pop(ctx);
}
