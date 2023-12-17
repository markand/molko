/*
 * js-animation.c -- basic animations (Javascript bindings)
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
#include <mlk/core/err.h>
#include <mlk/core/animation.h>

#include "js-animation.h"
#include "js-sprite.h"
#include "js.h"

#define SYMBOL          DUK_HIDDEN_SYMBOL("mlk::animation")
#define SPRITE_REF      DUK_HIDDEN_SYMBOL("mlk::animation::sprite")

static struct mlk_animation *
mlk_js_animation_this(duk_context *ctx)
{
	struct mlk_animation *an;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SYMBOL);
	an = duk_to_pointer(ctx, -1);
	duk_pop_n(ctx, 2);

	if (!an)
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "Not an Animation object");

	return an;
}

static duk_ret_t
mlk_js_animation_completed(duk_context *ctx)
{
	duk_push_boolean(ctx, mlk_animation_completed(mlk_js_animation_this(ctx)));

	return 1;
}

static duk_ret_t
mlk_js_animation_new(duk_context *ctx)
{
	struct mlk_animation *anim;
	struct mlk_sprite *sprite;
	unsigned int delay;

	if (!duk_is_constructor_call(ctx))
		return duk_error(ctx, DUK_ERR_TYPE_ERROR, "Animation must be new-constructed");

	sprite = mlk_js_sprite_require(ctx, 0);
	delay = duk_require_uint(ctx, 1);

	anim = mlk_alloc_new0(1, sizeof (*anim));
	anim->sprite = sprite;
	anim->delay = delay;
	mlk_animation_start(anim);

	duk_push_this(ctx);
	duk_push_pointer(ctx, anim);
	duk_put_prop_string(ctx, -2, SYMBOL);
	duk_push_string(ctx, "completed");
	duk_push_c_function(ctx, mlk_js_animation_completed, 0);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_GETTER);

	/* We store the texture into the sprite to avoid being collected. */
	duk_dup(ctx, 0);
	duk_put_prop_string(ctx, -2, SPRITE_REF);
	duk_pop(ctx);

	return 0;
}

static duk_ret_t
mlk_js_animation_finish(duk_context *ctx)
{
	duk_get_prop_string(ctx, 0, SYMBOL);
	mlk_alloc_free(duk_to_pointer(ctx, -1));
	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SYMBOL);
	duk_del_prop_string(ctx, 0, SPRITE_REF);

	return 0;
}

static duk_ret_t
mlk_js_animation_update(duk_context *ctx)
{
	struct mlk_animation *anim = mlk_js_animation_this(ctx);
	unsigned int ticks = duk_require_uint(ctx, 0);

	duk_push_boolean(ctx, mlk_animation_update(anim, ticks));

	return 1;
}

static duk_ret_t
mlk_js_animation_draw(duk_context *ctx)
{
	struct mlk_animation *anim = mlk_js_animation_this(ctx);
	int x, y;

	if (duk_get_top(ctx) == 2) {
		x = duk_require_int(ctx, 0);
		y = duk_require_int(ctx, 1);
	} else if (duk_get_top(ctx) == 1) {
		duk_require_object(ctx, 0);
		duk_get_prop_string(ctx, 0, "x");
		x = duk_get_int(ctx, -1);
		duk_get_prop_string(ctx, 0, "y");
		y = duk_get_int(ctx, -1);
		duk_pop_n(ctx, 2);
	} else
		return duk_error(ctx, DUK_ERR_ERROR, "Object or 2 numbers expected");

	if (!mlk_animation_draw(anim, x, y))
		return duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());

	return 0;
}

static duk_function_list_entry methods[] = {
	{ "update",     mlk_js_animation_update,        1               },
	{ "draw",       mlk_js_animation_draw,          DUK_VARARGS     },
	{ NULL,         NULL,                           0               }
};

void
mlk_js_animation_load(duk_context *ctx)
{
	assert(ctx);

	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1, "Mlk");
	duk_push_c_function(ctx, mlk_js_animation_new, 3);
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, methods);
	duk_push_c_function(ctx, mlk_js_animation_finish, 1);
	duk_set_finalizer(ctx, -2);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_prop_string(ctx, -2, "Animation");
	duk_pop_n(ctx, 2);
}
