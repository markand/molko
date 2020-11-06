/*
 * js-painter.c -- basic drawing routines (Javascript bindings)
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

#include <duktape.h>

#include <core/painter.h>

#include "js.h"
#include "js-painter.h"

static duk_ret_t
js_painter_getColor(duk_context *ctx)
{
	duk_push_number(ctx, painter_get_color());

	return 1;
}

static duk_ret_t
js_painter_setColor(duk_context *ctx)
{
	painter_set_color(duk_require_number(ctx, 0));

	return 0;
}

static duk_ret_t
js_painter_new(duk_context *ctx)
{
	if (!duk_is_constructor_call(ctx))
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "Painter must be new-constructed");

	duk_push_this(ctx);
	duk_push_string(ctx, "color");
	duk_push_c_function(ctx, js_painter_getColor, 0);
	duk_push_c_function(ctx, js_painter_setColor, 1);
	duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);

	return 0;
}

static duk_ret_t
js_painter_clear(duk_context *ctx)
{
	(void)ctx;

	painter_clear();

	return 0;
}

static duk_ret_t
js_painter_drawLine(duk_context *ctx)
{
	painter_draw_line(
	    duk_require_int(ctx, 0),
	    duk_require_int(ctx, 1),
	    duk_require_int(ctx, 2),
	    duk_require_int(ctx, 3)
	);

	return 0;
}

static duk_ret_t
js_painter_drawPoint(duk_context *ctx)
{
	painter_draw_point(
	    duk_require_int(ctx, 0),
	    duk_require_int(ctx, 1)
	);

	return 0;
}

static duk_ret_t
js_painter_drawRectangle(duk_context *ctx)
{
	painter_draw_rectangle(
	    duk_require_int(ctx, 0),
	    duk_require_int(ctx, 1),
	    duk_require_int(ctx, 2),
	    duk_require_int(ctx, 3)
	);
	
	return 0;
}

static duk_ret_t
js_painter_drawCircle(duk_context *ctx)
{
	painter_draw_circle(
	    duk_require_int(ctx, 0),
	    duk_require_int(ctx, 1),
	    duk_require_number(ctx, 2)
	);

	return 0;
}

static duk_ret_t
js_painter_present(duk_context *ctx)
{
	(void)ctx;

	painter_present();

	return 0;
}

static const duk_function_list_entry methods[] = {
	{ "clear",              js_painter_clear,               0 },
	{ "drawLine",           js_painter_drawLine,            4 },
	{ "drawPoint",          js_painter_drawPoint,           2 },
	{ "drawRectangle",      js_painter_drawRectangle,       4 },
	{ "drawCircle",         js_painter_drawCircle,          3 },
	{ "present",            js_painter_present,             0 },
	{ NULL,                 NULL,                           0 }
};

void
js_painter_load(struct js *js)
{
	assert(js);

	duk_push_global_object(js->handle);
	duk_get_prop_string(js->handle, -1, "Molko");
	duk_push_c_function(js->handle, js_painter_new, 0);
	duk_push_object(js->handle);
	duk_put_function_list(js->handle, -1, methods);
	duk_put_prop_string(js->handle, -2, "prototype");
	duk_put_prop_string(js->handle, -2, "Painter");
	duk_pop_n(js->handle, 2);
}
