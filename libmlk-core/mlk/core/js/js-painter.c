/*
 * js-painter.c -- basic drawing routines (Javascript bindings)
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

#include <duktape.h>

#include <mlk/core/painter.h>

#include "js.h"
#include "js-painter.h"

#define SYMBOL          DUK_HIDDEN_SYMBOL("mlk::sprite")
#define TEXTURE_REF     DUK_HIDDEN_SYMBOL("mlk::sprite::texture")

static duk_ret_t
mlk_js_painter_getColor(duk_context *ctx)
{
	duk_push_number(ctx, mlk_painter_get_color());

	return 1;
}

static duk_ret_t
mlk_js_painter_setColor(duk_context *ctx)
{
	mlk_painter_set_color(duk_require_number(ctx, 0));

	return 0;
}

static duk_ret_t
mlk_js_painter_clear(duk_context *ctx)
{
	(void)ctx;

	mlk_painter_clear();

	return 0;
}

static duk_ret_t
mlk_js_painter_drawLine(duk_context *ctx)
{
	int x1, y1, x2, y2;

	if (duk_get_top(ctx) == 4) {
		x1 = duk_require_int(ctx, 0);
		y1 = duk_require_int(ctx, 1);
		x2 = duk_require_int(ctx, 2);
		y2 = duk_require_int(ctx, 3);
	} else if (duk_get_top(ctx) == 1) {
		duk_require_object(ctx, 0);
		duk_get_prop_string(ctx, 0, "x1");
		x1 = duk_require_int(ctx, -1);
		duk_get_prop_string(ctx, 0, "y1");
		y1 = duk_require_int(ctx, -1);
		duk_get_prop_string(ctx, 0, "x2");
		x2 = duk_require_int(ctx, -1);
		duk_get_prop_string(ctx, 0, "y2");
		y2 = duk_require_int(ctx, -1);
		duk_pop_n(ctx, 4);
	} else
		return duk_error(ctx, DUK_ERR_ERROR, "Object or 4 numbers expected");

	mlk_painter_draw_line(x1, y1, x2, y2);

	return 0;
}

static duk_ret_t
mlk_js_painter_drawPoint(duk_context *ctx)
{
	int x, y;

	if (duk_get_top(ctx) == 2) {
		x = duk_require_int(ctx, 0);
		y = duk_require_int(ctx, 1);
	} else if (duk_get_top(ctx) == 1) {
		duk_require_object(ctx, 0);
		duk_get_prop_string(ctx, 0, "x");
		x = duk_require_int(ctx, -1);
		duk_get_prop_string(ctx, 0, "y");
		y = duk_require_int(ctx, -1);
		duk_pop_n(ctx, 2);
	} else
		return duk_error(ctx, DUK_ERR_ERROR, "Object or 2 numbers expected");

	mlk_painter_draw_point(x, y);

	return 0;
}

static duk_ret_t
mlk_js_painter_drawRectangle(duk_context *ctx)
{
	int x, y;
	unsigned int w, h;

	if (duk_get_top(ctx) == 4) {
		x = duk_require_int(ctx, 0);
		y = duk_require_int(ctx, 1);
		w = duk_require_uint(ctx, 2);
		h = duk_require_uint(ctx, 3);
	} else if (duk_get_top(ctx) == 1) {
		duk_require_object(ctx, 0);
		duk_get_prop_string(ctx, 0, "x");
		x = duk_require_int(ctx, -1);
		duk_get_prop_string(ctx, 0, "y");
		y = duk_require_int(ctx, -1);
		duk_get_prop_string(ctx, 0, "w");
		w = duk_require_uint(ctx, -1);
		duk_get_prop_string(ctx, 0, "h");
		h = duk_require_uint(ctx, -1);
		duk_pop_n(ctx, 4);
	} else
		return duk_error(ctx, DUK_ERR_ERROR, "Object or 4 numbers expected");

	mlk_painter_draw_rectangle(x, y, w, h);

	return 0;
}

static duk_ret_t
mlk_js_painter_drawCircle(duk_context *ctx)
{
	int x, y;
	double r;

	if (duk_get_top(ctx) == 3) {
		x = duk_require_int(ctx, 0);
		y = duk_require_int(ctx, 1);
		r = duk_require_number(ctx, 2);
	} else if (duk_get_top(ctx) == 1) {
		duk_require_object(ctx, 0);
		duk_get_prop_string(ctx, 0, "x");
		x = duk_require_int(ctx, -1);
		duk_get_prop_string(ctx, 0, "y");
		y = duk_require_int(ctx, -1);
		duk_get_prop_string(ctx, 0, "r");
		r = duk_require_number(ctx, -1);
		duk_pop_n(ctx, 3);
	} else
		return duk_error(ctx, DUK_ERR_ERROR, "Object or 3 numbers expected");

	mlk_painter_draw_circle(x, y, r);

	return 0;
}

static duk_ret_t
mlk_js_painter_present(duk_context *ctx)
{
	(void)ctx;

	mlk_painter_present();

	return 0;
}

static const duk_function_list_entry functions[] = {
	{ "clear",              mlk_js_painter_clear,           0               },
	{ "drawLine",           mlk_js_painter_drawLine,        DUK_VARARGS     },
	{ "drawPoint",          mlk_js_painter_drawPoint,       DUK_VARARGS     },
	{ "drawRectangle",      mlk_js_painter_drawRectangle,   DUK_VARARGS     },
	{ "drawCircle",         mlk_js_painter_drawCircle,      DUK_VARARGS     },
	{ "present",            mlk_js_painter_present,         0               },
	{ NULL,                 NULL,                           0               }
};

void
mlk_js_painter_load(duk_context *ctx)
{
	assert(ctx);

	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1, "Mlk");
	duk_push_object(ctx);
	duk_push_string(ctx, "color");
	duk_push_c_function(ctx, mlk_js_painter_getColor, 0);
	duk_push_c_function(ctx, mlk_js_painter_setColor, 1);
	duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);
	duk_put_function_list(ctx, -1, functions);
	duk_put_prop_string(ctx, -2, "Painter");
	duk_pop(ctx);
}
