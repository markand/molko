/*
 * js-sound.c -- sound support (Javascript bindings)
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
#include <mlk/core/sound.h>

#include "js-sound.h"

#define SYMBOL DUK_HIDDEN_SYMBOL("mlk::sound")

static struct mlk_sound *
mlk_js_sound_this(duk_context *ctx)
{
	struct mlk_sound *snd;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SYMBOL);
	snd = duk_to_pointer(ctx, -1);
	duk_pop_n(ctx, 2);

	if (!snd)
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "Not a Sound object");

	return snd;
}

static duk_ret_t
mlk_js_sound_new(duk_context *ctx)
{
	struct mlk_sound snd;
	const char *path;

	if (!duk_is_constructor_call(ctx))
		return duk_error(ctx, DUK_ERR_TYPE_ERROR, "Sound must be new-constructed");

	path = duk_require_string(ctx, 0);

	if (mlk_sound_open(&snd, path) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());

	duk_push_this(ctx);
	duk_push_pointer(ctx, mlk_alloc_dup(&snd, 1, sizeof (snd)));
	duk_put_prop_string(ctx, -2, SYMBOL);
	duk_pop(ctx);

	return 0;
}

static duk_ret_t
mlk_js_sound_play(duk_context *ctx)
{
	struct mlk_sound *snd = mlk_js_sound_this(ctx);

	if (mlk_sound_play(snd) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());

	return 0;
}

static duk_ret_t
mlk_js_sound_pause(duk_context *ctx)
{
	mlk_sound_pause(mlk_js_sound_this(ctx));

	return 0;
}

static duk_ret_t
mlk_js_sound_resume(duk_context *ctx)
{
	mlk_sound_resume(mlk_js_sound_this(ctx));

	return 0;
}

static duk_ret_t
mlk_js_sound_stop(duk_context *ctx)
{
	mlk_sound_stop(mlk_js_sound_this(ctx));

	return 0;
}

static duk_ret_t
mlk_js_sound_finish(duk_context *ctx)
{
	struct mlk_sound *snd;

	duk_get_prop_string(ctx, 0, SYMBOL);

	if ((snd = duk_to_pointer(ctx, -1))) {
		mlk_sound_finish(snd);
		mlk_alloc_free(snd);
	}

	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SYMBOL);

	return 0;
}

static const duk_function_list_entry methods[] = {
	{ "play",               mlk_js_sound_play,      DUK_VARARGS     },
	{ "pause",              mlk_js_sound_pause,     0               },
	{ "resume",             mlk_js_sound_resume,    0               },
	{ "stop",               mlk_js_sound_stop,      DUK_VARARGS     },
	{ NULL,                 NULL,                   0               }
};

void
mlk_js_sound_load(duk_context *ctx)
{
	assert(ctx);

	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1, "Mlk");
	duk_push_c_function(ctx, mlk_js_sound_new, 3);
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, methods);
	duk_push_c_function(ctx, mlk_js_sound_finish, 1);
	duk_set_finalizer(ctx, -2);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_prop_string(ctx, -2, "Sound");
	duk_pop_n(ctx, 2);
}
