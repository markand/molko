/*
 * save.c -- select a save slot
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
#include <stdio.h>
#include <string.h>

#include <compat.h>

#include <core/event.h>
#include <core/font.h>
#include <core/maths.h>
#include <core/painter.h>
#include <core/sprite.h>
#include <core/window.h>

#include <ui/align.h>
#include <ui/label.h>
#include <ui/frame.h>
#include <ui/theme.h>

#include <adventure/adventure_p.h>
#include <adventure/assets.h>

#include "save.h"

#define THEME(s)        ((s)->theme ? (s)->theme : theme_default())
#define LINES_MAX       (3)

/* TODO: require a module for this*/
#define TEAM_MAX        (4)

struct geo {
	int x, y;
	unsigned int w, h;

	/* Per line height/padding. */
	unsigned int lh;
	unsigned int lp;

	struct {
		int x, y;
		unsigned int w, h;

		/* Lines start positions. */
		int lx, ly;

		struct {
			int x, y;
			unsigned int w, h;
		} faces[TEAM_MAX];
	} saves[DIALOG_SAVE_MAX];
};

static struct geo
geometry(const struct dialog_save *dlg)
{
	struct geo geo = {
		.w = window.w - THEME(dlg)->padding * 40,
		.h = window.h - THEME(dlg)->padding * 10,
	};

	align(ALIGN_CENTER, &geo.x, &geo.y, geo.w, geo.h, 0, 0, window.w, window.h);

	for (size_t i = 0; i < DIALOG_SAVE_MAX; ++i) {
		const unsigned int padding = THEME(dlg)->padding;

		geo.saves[i].w  = (geo.w -  padding * 2);
		geo.saves[i].h  = (geo.h - (padding * (DIALOG_SAVE_MAX + 1))) / DIALOG_SAVE_MAX;
		geo.saves[i].x  = (geo.x +  padding);
		geo.saves[i].y  = (geo.y +  padding) + i * (padding + geo.saves[i].h);
		geo.saves[i].lx = (geo.saves[i].x + padding);

		/* Compute lines padding (we draw LINES_MAX lines). */
		geo.lh = font_height(THEME(dlg)->fonts[THEME_FONT_INTERFACE]);
		geo.lp = (geo.saves[i].h - (geo.lh * LINES_MAX)) / (LINES_MAX + 1);

		/* Compute faces position. */
		for (size_t f = 0; f < TEAM_MAX; ++f) {
			geo.saves[i].faces[f].h = geo.saves[i].h - padding * 2;
			geo.saves[i].faces[f].w = geo.saves[i].faces[f].h;
			geo.saves[i].faces[f].x = (geo.saves[i].x + padding) + (f * (padding + geo.saves[i].faces[f].w));
			geo.saves[i].faces[f].y = (geo.saves[i].y + padding);
			geo.saves[i].lx += geo.saves[i].faces[f].w + padding;
		}

		geo.saves[i].ly = geo.saves[i].y + geo.lp;
	}

	return geo;
}

static void
draw_frame(const struct geo *geo)
{
	struct frame f = {
		.x = geo->x,
		.y = geo->y,
		.w = geo->w,
		.h = geo->h,
	};

	frame_draw(&f);
}

static void
draw_save_box(const struct geo *geo, size_t i)
{
	/* TODO: change colors at some point. */
	painter_set_color(0x884b2bff);
	painter_draw_rectangle(geo->saves[i].x, geo->saves[i].y, geo->saves[i].w, geo->saves[i].h);
}

static void
draw_save_faces(const struct dialog_save *dlg, const struct geo *geo, size_t i)
{
	/* TODO: determine face. */
	for (size_t f = 0; f < TEAM_MAX; ++f) {
		sprite_scale(assets_sprites[ASSETS_SPRITE_FACES], 0, f,
		    geo->saves[i].faces[f].x,
		    geo->saves[i].faces[f].y,
		    geo->saves[i].faces[f].w,
		    geo->saves[i].faces[f].h
		);
	}
}

