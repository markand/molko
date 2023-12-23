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
#include <mlk/core/vfs.h>

#include "js-sound.h"
#include "js.h"

#define SYMBOL DUK_HIDDEN_SYMBOL("mlk::sound")
#define PROTO  DUK_HIDDEN_SYMBOL("mlk::sound::proto")

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

static duk_ret_t
mlk_js_sound_fromFile(duk_context *ctx)
{
	const char *path = duk_require_string(ctx, 0);
	struct mlk_sound snd;

	if (mlk_sound_open(&snd, path) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());

	mlk_js_sound_push(ctx, &snd);

	return 1;
}

static duk_ret_t
mlk_js_sound_fromVFS(duk_context *ctx)
{
	const char *path = duk_require_string(ctx, 0);
	struct mlk_vfs *vfs = mlk_js_require(ctx)->vfs;
	struct mlk_vfs_file *file;
	struct mlk_sound snd;
	int rv;

	if (!(file = mlk_vfs_open(vfs, path, "r")))
		return duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());

	rv = mlk_sound_openvfs(&snd, file);
	mlk_vfs_file_finish(file);

	if (rv < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());

	mlk_js_sound_push(ctx, &snd);

	return 1;
}

static const duk_function_list_entry methods[] = {
	{ "play",       mlk_js_sound_play,      DUK_VARARGS     },
	{ "pause",      mlk_js_sound_pause,     0               },
	{ "resume",     mlk_js_sound_resume,    0               },
	{ "stop",       mlk_js_sound_stop,      DUK_VARARGS     },
	{ NULL,         NULL,                   0               }
};

static const duk_function_list_entry functions[] = {
	{ "fromFile",   mlk_js_sound_fromFile,  1               },
	{ "fromVFS",    mlk_js_sound_fromVFS,   1               },
	{ NULL,         NULL,                   0               }
};

void
mlk_js_sound_load(duk_context *ctx)
{
	assert(ctx);

	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1, "Mlk");
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, functions);
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, methods);
	duk_push_c_function(ctx, mlk_js_sound_finish, 1);
	duk_set_finalizer(ctx, -2);
	duk_push_global_stash(ctx);
	duk_dup(ctx, -2);
	duk_put_prop_string(ctx, -2, PROTO);
	duk_pop(ctx);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_prop_string(ctx, -2, "Sound");
	duk_pop_n(ctx, 2);
}

void
mlk_js_sound_push(duk_context *ctx, const struct mlk_sound *snd)
{
	assert(ctx);
	assert(snd);

	duk_push_object(ctx);
	duk_push_global_stash(ctx);
	duk_get_prop_string(ctx, -1, PROTO);
	duk_remove(ctx, -2);
	duk_push_pointer(ctx, mlk_alloc_dup(snd, 1, sizeof (*snd)));
	duk_put_prop_string(ctx, -3, SYMBOL);
	duk_set_prototype(ctx, -2);
}
