/*
 * js-music.c -- music support (Javascript bindings)
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
#include <core/music.h>

#include "js-music.h"

#define SYMBOL          DUK_HIDDEN_SYMBOL("molko::music")

static struct music *
js_music_this(duk_context *ctx)
{
	struct music *mus;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SYMBOL);
	mus = duk_to_pointer(ctx, -1);
	duk_pop_n(ctx, 2);

	if (!mus)
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "Not a Music object");

	return mus;
}

static duk_ret_t
js_music_new(duk_context *ctx)
{
	struct music mus;
	const char *path;

	if (!duk_is_constructor_call(ctx))
		return duk_error(ctx, DUK_ERR_TYPE_ERROR, "Music must be new-constructed");

	path = duk_require_string(ctx, 0);

	if (!music_open(&mus, path))
		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	duk_push_this(ctx);
	duk_push_pointer(ctx, alloc_dup(&mus, sizeof (mus)));
	duk_put_prop_string(ctx, -2, SYMBOL);
	duk_pop(ctx);

	return 0;
}

static duk_ret_t
js_music_playing(duk_context *ctx)
{
	duk_push_boolean(ctx, music_playing());

	return 1;
}

static duk_ret_t
js_music_pause(duk_context *ctx)
{
	(void)ctx;

	music_pause();

	return 0;
}

static duk_ret_t
js_music_resume(duk_context *ctx)
{
	(void)ctx;

	music_resume();

	return 0;
}

static duk_ret_t
js_music_stop(duk_context *ctx)
{
	music_stop(duk_get_uint(ctx, 0));

	return 0;
}

static duk_ret_t
js_music_play(duk_context *ctx)
{
	struct music *mus = js_music_this(ctx);
	enum music_flags flags = duk_get_uint(ctx, 0);
	unsigned int fadein = duk_get_uint(ctx, 1);

	if (!music_play(mus, flags, fadein))
		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	return 0;
}

static duk_ret_t
js_music_finish(duk_context *ctx)
{
	struct music *mus;

	duk_get_prop_string(ctx, 0, SYMBOL);

	if ((mus = duk_to_pointer(ctx, -1))) {
		music_finish(mus);
		free(duk_to_pointer(ctx, -1));
	}

	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SYMBOL);

	return 0;
}

static const duk_number_list_entry flags[] = {
	{ "NONE",       MUSIC_NONE                              },
	{ "LOOP",       MUSIC_LOOP                              },
	{ NULL,         0                                       }
};

static const duk_function_list_entry functions[] = {
	{ "playing",    js_music_playing,       0               },
	{ "pause",      js_music_pause,         0               },
	{ "resume",     js_music_resume,        0               },
	{ "stop",       js_music_stop,          DUK_VARARGS     },
	{ NULL,         NULL,                   0               }
};

static const duk_function_list_entry methods[] = {
	{ "play",       js_music_play,          DUK_VARARGS     },
	{ NULL,         NULL,                   0               }
};

void
js_music_load(duk_context *ctx)
{
	assert(ctx);

	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1, "Molko");
	duk_push_c_function(ctx, js_music_new, 3);
	duk_put_function_list(ctx, -1, functions);
	duk_put_number_list(ctx, -1, flags);
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, methods);
	duk_push_c_function(ctx, js_music_finish, 1);
	duk_set_finalizer(ctx, -2);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_prop_string(ctx, -2, "Music");
	duk_pop_n(ctx, 2);
}
