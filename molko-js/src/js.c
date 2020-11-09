/*
 * js.c -- javascript loader
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
#include <libgen.h>
#include <limits.h>
#include <string.h>

#include <core/alloc.h>
#include <core/error.h>
#include <core/panic.h>
#include <core/trace.h>

#include <duktape.h>
#include <duk_module.h>

#include "js.h"
#include "js-clock.h"
#include "js-event.h"
#include "js-font.h"
#include "js-painter.h"
#include "js-texture.h"
#include "js-sprite.h"
#include "js-util.h"
#include "js-window.h"

static duk_ret_t
js_print(duk_context *ctx)
{
	puts(duk_require_string(ctx, 0));

	return 0;
}

static duk_ret_t
js_trace(duk_context *ctx)
{
	tracef("%s", duk_require_string(ctx, 0));

	return 0;
}

static void *
wrap_malloc(void *udata, duk_size_t size)
{
	(void)udata;

	return allocator.alloc(size);
}

static void *
wrap_realloc(void *udata, void *oldptr, duk_size_t size)
{
	(void)udata;

	return allocator.realloc(oldptr, size);
}

static void
wrap_free(void *udata, void *ptr)
{
	(void)udata;

	allocator.free(ptr);
}

static void
wrap_fatal(void *udata, const char *msg)
{
	(void)udata;

	panicf("%s", msg);
}

static duk_ret_t
search(duk_context *ctx)
{
	char path[PATH_MAX];
	char *ret;

	duk_push_global_stash(ctx);
	duk_get_prop_string(ctx, -1, DUK_HIDDEN_SYMBOL("molko::base"));
	snprintf(path, sizeof (path), "%s/%s.js", duk_to_string(ctx, -1), duk_require_string(ctx, 0));
	duk_pop_n(ctx, 2);

	tracef("opening module: %s", path);

	if (!(ret = readall(path)))
		duk_error(ctx, DUK_ERR_ERROR, "%s", error());
	
	duk_push_string(ctx, ret);
	free(ret);

	return 1;
}

static void
setup_module(struct js *js)
{
	duk_module_duktape_init(js->handle);
	duk_get_global_string(js->handle, "Duktape");
	duk_push_c_function(js->handle, search, 4);
	duk_put_prop_string(js->handle, -2, "modSearch");
	duk_pop(js->handle);
}

static void
setup_global(struct js *js)
{
	/* Create global "Molko" property. */
	duk_push_global_object(js->handle);
	duk_push_object(js->handle);
	duk_put_prop_string(js->handle, -2, "Molko");
	duk_push_c_function(js->handle, js_print, 1);
	duk_put_prop_string(js->handle, -2, "print");
	duk_push_c_function(js->handle, js_trace, 1);
	duk_put_prop_string(js->handle, -2, "trace");
	duk_pop(js->handle);
}

static void
setup_properties(struct js *js)
{
	/* Store a reference to this pointer. */
	duk_push_global_stash(js->handle);
	duk_push_pointer(js->handle, js);
	duk_put_prop_string(js->handle, -2, DUK_HIDDEN_SYMBOL("js.pointer"));
	duk_pop(js->handle);
}

static void
setup_base(struct js *js, const char *path)
{
	char base[PATH_MAX];

	snprintf(base, sizeof (base), "%s", path);
	snprintf(base, sizeof (base), "%s", dirname(base));

	duk_push_global_stash(js->handle);
	duk_push_string(js->handle, base);
	duk_put_prop_string(js->handle, -2, DUK_HIDDEN_SYMBOL("molko::base"));
	duk_pop(js->handle);
}

bool
js_init(struct js *js)
{
	assert(js);

	js->handle = duk_create_heap(wrap_malloc, wrap_realloc, wrap_free,
	    NULL, wrap_fatal);

	if (!js->handle)
		panicf("could not create Javascript context");

	/* Setup module. */
	setup_module(js);
	setup_global(js);
	setup_properties(js);
	
	return true;
}

void
js_core_load(struct js *js)
{
	assert(js);

	js_clock_load(js);
	js_event_load(js);
	js_font_load(js);
	js_painter_load(js);
	js_texture_load(js);
	js_sprite_load(js);
	js_util_load(js);
	js_window_load(js);
}

bool
js_open(struct js *js, const char *path)
{
	assert(js);
	assert(path);

	char *text = readall(path);
	bool ret = true;

	if (!text)
		return false;

	setup_base(js, path);

	if (duk_peval_string(js->handle, text) != 0) {
		int ln;

		duk_get_prop_string(js->handle, -1, "lineNumber");
		ln = duk_to_int(js->handle, -1);
		duk_pop(js->handle);
		errorf("%d: %s", ln, duk_safe_to_string(js->handle, -1));
		ret = false;
	}

	free(text);
	duk_pop(js->handle);

	return ret;
}

bool
js_openmem(struct js *js, const void *data, size_t datasz)
{
	assert(js);
	assert(data);
	
	(void)js;
	(void)data;
	(void)datasz;

	return true;
}

void
js_finish(struct js *js)
{
	assert(js);

	duk_destroy_heap(js->handle);
	memset(js, 0, sizeof (*js));
}
