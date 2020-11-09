/*
 * js-animation.c -- basic animations (Javascript bindings)
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
#include <core/error.h>
#include <core/animation.h>

#include "js.h"
#include "js-animation.h"
#include "js-sprite.h"

#define SYMBOL          DUK_HIDDEN_SYMBOL("molko::animation")
#define SPRITE_REF      DUK_HIDDEN_SYMBOL("molko::animation::sprite")

static struct animation *
js_animation_this(duk_context *ctx)
{
	struct animation *an;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SYMBOL);
	an = duk_to_pointer(ctx, -1);
	duk_pop_n(ctx, 2);

	if (!an)
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "Not an Animation object");

	return an;
}

static duk_ret_t
js_animation_completed(duk_context *ctx)
{
	duk_push_boolean(ctx, animation_completed(js_animation_this(ctx)));

	return 1;
}

static duk_ret_t
js_animation_new(duk_context *ctx)
{
	struct animation *anim;
	struct sprite *sprite;
	unsigned int delay;

	if (!duk_is_constructor_call(ctx))
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "Animation must be new-constructed");

	sprite = js_sprite_require(ctx, 0);
	delay = duk_require_uint(ctx, 1);

	anim = alloc_zero(1, sizeof (*anim));
	animation_init(anim, sprite, delay);

	duk_push_this(ctx);
	duk_push_pointer(ctx, anim);
	duk_put_prop_string(ctx, -2, SYMBOL);
	duk_push_string(ctx, "completed");
	duk_push_c_function(ctx, js_animation_completed, 0);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_GETTER);

	/* We store the texture into the sprite to avoid being collected. */
	duk_dup(ctx, 0);
	duk_put_prop_string(ctx, -2, SPRITE_REF);
	duk_pop(ctx);

	return 0;
}

static duk_ret_t
js_animation_finish(duk_context *ctx)
{
	duk_get_prop_string(ctx, 0, SYMBOL);
	free(duk_to_pointer(ctx, -1));
	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SYMBOL);
	duk_del_prop_string(ctx, 0, SPRITE_REF);

	return 0;
}

static duk_ret_t
js_animation_update(duk_context *ctx)
{
	struct animation *anim = js_animation_this(ctx);
	unsigned int ticks = duk_require_uint(ctx, 0);

	duk_push_boolean(ctx, animation_update(anim, ticks));

	return 1;
}

static duk_ret_t
js_animation_draw(duk_context *ctx)
{
	struct animation *anim = js_animation_this(ctx);
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

	if (!animation_draw(anim, x, y))
		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	return 0;
}

static duk_function_list_entry methods[] = {
	{ "update",     js_animation_update,    1               },
	{ "draw",       js_animation_draw,      DUK_VARARGS     },
	{ NULL,         NULL,                   0               }
};

void
js_animation_load(duk_context *ctx)
{
	assert(ctx);

	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1, "Molko");
	duk_push_c_function(ctx, js_animation_new, 3);
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, methods);
	duk_push_c_function(ctx, js_animation_finish, 1);
	duk_set_finalizer(ctx, -2);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_prop_string(ctx, -2, "Animation");
	duk_pop_n(ctx, 2);
}
