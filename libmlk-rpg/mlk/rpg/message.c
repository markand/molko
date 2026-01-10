/*
 * message.c -- message dialog
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
#include <stdlib.h>
#include <string.h>

#include <mlk/core/err.h>
#include <mlk/core/event.h>
#include <mlk/core/font.h>
#include <mlk/core/maths.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/sprite.h>
#include <mlk/core/texture.h>
#include <mlk/core/trace.h>
#include <mlk/core/util.h>

#include <mlk/ui/align.h>
#include <mlk/ui/ui.h>
#include <mlk/ui/ui_p.h>

#include "message.h"

static inline int
is_selectable(const struct mlk_message *msg, size_t n)
{
	return msg->lines[n] && ((msg->selectable >> n) & 0x1) == 1;
}

static inline struct mlk_message_style *
get_style(struct mlk_message *msg)
{
	return msg->style ? msg->style : mlk_message_style;
}

static inline struct mlk_font *
get_font(struct mlk_message *msg)
{
	return MLK__STYLE_FONT(get_style(msg)->font, MLK_UI_FONT_INTERFACE);
}

static unsigned int
min_width(struct mlk_message_style *style, struct mlk_message *msg)
{
	struct mlk_font *font;
	unsigned int maxw = 0, w = 0;
	int err;

	font = MLK__STYLE_FONT(style->font, MLK_UI_FONT_INTERFACE);

	for (size_t i = 0; i < msg->linesz; ++i) {
		if (!msg->lines[i])
			continue;
		if ((err = mlk_font_query(font, msg->lines[i], &w, NULL)) < 0)
			return err;
		if (w > maxw)
			maxw = w;
	}

	return (style->padding * 2) + maxw;
}

static unsigned int
min_height(struct mlk_message_style *style, struct mlk_message *msg)
{
	assert(msg);

	struct mlk_font *font;
	unsigned int lh;

	font = MLK__STYLE_FONT(style->font, MLK_UI_FONT_INTERFACE);
	lh = mlk_font_height(font);

	return (style->padding * 2) + (msg->linesz * lh) + ((msg->linesz - 1) * style->padding);
}

static void
draw_frame(struct mlk_message *msg)
{
	struct mlk_message_style *style = get_style(msg);

	mlk_painter_set_color(style->border);
	mlk_painter_draw_rectangle(0, 0, msg->w, msg->h);
	mlk_painter_set_color(style->background);
	mlk_painter_draw_rectangle(
		style->border_size,
		style->border_size,
		msg->w - (style->border_size * 2),
		msg->h - (style->border_size * 2)
	);
}

static void
draw_lines(struct mlk_message *msg)
{
	struct mlk_message_style *style;
	struct mlk_font *font;
	struct mlk_texture texture;
	unsigned long color;
	int x, y;

	style = get_style(msg);
	font = get_font(msg);

	for (size_t i = 0; i < msg->linesz; ++i) {
		if (!msg->lines[i])
			continue;

		if (msg->selectable && msg->selected == i && is_selectable(msg, i))
			color = style->color_selected;
		else
			color = style->color;

		if (mlk_font_render(font, &texture, msg->lines[i], color) < 0) {
			mlk_tracef("unable to render message text", mlk_err());
			continue;
		}

		x = style->padding;
		y = style->padding + (i * (texture.h + style->padding));

		if (x + texture.w > msg->w)
			mlk_tracef("message width too small: %u < %u", msg->w, min_width(style, msg));
		if (y + texture.h > msg->h)
			mlk_tracef("message height too small: %u < %u", msg->h, min_height(style, msg));

		mlk_texture_draw(&texture, x, y);
		mlk_texture_finish(&texture);
	}
}

static int
query(struct mlk_message_style *self,
      struct mlk_message *msg,
      unsigned int *w,
      unsigned int *h)
{
	if (w)
		*w = min_width(self, msg);
	if (h)
		*h = min_height(self, msg);

	return 0;
}

static void
update(struct mlk_message_style *self,
       struct mlk_message *msg,
       unsigned int ticks)
{
	(void)self;

	const struct mlk_message_style *style = MLK__STYLE(msg, mlk_message_style);

	msg->elapsed += ticks;

	switch (msg->state) {
	case MLK_MESSAGE_STATE_OPENING:
		msg->scale = (double)msg->elapsed / (double)style->speed;

		if (msg->scale > 1)
			msg->scale = 1;

		if (msg->elapsed >= style->speed) {
			msg->state = MLK_MESSAGE_STATE_SHOWING;
			msg->elapsed = 0;
		}

		break;
	case MLK_MESSAGE_STATE_SHOWING:
		/* Do automatically switch state if requested by the user. */
		if (msg->flags & MLK_MESSAGE_FLAGS_AUTOMATIC && msg->elapsed >= style->timeout) {
			msg->state = msg->flags & MLK_MESSAGE_FLAGS_FADEOUT
			    ? MLK_MESSAGE_STATE_HIDING
			    : MLK_MESSAGE_STATE_NONE;
			msg->elapsed = 0;
		}

		break;
	case MLK_MESSAGE_STATE_HIDING:
		msg->scale = 1 - (double)msg->elapsed / (double)style->speed;

		if (msg->scale < 0)
			msg->scale = 0;
		if (msg->elapsed >= style->speed) {
			msg->state = MLK_MESSAGE_STATE_NONE;
			msg->elapsed = 0;
		}

		break;
	default:
		break;
	}

}

