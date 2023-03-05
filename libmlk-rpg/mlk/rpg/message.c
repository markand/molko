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

#include <mlk/core/event.h>
#include <mlk/core/font.h>
#include <mlk/core/maths.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/sprite.h>
#include <mlk/core/trace.h>
#include <mlk/core/util.h>

#include <mlk/ui/align.h>
#include <mlk/ui/ui.h>
#include <mlk/ui/ui_p.h>

#include "message.h"

static inline struct mlk_font *
style_font(const struct mlk_message *message)
{
	const struct mlk_message_style *style = MLK__STYLE(message, mlk_message_style);

	if (style->text_font)
		return style->text_font;

	return mlk_ui_fonts[MLK_UI_FONT_INTERFACE];
}

static unsigned int
min_width(const struct mlk_message *msg)
{
	assert(msg);

	const struct mlk_message_style *style = MLK__STYLE(msg, mlk_message_style);
	struct mlk_font *font;
	unsigned int maxw = 0, w = 0;
	int err;

	font = style_font(msg);

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
min_height(const struct mlk_message *msg)
{
	assert(msg);

	const struct mlk_message_style *style = MLK__STYLE(msg, mlk_message_style);
	struct mlk_font *font;
	unsigned int lh;

	font = style_font(msg);
	lh = mlk_font_height(font);

	return (style->padding * 2) + (msg->linesz * lh) + ((msg->linesz - 1) * style->padding);
}

static void
draw_frame(const struct mlk_message *msg)
{
	const struct mlk_message_style *style = MLK__STYLE(msg, mlk_message_style);

	mlk_painter_set_color(style->border_color);
	mlk_painter_draw_rectangle(0, 0, msg->w, msg->h);
	mlk_painter_set_color(style->bg_color);
	mlk_painter_draw_rectangle(
		style->border_size,
		style->border_size,
		msg->w - (style->border_size * 2),
		msg->h - (style->border_size * 2)
	);
}

static void
draw_lines(const struct mlk_message *msg)
{
	const struct mlk_message_style *style;
	struct mlk_font *font;
	struct mlk_texture texture;
	unsigned long color;
	int err, x, y;

	style = MLK__STYLE(msg, mlk_message_style);
	font = style_font(msg);

	for (size_t i = 0; i < msg->linesz; ++i) {
		if (!msg->lines[i])
			continue;

		if ((msg->flags & MLK_MESSAGE_FLAGS_QUESTION) && msg->index == (unsigned int)i)
			color = style->selected_color;
		else
			color = style->text_color;

		if ((err = mlk_font_render(font, &texture, msg->lines[i], color)) < 0) {
			mlk_tracef("%s", mlk_err_string(err));
			continue;
		}

		x = style->padding;
		y = style->padding + (i * (texture.h + style->padding));

		if (x + texture.w > msg->w)
			mlk_tracef("message width too small: %u < %u", msg->w, min_width(msg));
		if (y + texture.h > msg->h)
			mlk_tracef("message height too small: %u < %u", msg->h, min_height(msg));

		mlk_texture_draw(&texture, x, y);
		mlk_texture_finish(&texture);
	}
}

static int
delegate_query(struct mlk_message_delegate *self,
               const struct mlk_message *msg,
               unsigned int *w,
               unsigned int *h)
{
	(void)self;

	if (w)
		*w = min_width(msg);
	if (h)
		*h = min_height(msg);
}

static void
delegate_update(struct mlk_message_delegate *self,
                struct mlk_message *msg,
                unsigned int ticks)
{
	(void)self;

	const struct mlk_message_style *style = MLK__STYLE(msg, mlk_message_style);

	msg->elapsed += ticks;

	switch (msg->state) {
	case MLK_MESSAGE_STATE_OPENING:
		msg->scale = (double)msg->elapsed / (double)style->delay;

		if (msg->scale > 1)
			msg->scale = 1;

		if (msg->elapsed >= style->delay) {
			msg->state = MLK_MESSAGE_STATE_SHOWING;
			msg->elapsed = 0;
		}

		break;
	case MLK_MESSAGE_STATE_SHOWING:
		/* Do automatically switch state if requested by the user. */
		if (msg->flags & MLK_MESSAGE_FLAGS_AUTOMATIC && msg->elapsed >= style->duration) {
			msg->state = msg->flags & MLK_MESSAGE_FLAGS_FADEOUT
			    ? MLK_MESSAGE_STATE_HIDING
			    : MLK_MESSAGE_STATE_NONE;
			msg->elapsed = 0;
		}

		break;
	case MLK_MESSAGE_STATE_HIDING:
		msg->scale = 1 - (double)msg->elapsed / (double)style->delay;

		if (msg->scale < 0)
			msg->scale = 0;
		if (msg->elapsed >= style->delay) {
			msg->state = MLK_MESSAGE_STATE_NONE;
			msg->elapsed = 0;
		}

		break;
	default:
		break;
	}

}

static void
delegate_draw(struct mlk_message_delegate *self, const struct mlk_message *msg)
{
	(void)self;

	struct mlk_texture tex;
	int x, y, err;
	unsigned int w, h;

	if (msg->w == 0 || msg->h == 0) {
		mlk_tracef("message has null dimensions");
		return;
	}

	if ((err = mlk_texture_new(&tex, msg->w, msg->h)) < 0)
		mlk_panic(err);

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

struct mlk_message_style mlk_message_style = {
	.padding        = MLK_UI_PADDING,
	.delay          = MLK_MESSAGE_DELAY_DEFAULT,
	.duration       = MLK_MESSAGE_DURATION_DEFAULT,
	.bg_color       = MLK_UI_COLOR_BG,
	.border_color   = MLK_UI_COLOR_BORDER,
	.border_size    = MLK_UI_BORDER,
	.text_color     = MLK_UI_COLOR_TEXT,
	.selected_color = MLK_UI_COLOR_SELECTED
};
struct mlk_message_delegate mlk_message_delegate = {
	.query          = delegate_query,
	.update         = delegate_update,
	.draw           = delegate_draw
};

void
mlk_message_start(struct mlk_message *msg)
{
	assert(msg);

	const struct mlk_message_style *style = MLK__STYLE(msg, mlk_message_style);

	if ((msg->flags & (MLK_MESSAGE_FLAGS_FADEIN | MLK_MESSAGE_FLAGS_FADEOUT)) && style->delay == 0)
		mlk_tracef("message has animation but zero delay");

	msg->elapsed = 0;
	msg->scale = msg->flags & MLK_MESSAGE_FLAGS_FADEIN ? 0.0 : 1.0;
	msg->state = msg->flags & MLK_MESSAGE_FLAGS_FADEIN
	    ? MLK_MESSAGE_STATE_OPENING
	    : MLK_MESSAGE_STATE_SHOWING;

	if (msg->flags & MLK_MESSAGE_FLAGS_AUTOMATIC && style->duration == 0)
		mlk_tracef("message is automatic but has zero duration");
}

int
mlk_message_query(const struct mlk_message *msg, unsigned int *w, unsigned int *h)
{
	assert(msg);

	MLK__DELEGATE_INVOKE_RET(msg->delegate, mlk_message_delegate, query, msg, w, h);

	if (w)
		*w = 0;
	if (h)
		*h = 0;

	return MLK_ERR_NO_SUPPORT;
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
		if (msg->index > 0)
			msg->index--;
		break;
	case MLK_KEY_DOWN:
		if (msg->index + 1 < msg->linesz && msg->lines[msg->index + 1])
			msg->index++;
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

	MLK__DELEGATE_INVOKE(msg->delegate, mlk_message_delegate, update, msg, ticks);

	return msg->state == MLK_MESSAGE_STATE_NONE;
}

void
mlk_message_draw(const struct mlk_message *msg)
{
	assert(msg);

	MLK__DELEGATE_INVOKE(msg->delegate, mlk_message_delegate, draw, msg);
}

void
mlk_message_hide(struct mlk_message *msg)
{
	assert(msg);

	msg->state = MLK_MESSAGE_STATE_HIDING;
	msg->elapsed = 0;
}
