/*
 * notify.c -- in game notifications
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
#include <stddef.h>
#include <string.h>

#include <mlk/core/font.h>
#include <mlk/core/texture.h>
#include <mlk/core/trace.h>
#include <mlk/core/window.h>

#include "align.h"
#include "frame.h"
#include "label.h"
#include "notify.h"
#include "theme.h"

#define WIDTH   (mlk_window.w / 3)
#define HEIGHT  (mlk_window.h / 10)

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

static void draw(const struct mlk_notify *, size_t);

static const struct mlk_notify_system default_system = {
	.draw = draw
};
static const struct mlk_notify_system *system = &default_system;
static struct mlk_notify stack[MLK_NOTIFY_MAX];
static size_t stacksz;

static void
geometry(struct geo *geo, const struct mlk_notify *n, size_t index)
{
	int x, y;

	/* Determine theme. */
	geo->theme = system->theme ? system->theme : mlk_theme_default();

	/* Determine notification position. */
	x  = mlk_window.w - geo->theme->padding;
	x -= WIDTH;

	y  = geo->theme->padding * (index + 1);;
	y += HEIGHT * index;

	/* Content frame. */
	geo->frame_x = x;
	geo->frame_y = y;
	geo->frame_w = WIDTH;
	geo->frame_h = HEIGHT;

	/* Align icon at the left center. */
	if (n->icon->h >= HEIGHT) {
		mlk_tracef("notification icon is too large: %u > %u", n->icon->h, HEIGHT);
		geo->icon_x = x + geo->theme->padding;
		geo->icon_y = y + geo->theme->padding;
	} else {
		mlk_align(MLK_ALIGN_LEFT, &geo->icon_x, &geo->icon_y, n->icon->w, n->icon->h, x, y, WIDTH, HEIGHT);
		geo->icon_x += geo->icon_y - y;
	}

	/* Align title to the right of the icon at the same y coordinate. */
	geo->title_x  = geo->icon_x + n->icon->w + geo->theme->padding;
	geo->title_y  = geo->icon_y;
	geo->title_y -= mlk_font_height(geo->theme->fonts[MLK_THEME_FONT_IMPORTANT]) / 2;

	/* Align body so it ends at the end of the icon. */
	geo->body_x  = geo->title_x;
	geo->body_y  = geo->icon_y + n->icon->h;
	geo->body_y -= mlk_font_height(geo->theme->fonts[MLK_THEME_FONT_INTERFACE]) / 2;
}

static void
draw_frame(const struct geo *geo)
{
	const struct mlk_frame f = {
		.x = geo->frame_x,
		.y = geo->frame_y,
		.w = geo->frame_w,
		.h = geo->frame_h
	};

	mlk_frame_draw(&f);
}

static void
draw_icon(const struct geo *geo, const struct mlk_notify *n)
{
	mlk_texture_draw(n->icon, geo->icon_x, geo->icon_y);
}

#include <stdio.h>

static void
draw_title(const struct geo *geo, const struct mlk_notify *n)
{
	const struct mlk_label l = {
		.x = geo->title_x,
		.y = geo->title_y,
		.text = n->title,
		.flags = MLK_LABEL_FLAGS_SHADOW | MLK_LABEL_FLAGS_IMPORTANT
	};

	mlk_label_draw(&l);
}

static void
draw_body(const struct geo *geo, const struct mlk_notify *n)
{
	const struct mlk_label l = {
		.x = geo->body_x,
		.y = geo->body_y,
		.text = n->body,
		.flags = MLK_LABEL_FLAGS_SHADOW
	};

	mlk_label_draw(&l);
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

void
mlk_notify(const struct mlk_texture *icon, const char *title, const char *body)
{
	assert(icon);
	assert(title);
	assert(body);

	struct mlk_notify *n;

	if (stacksz >= MLK_NOTIFY_MAX) {
		memmove(&stack[0], &stack[1], sizeof (stack[0]) - MLK_NOTIFY_MAX - 1);
		n = &stack[MLK_NOTIFY_MAX - 1];
	} else
		n = &stack[stacksz++];

	memset(n, 0, sizeof (*n));
	n->icon = icon;
	n->title = title;
	n->body = body;
}

void
mlk_notify_update(unsigned int ticks)
{
	struct mlk_notify *n;

	for (size_t i = 0; i < stacksz; ++i) {
		n = &stack[i];
		n->elapsed += ticks;

		if (n->elapsed >= MLK_NOTIFY_TIMEOUT_DEFAULT)
			memmove(n, n + 1, sizeof (*n) * (--stacksz - i));
	}
}

void
mlk_notify_draw(void)
{
	for (size_t i = 0; i < stacksz; ++i)
		system->draw(&stack[i], i);
}

void
mlk_notify_set_system(const struct mlk_notify_system *sys)
{
	system = sys ? sys : &default_system;
}