static void
draw(struct mlk_message_style *self, struct mlk_message *msg)
{
	(void)self;

	struct mlk_texture tex;
	int x, y;
	unsigned int w, h;

	if (msg->w == 0 || msg->h == 0) {
		mlk_tracef("message has null dimensions");
		return;
	}

	if (mlk_texture_init(&tex, msg->w, msg->h) < 0)
		mlk_panic();

	MLK_PAINTER_BEGIN(&tex);
	draw_frame(msg);
	draw_lines(msg);
	MLK_PAINTER_END();

	/* Compute scaling. */
	w = msg->w * msg->scale;
	h = msg->h * msg->scale;

	/* Centerize within its drawing area. */
	mlk_align(MLK_ALIGN_CENTER, &x, &y, w, h, msg->x, msg->y, msg->w, msg->h);

	/* Draw and clear. */
	mlk_texture_scale(&tex, 0, 0, msg->w, msg->h, x, y, w, h, 0);
	mlk_texture_finish(&tex);
}

// TODO: add dark variant.
struct mlk_message_style mlk_message_style_dark = {
	.background     = MLK_UI_COLOR_BG,
	.border         = MLK_UI_COLOR_BORDER,
	.border_size    = MLK_UI_BORDER,
	.color          = MLK_UI_COLOR_TEXT,
	.color_selected = MLK_UI_COLOR_SELECTED,
	.padding        = MLK_UI_PADDING,
	.timeout        = MLK_MESSAGE_TIMEOUT_DEFAULT,
	.speed          = MLK_MESSAGE_SPEED_DEFAULT,
	.query          = query,
	.update         = update,
	.draw           = draw
};

struct mlk_message_style mlk_message_style_light = {
	.background     = MLK_UI_COLOR_BG,
	.border         = MLK_UI_COLOR_BORDER,
	.border_size    = MLK_UI_BORDER,
	.color          = MLK_UI_COLOR_TEXT,
	.color_selected = MLK_UI_COLOR_SELECTED,
	.padding        = MLK_UI_PADDING,
	.timeout        = MLK_MESSAGE_TIMEOUT_DEFAULT,
	.speed          = MLK_MESSAGE_SPEED_DEFAULT,
	.query          = query,
	.update         = update,
	.draw           = draw
};

struct mlk_message_style *mlk_message_style = &mlk_message_style_light;

