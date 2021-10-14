/*
 * js-painter.c -- core painter binding
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

#include <core/painter.h>

#include "js-painter.h"
#include "js-texture.h"

static duk_ret_t
Painter_getTarget(duk_context *ctx)
{
	struct texture *tex;

	if (!(tex = painter_get_target()))
		duk_push_null(ctx);
	else
		js_texture_push(ctx, tex);

	return 1;
}

static duk_ret_t
Painter_setTarget(duk_context *ctx)
{
	if (duk_is_null(ctx, 0))
		painter_set_target(NULL);
	else
		painter_set_target(js_texture_require(ctx, 0));

	return 0;
}

static duk_ret_t
Painter_getColor(duk_context *ctx)
{
	duk_push_number(ctx, painter_get_color());

	return 1;
}

static duk_ret_t
Painter_setColor(duk_context *ctx)
{
	const unsigned long color = duk_require_number(ctx, 0);

	painter_set_color(color);

	return 0;
}

static duk_ret_t
Painter_drawLine(duk_context *ctx)
{
	const int x1 = duk_require_int(ctx, 0);
	const int y1 = duk_require_int(ctx, 1);
	const int x2 = duk_require_int(ctx, 2);
	const int y2 = duk_require_int(ctx, 3);

	painter_draw_line(x1, y1, x2, y2);

	return 0;
}

static duk_ret_t
Painter_drawPoint(duk_context *ctx)
{
	const int x = duk_require_int(ctx, 0);
	const int y = duk_require_int(ctx, 1);

	painter_draw_point(x, y);

	return 0;
}

static duk_ret_t
Painter_drawRectangle(duk_context *ctx)
{
	const int x = duk_require_int(ctx, 0);
	const int y = duk_require_int(ctx, 1);
	const unsigned int w = duk_require_uint(ctx, 2);
	const unsigned int h = duk_require_uint(ctx, 3);

	painter_draw_rectangle(x, y, w, h);

	return 0;
}

static duk_ret_t
Painter_drawCircle(duk_context *ctx)
{
	const int x = duk_require_int(ctx, 0);
	const int y = duk_require_int(ctx, 1);
	const int r = duk_require_int(ctx, 2);

	painter_draw_circle(x, y, r);

	return 0;
}

static duk_ret_t
Painter_clear(duk_context *ctx)
{
	(void)ctx;

	painter_clear();

	return 0;
}

static duk_ret_t
Painter_present(duk_context *ctx)
{
	(void)ctx;

	painter_present();

	return 0;
}

static const duk_function_list_entry functions[] = {
	{ "drawLine",           Painter_drawLine,       4       },
	{ "drawPoint",          Painter_drawPoint,      2       },
	{ "drawRectangle",      Painter_drawRectangle,  4       },
	{ "drawCircle",         Painter_drawCircle,     3       },
	{ "clear",              Painter_clear,          0       },
	{ "present",            Painter_present,        0       },
	{ NULL,                 NULL,                   0       }
};

void
js_painter_bind(duk_context *ctx)
{
	assert(ctx);

	duk_push_global_object(ctx);
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, functions);
	duk_push_string(ctx, "color");
	duk_push_c_function(ctx, Painter_getColor, 0);
	duk_push_c_function(ctx, Painter_setColor, 1);
	duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_SETTER | DUK_DEFPROP_HAVE_GETTER);
	duk_push_string(ctx, "target");
	duk_push_c_function(ctx, Painter_getTarget, 0);
	duk_push_c_function(ctx, Painter_setTarget, 1);
	duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_SETTER | DUK_DEFPROP_HAVE_GETTER);
	duk_put_prop_string(ctx, -2, "Painter");
	duk_pop(ctx);
}
