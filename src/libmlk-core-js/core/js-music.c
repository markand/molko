/*
 * js-music.c -- core music binding
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
#include <core/music.h>
#include <core/vfs.h>

#include "js-core.h"
#include "js-music.h"

#define SIGNATURE DUK_HIDDEN_SYMBOL("Mlk.Music")

static inline struct music *
self(duk_context *ctx)
{
	struct music *mus;

	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, SIGNATURE);
	mus = duk_to_pointer(ctx, -1);
	duk_pop_2(ctx);

	if (!mus)
		return (void)duk_error(ctx, DUK_ERR_TYPE_ERROR, "not a Music object"), NULL;

	return mus;
}

static duk_ret_t
Music_new(duk_context *ctx)
{
	const char *entry = duk_require_string(ctx, 0);
	struct vfs_file file;
	struct music *mus;

	if (vfs_open(js_core_global_vfs(ctx), &file, entry, "r") < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	mus = alloc_new0(sizeof (*mus));

	if (music_openvfs(mus, &file) < 0) {
		free(mus);
		vfs_file_finish(&file);

		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());
	}

	vfs_file_finish(&file);

	duk_push_this(ctx);
	duk_push_pointer(ctx, mus);
	duk_put_prop_string(ctx, -2, SIGNATURE);
	duk_pop(ctx);

	return 0;
}

static duk_ret_t
Music_play(duk_context *ctx)
{
	const int flags = duk_get_int_default(ctx, 0, MUSIC_NONE);
	const unsigned int fadein = duk_get_uint_default(ctx, 0, 0);

	if (music_play(self(ctx), flags, fadein) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", error());

	return 0;
}

static duk_ret_t
Music_playing(duk_context *ctx)
{
	duk_push_int(ctx, music_playing());

	return 0;
}

static duk_ret_t
Music_pause(duk_context *ctx)
{
	(void)ctx;

	music_pause();

	return 0;
}

static duk_ret_t
Music_resume(duk_context *ctx)
{
	(void)ctx;

	music_resume();

	return 0;
}

static duk_ret_t
Music_stop(duk_context *ctx)
{
	music_stop(duk_get_uint_default(ctx, 0, 0));

	return 0;
}

static duk_ret_t
Music_destructor(duk_context *ctx)
{
	struct music *mus;

	duk_get_prop_string(ctx, 0, SIGNATURE);

	if ((mus = duk_to_pointer(ctx, -1)))
		music_finish(mus);

	duk_pop(ctx);
	duk_del_prop_string(ctx, 0, SIGNATURE);

	return 0;
}

static const duk_function_list_entry methods[] = {
	{ "play",               Music_play,             DUK_VARARGS     },
	{ NULL,                 NULL,                   0               }
};

static const duk_function_list_entry functions[] = {
	{ "playing",            Music_playing,          0               },
	{ "pause",              Music_pause,            0               },
	{ "resume",             Music_resume,           0               },
	{ "stop",               Music_stop,             DUK_VARARGS     },
	{ NULL,                 NULL,                   0               }
};

static const duk_number_list_entry flags[] = {
	{ "NONE",       MUSIC_NONE              },
	{ "LOOP",       MUSIC_LOOP              },
	{ NULL,         0                       }
};

void
js_music_bind(duk_context *ctx)
{
	assert(ctx);

	duk_push_c_function(ctx, Music_new, 1);
	duk_put_function_list(ctx, -1, functions);
	duk_push_object(ctx);
	duk_put_number_list(ctx, -1, flags);
	duk_put_prop_string(ctx, -2, "Flags");
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, functions);
	duk_put_function_list(ctx, -1, methods);
	duk_push_c_function(ctx, Music_destructor, 1);
	duk_set_finalizer(ctx, -2);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_global_string(ctx, "Music");
}