void
mlk_message_start(struct mlk_message *msg)
{
	assert(msg);

	struct mlk_message_style *style = MLK__STYLE(msg, mlk_message_style);

	if ((msg->flags & (MLK_MESSAGE_FLAGS_FADEIN | MLK_MESSAGE_FLAGS_FADEOUT)) && style->speed == 0)
		mlk_tracef("message has animation but zero delay");

	msg->elapsed = 0;
	msg->scale = msg->flags & MLK_MESSAGE_FLAGS_FADEIN ? 0.0 : 1.0;
	msg->state = msg->flags & MLK_MESSAGE_FLAGS_FADEIN
	    ? MLK_MESSAGE_STATE_OPENING
	    : MLK_MESSAGE_STATE_SHOWING;

	if (msg->flags & MLK_MESSAGE_FLAGS_AUTOMATIC && style->timeout == 0)
		mlk_tracef("message is automatic but has zero timeout");

	/*
	 * Make sure selected index goes in the range of the lines and that it
	 * starts on a proper selectable line.
	 */
	if (msg->selectable) {
		if (msg->selected >= msg->linesz || !is_selectable(msg, msg->selected))
			msg->selected = 0;

		while (!is_selectable(msg, msg->selected))
			msg->selected++;
	}
}

int
mlk_message_query(struct mlk_message *msg, unsigned int *w, unsigned int *h)
{
	assert(msg);

	return MLK__STYLE_CALL(msg->style, mlk_message_style, query, msg, w, h);
}

static inline size_t
first(const struct mlk_message *msg)
{
	size_t ret = -1;

	for (size_t i = 0; i < msg->linesz; ++i) {
		if (is_selectable(msg, i)) {
			ret = i;
			break;
		}
	}

	return ret;
}

static inline size_t
last(const struct mlk_message *msg)
{
	size_t ret = -1;

	for (size_t i = msg->linesz; i >= 0; --i) {
		if (is_selectable(msg, i)) {
			ret = i;
			break;
		}
	}

	return ret;
}

static inline size_t
previous(const struct mlk_message *msg)
{
	size_t ret;

	/* wrap */
	if (msg->selected == first(msg))
		ret = last(msg);
	else {
		ret = msg->selected - 1;

		while (ret > 0 && !is_selectable(msg, ret))
			ret--;
	}

	return ret;
}

static inline size_t
next(const struct mlk_message *msg)
{
	size_t ret;

	/* wrap */
	if (msg->selected == last(msg))
		ret = first(msg);
	else {
		ret = msg->selected + 1;

		while (ret < msg->linesz && !is_selectable(msg, ret))
			ret++;
	}

	return ret;
}

void
mlk_message_handle(struct mlk_message *msg, const union mlk_event *ev)
{
	assert(msg);
	assert(ev);

	/* Skip if the message animation hasn't complete. */
	if (msg->state != MLK_MESSAGE_STATE_SHOWING)
		return;

	/* Only keyboard event are valid. */
	if (ev->type != MLK_EVENT_KEYDOWN || msg->state == MLK_MESSAGE_STATE_NONE)
		return;

	switch (ev->key.key) {
	case MLK_KEY_UP:
		if (msg->selectable && msg->linesz)
			msg->selected = previous(msg);
		break;
	case MLK_KEY_DOWN:
		if (msg->selectable && msg->linesz)
			msg->selected = next(msg);
		break;
	case MLK_KEY_ENTER:
		msg->state = msg->flags & MLK_MESSAGE_FLAGS_FADEOUT
		    ? MLK_MESSAGE_STATE_HIDING
		    : MLK_MESSAGE_STATE_NONE;
		msg->elapsed = 0;
		break;
	default:
		break;
	}
}

int
mlk_message_update(struct mlk_message *msg, unsigned int ticks)
{
	assert(msg);

	MLK__STYLE_CALL(msg->style, mlk_message_style, update, msg, ticks);

	return msg->state == MLK_MESSAGE_STATE_NONE;
}

int
mlk_message_complete(const struct mlk_message *msg)
{
	assert(msg);

	return msg->state == MLK_MESSAGE_STATE_NONE;
}

void
mlk_message_draw(struct mlk_message *msg)
{
	assert(msg);

	MLK__STYLE_CALL(msg->style, mlk_message_style, draw, msg);
}

void
mlk_message_hide(struct mlk_message *msg)
{
	assert(msg);

	msg->state = MLK_MESSAGE_STATE_HIDING;
	msg->elapsed = 0;
}