static void
draw_save_times(const struct dialog_save *dlg, const struct geo *geo, size_t i)
{
	struct label label = {0};
	char time[128], line[256];
	struct save_property prop;

	label.theme = dlg->theme;
	label.x = geo->saves[i].lx;
	label.y = geo->saves[i].ly;
	label.flags = LABEL_FLAGS_SHADOW;
	label.text = line;

	/* TODO: Get map position. */
	strlcpy(line, "World", sizeof (line));
	label_draw(&label);
	
	/* Last time. */
	strftime(time, sizeof (time), "%c", localtime(&dlg->saves[i].updated));
	snprintf(line, sizeof (line), _("Last played: %s"), time);

	label.y += geo->lp + geo->lh;
	label_draw(&label);

	/* TODO: Time played. */
	snprintf(line, sizeof (line), _("Time played: %s"), "100 hours");

	label.y += geo->lp + geo->lh;
	label_draw(&label);
}

static void
draw_save(const struct dialog_save *dlg, const struct geo *geo, size_t i)
{
	draw_save_box(geo, i);

	/* Do not draw the content if save is invalid. */
	if (!save_ok(&dlg->saves[i]))
		return;

	draw_save_faces(dlg, geo, i);
	draw_save_times(dlg, geo, i);
}

static void
draw_saves(const struct dialog_save *dlg, const struct geo *geo)
{
	for (size_t i = 0; i < DIALOG_SAVE_MAX; ++i)
		draw_save(dlg, geo, i);
}

static void
draw_cursor(const struct dialog_save *dlg, const struct geo *geo)
{
	const struct sprite *sprite =assets_sprites[ASSETS_SPRITE_UI_CURSOR];
	const int x = geo->saves[dlg->selected].x - sprite->cellw;
	const int y = geo->saves[dlg->selected].y;

	sprite_draw(sprite, 1, 2, x, y + (geo->saves[dlg->selected].h / 2) - (sprite->cellh / 2));
}

static bool
handle_keydown(struct dialog_save *s, const struct event_key *key)
{
	assert(key->type == EVENT_KEYDOWN);

	switch (key->key) {
	case KEY_UP:
		if (s->selected == 0)
			s->selected = DIALOG_SAVE_MAX - 1;
		else
			s->selected --;
		break;
	case KEY_DOWN:
		if (s->selected + 1 >= DIALOG_SAVE_MAX)
			s->selected = 0;
		else
			s->selected ++;
		break;
	case KEY_ENTER:
		return save_ok(&s->saves[s->selected]);
	default:
		break;
	}

	return false;
}

static bool
handle_clickdown(struct dialog_save *s, const struct geo *geo, const struct event_click *clk)
{
	assert(clk->type == EVENT_CLICKDOWN);

	for (size_t i = 0; i < DIALOG_SAVE_MAX; ++i) {
		if (maths_is_boxed(geo->saves[i].x, geo->saves[i].y,
		                   geo->saves[i].w, geo->saves[i].h,
		                   clk->x, clk->y)) {
			s->selected = i;
			break;
		}
	}

	return clk->clicks >= 2 && save_ok(&s->saves[s->selected]);
}

void
dialog_save_init(struct dialog_save *s)
{
	assert(s);

	for (size_t i = 0; i < DIALOG_SAVE_MAX; ++i)
		save_open(&s->saves[i], i, SAVE_MODE_READ);
}

bool
dialog_save_handle(struct dialog_save *dlg, const union event *ev)
{
	assert(dlg);
	assert(ev);

	const struct geo geo = geometry(dlg);

	switch (ev->type) {
	case EVENT_KEYDOWN:
		return handle_keydown(dlg, &ev->key);
	case EVENT_CLICKDOWN:
		return handle_clickdown(dlg, &geo, &ev->click);
	default:
		break;
	}

	return false;
}

void
dialog_save_update(struct dialog_save *dlg, unsigned int ticks)
{
	assert(dlg);
}

void
dialog_save_draw(const struct dialog_save *dlg)
{
	assert(dlg);

	const struct geo geo = geometry(dlg);

	draw_frame(&geo);
	draw_saves(dlg, &geo);
	draw_cursor(dlg, &geo);
}

void
dialog_save_finish(struct dialog_save *dlg)
{
	assert(dlg);

	for (size_t i = 0; i < DIALOG_SAVE_MAX; ++i)
		save_finish(&dlg->saves[i]);
}
