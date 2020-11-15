/*
 * js-sound.c -- sound support (Javascript bindings)
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

#include <core/alloc.h>
#include <core/error.h>
#include <core/sound.h>

#include "js-sound.h"

#define SYMBOL DUK_HIDDEN_SYMBOL("molko::sound")

static struct sound *
js_sound_this(duk_context *ctx)
{
	struct sound *snd;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SYMBOL);
	snd = duk_to_pointer(ctx, -1);
	duk_pop_n(ctx, 2);

	if (!snd)
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "Not a Sound object");

	return snd;
}

static duk_ret_t
js_sound_new(duk_context *ctx)
{
	struct sound snd;
	const char *path;

	if (!duk_is_constructor_call(ctx))
		return duk_error(ctx, DUK_ERR_TYPE_ERROR, "Sound must be new-constructed");

	path = duk_require_string(ctx, 0);

	if (!sound_open(&snd, path))
		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	duk_push_this(ctx);
	duk_push_pointer(ctx, alloc_dup(&snd, sizeof (snd)));
	duk_put_prop_string(ctx, -2, SYMBOL);
	duk_pop(ctx);

	return 0;
}

static duk_ret_t
js_sound_pause(duk_context *ctx)
{
	(void)ctx;

	sound_pause(NULL);

	return 0;
}

static duk_ret_t
js_sound_resume(duk_context *ctx)
{
	(void)ctx;

	sound_resume(NULL);

	return 0;
}

static duk_ret_t
js_sound_stop(duk_context *ctx)
{
	sound_stop(NULL, duk_opt_uint(ctx, 0, 0));

	return 0;
}

static duk_ret_t
js_sound_proto_play(duk_context *ctx)
{
	struct sound *snd = js_sound_this(ctx);
	int channel = duk_opt_int(ctx, 0, -1);
	unsigned int fadein = duk_opt_uint(ctx, 1, 0);

	if (!sound_play(snd, channel, fadein))
		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	return 0;
}

static duk_ret_t
js_sound_proto_pause(duk_context *ctx)
{
	sound_pause(js_sound_this(ctx));

	return 0;
}

static duk_ret_t
js_sound_proto_resume(duk_context *ctx)
{
	sound_resume(js_sound_this(ctx));

	return 0;
}

static duk_ret_t
js_sound_proto_stop(duk_context *ctx)
{
	sound_stop(js_sound_this(ctx), duk_opt_uint(ctx, 0, 0));

	return 0;
}

static duk_ret_t
js_sound_finish(duk_context *ctx)
{
	struct sound *snd;

	duk_get_prop_string(ctx, 0, SYMBOL);

	if ((snd = duk_to_pointer(ctx, -1))) {
		sound_finish(snd);
		free(duk_to_pointer(ctx, -1));
	}

	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SYMBOL);

	return 0;
}

const duk_number_list_entry constants[] = {
	{ "CHANNEL_MAX",        SOUND_CHANNELS_MAX,     },
	{ NULL,                 0                       }
};

const duk_function_list_entry functions[] = {
	{ "pause",              js_sound_pause,         0               },
	{ "resume",             js_sound_resume,        0               },
	{ "stop",               js_sound_stop,          DUK_VARARGS     },
	{ NULL,                 NULL,                   0               }
};

const duk_function_list_entry methods[] = {
	{ "play",               js_sound_proto_play,    DUK_VARARGS     },
	{ "pause",              js_sound_proto_pause,   0               },
	{ "resume",             js_sound_proto_resume,  0               },
	{ "stop",               js_sound_proto_stop,    DUK_VARARGS     },
	{ NULL,                 NULL,                   0               }
};

void
js_sound_load(duk_context *ctx)
{
	assert(ctx);

	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1, "Molko");
	duk_push_c_function(ctx, js_sound_new, 3);
	duk_put_function_list(ctx, -1, functions);
	duk_put_number_list(ctx, -1, constants);
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, methods);
	duk_push_c_function(ctx, js_sound_finish, 1);
	duk_set_finalizer(ctx, -2);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_prop_string(ctx, -2, "Sound");
	duk_pop_n(ctx, 2);
}