/*
 * js-sprite.c -- core sprite binding
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
#include <core/sprite.h>

#include "js-sprite.h"
#include "js-texture.h"

#define SIGNATURE DUK_HIDDEN_SYMBOL("Mlk.Sprite")
#define TEXREF DUK_HIDDEN_SYMBOL("Mlk.Sprite.texture")

static inline struct sprite *
self(duk_context *ctx)
{
	struct sprite *sprite;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SIGNATURE);
	sprite = duk_to_pointer(ctx, -1);
	duk_pop_2(ctx);

	if (!sprite)
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "not a Sprite object");

	return sprite;
}

static duk_ret_t
Sprite_constructor(duk_context *ctx)
{
	struct sprite *sprite;
	struct texture *tex = js_texture_require(ctx, 0);
	const unsigned int cellw = duk_require_uint(ctx, 1);
	const unsigned int cellh = duk_require_uint(ctx, 2);

	sprite = alloc_new0(sizeof (*sprite));
	sprite_init(sprite, tex, cellw, cellh);

	duk_push_this(ctx);
	duk_push_pointer(ctx, sprite);
	duk_put_prop_string(ctx, -2, SIGNATURE);

	/* Store a copy of texture to avoid destruction. */
	duk_dup(ctx, 0);
	duk_put_prop_string(ctx, -2, TEXREF);

	duk_pop(ctx);

	return 0;
}

static duk_ret_t
Sprite_destructor(duk_context *ctx)
{
	duk_get_prop_string(ctx, 0, SIGNATURE);
	free(duk_to_pointer(ctx, -1));
	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SIGNATURE);

	/* Remove texture reference. */
	duk_del_prop_string(ctx, 0, TEXREF);

	return 0;
}

static duk_ret_t
Sprite_draw(duk_context *ctx)
{
	const struct sprite *sprite = self(ctx);
	const unsigned int r = duk_require_uint(ctx, 0);
	const unsigned int c = duk_require_uint(ctx, 1);
	const int x = duk_require_int(ctx, 2);
	const int y = duk_require_int(ctx, 3);

	if (r >= sprite->nrows)
		duk_error(ctx, DUK_ERR_RANGE_ERROR, "row overflow: %u >= %u", r, sprite->nrows);
	if (c >= sprite->ncols)
		duk_error(ctx, DUK_ERR_RANGE_ERROR, "column overflow: %u >= %u", c, sprite->ncols);

	sprite_draw(sprite, r, c, x, y);

	return 0;
}

static duk_ret_t
Sprite_scale(duk_context *ctx)
{
	const struct sprite *sprite = self(ctx);
	const unsigned int r = duk_require_uint(ctx, 0);
	const unsigned int c = duk_require_uint(ctx, 1);
	const int x = duk_require_int(ctx, 2);
	const int y = duk_require_int(ctx, 3);
	const unsigned int w = duk_require_uint(ctx, 4);
	const unsigned int h = duk_require_uint(ctx, 5);

	if (r >= sprite->nrows)
		duk_error(ctx, DUK_ERR_RANGE_ERROR, "row overflow: %u >= %u", r, sprite->nrows);
	if (c >= sprite->ncols)
		duk_error(ctx, DUK_ERR_RANGE_ERROR, "column overflow: %u >= %u", c, sprite->ncols);

	sprite_scale(sprite, r, c, x, y, w, h);

	return 0;
}

static const duk_function_list_entry methods[] = {
	{ "draw",               Sprite_draw,            4               },
	{ "scale",              Sprite_scale,           6               },
	{ NULL,                 NULL,                   0               }
};

void
js_sprite_bind(duk_context *ctx)
{
	assert(ctx);

	duk_push_c_function(ctx, Sprite_constructor, 3);
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, methods);
	duk_push_c_function(ctx, Sprite_destructor, 1);
	duk_set_finalizer(ctx, -2);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_global_string(ctx, "Sprite");
}
