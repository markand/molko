/*
 * js.c -- javascript loader
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
#include <string.h>

#include <mlk/core/alloc.h>
#include <mlk/core/err.h>
#include <mlk/core/panic.h>
#include <mlk/core/trace.h>
#include <mlk/core/vfs.h>

#include <duktape.h>
#include <duk_module.h>

#include "js-animation.h"
#include "js-clock.h"
#include "js-event.h"
#include "js-font.h"
#include "js-music.h"
#include "js-painter.h"
#include "js-sound.h"
#include "js-sprite.h"
#include "js-texture.h"
#include "js-util.h"
#include "js-window.h"
#include "js.h"

#define JS_REF DUK_HIDDEN_SYMBOL("mlk::js")

static void *
wrap_malloc(void *udata, duk_size_t size)
{
	(void)udata;

	return mlk_alloc_new(1, size);
}

static void *
wrap_realloc(void *udata, void *oldptr, duk_size_t size)
{
	(void)udata;

	void *ptr = NULL;

	if (oldptr) {
		if (!size)
			mlk_alloc_free(oldptr);
		else
			ptr = mlk_alloc_resize(oldptr, size);
	} else if (size)
		ptr = mlk_alloc_new(1, size);

	return ptr;
}

static void
wrap_free(void *udata, void *ptr)
{
	(void)udata;

	mlk_alloc_free(ptr);
}

static void
wrap_fatal(void *udata, const char *msg)
{
	(void)udata;

	mlk_panicf("%s", msg);
}

static char *
read_all(struct mlk_js *js, const char *filename, size_t *len)
{
	struct mlk_vfs_file *file;
	char *text;

	if (!(file = mlk_vfs_open(js->vfs, filename, "r")))
		return NULL;

	text = mlk_vfs_file_read_all(file, len);
	mlk_vfs_file_finish(file);

	return text;
}

static struct mlk_js *
mlk_js_this(duk_context *ctx)
{
	struct mlk_js *js;

	duk_push_global_stash(ctx);
	duk_get_prop_string(ctx, -1, JS_REF);
	js = duk_to_pointer(ctx, -1);
	duk_pop(ctx);

	if (!js)
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "Not a Javascript context");

	return js;
}

static duk_ret_t
mlk_js_print(duk_context *ctx)
{
	puts(duk_require_string(ctx, 0));

	return 0;
}

static duk_ret_t
mlk_js_trace(duk_context *ctx)
{
	mlk_tracef("%s", duk_require_string(ctx, 0));

	return 0;
}

static duk_ret_t
mlk_js_modSearch(duk_context *ctx)
{
	struct mlk_js *js;
	const char *filename;
	char *text;
	size_t textsz;

	js = mlk_js_this(ctx);
	filename = duk_require_string(ctx, 0);

	if (!(text = read_all(js, filename, &textsz)))
		duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());

	duk_push_lstring(ctx, text, textsz);
	free(text);

	return 1;
}

static void
setup_module(struct mlk_js *js)
{
	duk_module_duktape_init(js->handle);
	duk_get_global_string(js->handle, "Duktape");
	duk_push_c_function(js->handle, mlk_js_modSearch, 4);
	duk_put_prop_string(js->handle, -2, "modSearch");
	duk_pop(js->handle);
}

static void
setup_global(struct mlk_js *js)
{
	/* Create global "Mlk" property. */
	duk_push_global_object(js->handle);
	duk_push_object(js->handle);
	duk_put_prop_string(js->handle, -2, "Mlk");
	duk_push_c_function(js->handle, mlk_js_print, 1);
	duk_put_prop_string(js->handle, -2, "print");
	duk_push_c_function(js->handle, mlk_js_trace, 1);
	duk_put_prop_string(js->handle, -2, "trace");
	duk_pop(js->handle);
}

static void
setup_internal(struct mlk_js *js)
{
	duk_push_global_stash(js->handle);

	/* Store a reference to this pointer. */
	duk_push_pointer(js->handle, js);
	duk_put_prop_string(js->handle, -2, JS_REF);

	duk_pop(js->handle);
}

static void
setup_core(struct mlk_js *js)
{
	mlk_js_animation_load(js->handle);
	mlk_js_clock_load(js->handle);
	mlk_js_event_load(js->handle);
	mlk_js_font_load(js->handle);
	mlk_js_music_load(js->handle);
	mlk_js_painter_load(js->handle);
	mlk_js_sound_load(js->handle);
	mlk_js_sprite_load(js->handle);
	mlk_js_texture_load(js->handle);
	mlk_js_util_load(js->handle);
	mlk_js_window_load(js->handle);
}

int
mlk_js_init(struct mlk_js *js, struct mlk_vfs *vfs)
{
	assert(js);
	assert(vfs);

	js->vfs = vfs;
	js->handle = duk_create_heap(wrap_malloc, wrap_realloc, wrap_free,
	    NULL, wrap_fatal);

	if (!js->handle) {
		mlk_errf("could not create Javascript context");
		return -1;
	}

	setup_internal(js);
	setup_global(js);
	setup_module(js);
	setup_core(js);

	return 0;
}

int
mlk_js_run(struct mlk_js *js, const char *main)
{
	assert(js);
	assert(main);

	int line, rv = -1;
	char *text;
	size_t textsz;

	if (!(text = read_all(js, main, &textsz)))
		return -1;

	if (duk_peval_lstring(js->handle, text, textsz) != 0) {
		duk_get_prop_string(js->handle, -1, "lineNumber");
		line = duk_to_int(js->handle, -1);
		duk_pop(js->handle);
		mlk_errf("%d: %s", line, duk_safe_to_string(js->handle, -1));
	} else
		rv = 0;

	mlk_alloc_free(text);

	return rv;
}

void
mlk_js_finish(struct mlk_js *js)
{
	assert(js);

	duk_destroy_heap(js->handle);
	memset(js, 0, sizeof (*js));
}
