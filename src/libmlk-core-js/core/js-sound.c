/*
 * js-sound.c -- core sound binding
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
#include <core/error.h>
#include <core/sound.h>
#include <core/vfs.h>

#include "js-core.h"
#include "js-sound.h"

#define SIGNATURE DUK_HIDDEN_SYMBOL("Mlk.Sound")

static struct sound *
self(duk_context *ctx)
{
	struct sound *snd;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SIGNATURE);
	snd = duk_to_pointer(ctx, -1);
	duk_pop_2(ctx);

	if (!snd)
		return (void)duk_error(ctx, DUK_ERR_TYPE_ERROR, "not a Sound object"), NULL;

	return snd;
}

static duk_ret_t
Sound_constructor(duk_context *ctx)
{
	const char *entry = duk_require_string(ctx, 0);
	struct vfs_file file;
	struct sound *snd;

	if (vfs_open(js_core_global_vfs(ctx), &file, entry, "r") < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	snd = alloc_new0(sizeof (*snd));

	if (sound_openvfs(snd, &file) < 0) {
		free(snd);
		vfs_file_finish(&file);

		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());
	}

	vfs_file_finish(&file);

	duk_push_this(ctx);
	duk_push_pointer(ctx, snd);
	duk_put_prop_string(ctx, -2, SIGNATURE);
	duk_pop(ctx);

	return 0;


}

static duk_ret_t
Sound_destructor(duk_context *ctx)
{
	struct sound *snd;

	duk_get_prop_string(ctx, 0, SIGNATURE);

	if ((snd = duk_to_pointer(ctx, -1)))
		sound_finish(snd);

	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SIGNATURE);

	return 0;
}

static duk_ret_t
Sound_proto_play(duk_context *ctx)
{
	const int channel = duk_get_int_default(ctx, 0, 0);
	const unsigned int fadein = duk_get_uint_default(ctx, 1, 0);

	if (sound_play(self(ctx), channel, fadein) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	return 0;
}

static duk_ret_t
Sound_proto_pause(duk_context *ctx)
{
	sound_pause(self(ctx));

	return 0;
}

static duk_ret_t
Sound_proto_resume(duk_context *ctx)
{
	sound_resume(self(ctx));

	return 0;
}

static duk_ret_t
Sound_proto_stop(duk_context *ctx)
{
	const unsigned int fadeout = duk_get_uint_default(ctx, 0, 0);

	sound_stop(self(ctx), fadeout);

	return 0;
}

static duk_ret_t
Sound_pause(duk_context *ctx)
{
	(void)ctx;

	sound_pause(NULL);

	return 0;
}

static duk_ret_t
Sound_resume(duk_context *ctx)
{
	(void)ctx;

	sound_resume(NULL);

	return 0;
}

static duk_ret_t
Sound_stop(duk_context *ctx)
{
	sound_stop(NULL, duk_get_uint_default(ctx, 0, 0));

	return 0;
}

static const duk_function_list_entry methods[] = {
	{ "play",               Sound_proto_play,       DUK_VARARGS     },
	{ "pause",              Sound_proto_pause,      0               },
	{ "resume",             Sound_proto_resume,     0               },
	{ "stop",               Sound_proto_stop,       DUK_VARARGS     },
	{ NULL,                 NULL,                   0               }
};

static const duk_function_list_entry functions[] = {
	{ "pause",              Sound_pause,            0               },
	{ "resume",             Sound_resume,           0               },
	{ "stop",               Sound_stop,             DUK_VARARGS     },
	{ NULL,                 NULL,                   0               }
};

void
js_sound_bind(duk_context *ctx)
{
	assert(ctx);

	duk_push_c_function(ctx, Sound_constructor, 1);
	duk_put_function_list(ctx, -1, functions);
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, methods);
	duk_push_c_function(ctx, Sound_destructor, 1);
	duk_set_finalizer(ctx, -2);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_global_string(ctx, "Sound");
}
