/*
 * js-animation.c -- core animation binding
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
#include <core/animation.h>

#include "js-animation.h"
#include "js-sprite.h"

#define SIGNATURE DUK_HIDDEN_SYMBOL("Mlk.Animation")
#define SPRITEREF DUK_HIDDEN_SYMBOL("Mlk.Animation.sprite")

static inline struct animation *
self(duk_context *ctx)
{
	struct animation *anim;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SIGNATURE);
	anim = duk_to_pointer(ctx, -1);
	duk_pop_2(ctx);

	if (!anim)
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "not a Animation object");

	return anim;
}

static duk_ret_t
Animation_getComplete(duk_context *ctx)
{
	duk_push_boolean(ctx, animation_completed(self(ctx)));

	return 1;
}

static duk_ret_t
Animation_start(duk_context *ctx)
{
	animation_start(self(ctx));

	return 0;
}

static duk_ret_t
Animation_update(duk_context *ctx)
{
	struct animation *anim = self(ctx);
	const unsigned int dt = duk_require_uint(ctx, 0);

	duk_push_boolean(ctx, animation_update(anim, dt));

	return 1;
}

static duk_ret_t
Animation_draw(duk_context *ctx)
{
	const struct animation *anim = self(ctx);
	const int x = duk_require_int(ctx, 0);
	const int y = duk_require_int(ctx, 1);

	if (!animation_completed(anim))
		animation_draw(anim, x, y);

	return 0;
}

static duk_ret_t
Animation_constructor(duk_context *ctx)
{
	const struct sprite *sprite = js_sprite_require(ctx, 0);
	const unsigned int delay = duk_require_uint(ctx, 1);
	struct animation *anim;

	anim = alloc_new0(sizeof (*anim));
	animation_init(anim, sprite, delay);

	duk_push_this(ctx);
	duk_push_pointer(ctx, anim);
	duk_put_prop_string(ctx, -2, SIGNATURE);
	duk_push_string(ctx, "complete");
	duk_push_c_function(ctx, Animation_getComplete, 0);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_GETTER);
	duk_pop(ctx);

	return 0;
}

static duk_ret_t
Animation_destructor(duk_context *ctx)
{
	duk_get_prop_string(ctx, 0, SIGNATURE);
	free(duk_to_pointer(ctx, -1));
	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SIGNATURE);

	/* Remove sprite reference. */
	duk_del_prop_string(ctx, 0, SPRITEREF);

	return 0;
}

static const duk_function_list_entry methods[] = {
	{ "start",              Animation_start,        0               },
	{ "update",             Animation_update,       1               },
	{ "draw",               Animation_draw,         2               },
	{ NULL,                 NULL,                   0               }
};

void
js_animation_bind(duk_context *ctx)
{
	assert(ctx);

	duk_push_c_function(ctx, Animation_constructor, 2);
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, methods);
	duk_push_c_function(ctx, Animation_destructor, 1);
	duk_set_finalizer(ctx, -2);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_global_string(ctx, "Animation");
}
