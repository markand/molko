/*
 * notify.c -- in game notifications
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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
#include <stddef.h>
#include <string.h>

#include <mlk/core/font.h>
#include <mlk/core/painter.h>
#include <mlk/core/texture.h>
#include <mlk/core/trace.h>
#include <mlk/core/util.h>
#include <mlk/core/window.h>

#include "align.h"
#include "notify.h"
#include "ui.h"
#include "ui_p.h"

#define WIDTH   (mlk_window.w / 2.5)
#define HEIGHT  (mlk_window.h / 10)
#define MAX     (4)
#define FONT    (mlk_notify_style.text_font ? mlk_notify_style.text_font : &mlk_ui_fonts[MLK_UI_FONT_INTERFACE])

struct geo {
	const struct mlk_theme *theme;
	int frame_x;
	int frame_y;
	unsigned int frame_w;
	unsigned int frame_h;
	int icon_x;
	int icon_y;
	int title_x;
	int title_y;
	int body_x;
	int body_y;
};

static struct mlk_notify stack[MAX];

static void
geometry(struct geo *geo, const struct mlk_notify *n, size_t index)
{
	struct mlk_font *font;
	int x, y;

	/* Determine notification position. */
	x  = mlk_window.w - mlk_notify_style.padding;
	x -= WIDTH;

	y  = mlk_notify_style.padding * (index + 1);
	y += HEIGHT * index;

	font = &mlk_ui_fonts[MLK_UI_FONT_INTERFACE];

	/* Content frame. */
	geo->frame_x = x;
	geo->frame_y = y;
	geo->frame_w = WIDTH;
	geo->frame_h = HEIGHT;

	/* Align icon at the left center. */
	if (n->icon->h >= HEIGHT) {
		mlk_tracef(_("notification icon is too large: %u > %u"), n->icon->h, HEIGHT);
		geo->icon_x = x + mlk_notify_style.padding;
		geo->icon_y = y + mlk_notify_style.padding;
	} else {
		mlk_align(MLK_ALIGN_LEFT, &geo->icon_x, &geo->icon_y, n->icon->w, n->icon->h, x, y, WIDTH, HEIGHT);
		geo->icon_x += geo->icon_y - y;
	}

	/* Align title to the right of the icon at the same y coordinate. */
	geo->title_x  = geo->icon_x + n->icon->w + mlk_notify_style.padding;
	geo->title_y  = geo->icon_y + (mlk_notify_style.padding / 2);
	geo->title_y -= mlk_font_height(font) / 2;

	/* Align body so it ends at the end of the icon. */
	geo->body_x  = geo->title_x;
	geo->body_y  = geo->icon_y + n->icon->h - (mlk_notify_style.padding / 2);
	geo->body_y -= mlk_font_height(font) / 2;
}

static void
draw_frame(const struct geo *geo)
{
	mlk_painter_set_color(mlk_notify_style.border);
	mlk_painter_draw_rectangle(geo->frame_x, geo->frame_y, geo->frame_w, geo->frame_h);
	mlk_painter_set_color(mlk_notify_style.background);
	mlk_painter_draw_rectangle(
		geo->frame_x +  mlk_notify_style.border_size,
		geo->frame_y +  mlk_notify_style.border_size,
		geo->frame_w - (mlk_notify_style.border_size * 2),
		geo->frame_h - (mlk_notify_style.border_size * 2)
	);
}

static void
draw_icon(const struct geo *geo, const struct mlk_notify *n)
{
	mlk_texture_draw(n->icon, geo->icon_x, geo->icon_y);
}

static void
draw_title(const struct geo *geo, const struct mlk_notify *n)
{
	mlk_ui_draw_text(
		MLK_ALIGN_NONE,
		MLK__STYLE_FONT(mlk_notify_style.font, MLK_UI_FONT_INTERFACE),
		mlk_notify_style.color,
		n->title,
		geo->title_x,
		geo->title_y,
		0,
		0
	);
}

static void
draw_body(const struct geo *geo, const struct mlk_notify *n)
{
	mlk_ui_draw_text(
		MLK_ALIGN_NONE,
		MLK__STYLE_FONT(mlk_notify_style.font, MLK_UI_FONT_INTERFACE),
		mlk_notify_style.color,
		n->body,
		geo->body_x,
		geo->body_y,
		0,
		0
	);
}

static void
draw(const struct mlk_notify *n, size_t index)
{
	struct geo geo;

	/* Compute notification size and widgets. */
	geometry(&geo, n, index);

	draw_frame(&geo);
	draw_icon(&geo, n);
	draw_title(&geo, n);
	draw_body(&geo, n);
}

struct mlk_notify_style mlk_notify_style = {
	.background     = MLK_UI_COLOR_BG,
	.border         = MLK_UI_COLOR_BORDER,
	.border_size    = 2,
	.timeout        = 5000,
	.padding        = 10,
	.stack          = stack,
	.stacksz        = MLK_UTIL_SIZE(stack),
	.draw           = draw
};

void
mlk_notify(const struct mlk_texture *icon, const char *title, const char *body)
{
	assert(body);

	struct mlk_notify *n;

	if (mlk_notify_style.length >= mlk_notify_style.stacksz) {
		memmove(&mlk_notify_style.stack[0],
			&mlk_notify_style.stack[1],
			sizeof (mlk_notify_style.stack[0]) - mlk_notify_style.stacksz - 1);
		n = &mlk_notify_style.stack[mlk_notify_style.length - 1];
	} else
		n = &mlk_notify_style.stack[mlk_notify_style.length++];

	memset(n, 0, sizeof (*n));
	n->icon = icon;
	n->title = title;
	n->body = body;
}

void
mlk_notify_clear(void)
{
	mlk_notify_style.length = 0;
}

void
mlk_notify_update(unsigned int ticks)
{
	struct mlk_notify *n;

	for (size_t i = 0; i < mlk_notify_style.length; ++i) {
		n = &mlk_notify_style.stack[i];
		n->elapsed += ticks;

		if (n->elapsed >= mlk_notify_style.timeout)
			memmove(n, n + 1, sizeof (*n) * (--mlk_notify_style.length - i));
	}
}

void
mlk_notify_draw(void)
{
	if (!mlk_notify_style.draw)
		return;

	for (size_t i = 0; i < mlk_notify_style.length; ++i)
		mlk_notify_style.draw(&mlk_notify_style.stack[i], i);
}
