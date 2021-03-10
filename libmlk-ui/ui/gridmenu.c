/*
 * gridmenu.c -- GUI grid menu
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
#include <string.h>

#include <core/event.h>
#include <core/maths.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/texture.h>
#include <core/trace.h>

#include "ui_p.h"
#include "frame.h"
#include "label.h"
#include "gridmenu.h"
#include "theme.h"

#define THEME(m) ((m)->theme ? (m)->theme : theme_default())

struct index {
	unsigned int row;
	unsigned int col;
};

static struct index
get_index(const struct gridmenu *menu)
{
	return (struct index) {
		.row = menu->selected / menu->ncols,
		.col = menu->selected % menu->ncols
	};
}

static void
positionate(struct gridmenu *menu)
{
	const struct theme *theme = THEME(menu);
	struct gridmenu_texture *gtex = &menu->tex;
	unsigned int vreq, hreq;

	/* Compute which item has the bigger width to create a spacing. */
	for (size_t i = 0; i < GRIDMENU_ENTRY_MAX; ++i) {
		unsigned int lw, lh;

		gtex->labels[i].theme = theme;
		gtex->labels[i].flags = LABEL_FLAGS_SHADOW;

		if (!(gtex->labels[i].text = menu->menu[i]))
			continue;

		label_query(&gtex->labels[i], &lw, &lh);
		gtex->eltw = lw > gtex->eltw ? lw : gtex->eltw;
		gtex->elth = lh > gtex->elth ? lh : gtex->elth;
	}

	vreq = (theme->padding * 2) + (gtex->elth * menu->nrows);
	hreq = (theme->padding * 2) + (gtex->eltw * menu->ncols);

	/* Compute spacing between elements. */
	if (hreq > menu->w) {
		tracef(_("gridmenu width is too small: %u < %u"), menu->w, vreq);
		gtex->spaceh = 1;
	} else {
		hreq -= theme->padding * 2;
		gtex->spaceh = (menu->w - hreq) / menu->ncols;
	}

	if (vreq > menu->h) {
		tracef(_("gridmenu height is too small: %u < %u"), menu->h, vreq);
		gtex->spacev = 1;
	} else {
		vreq -= theme->padding * 2;
		gtex->spacev = (menu->h - vreq) / menu->nrows;
	}

	/* This is the whole height. */
	gtex->relh  = theme->padding * 2;
	gtex->relh += gtex->elth * (GRIDMENU_ENTRY_MAX / menu->ncols);
	gtex->relh += gtex->spacev * (GRIDMENU_ENTRY_MAX / menu->ncols);
}

static void
repaint_frame(struct gridmenu *menu)
{
	const struct frame f = {
		.x = 0,
		.y = menu->tex.rely,
		.w = menu->w,
		.h = menu->h,
		.theme = menu->theme,
	};

	frame_draw(&f);
}

static void
repaint_labels(struct gridmenu *menu)
{
	struct theme theme;
	unsigned int r = 0, c = 0;

	/* Copy theme to change color if selected. */
	theme_shallow(&theme, THEME(menu));

	for (size_t i = 0; i < GRIDMENU_ENTRY_MAX; ++i) {
		struct label *l = &menu->tex.labels[i];

		if (i == menu->selected)
			theme.colors[THEME_COLOR_NORMAL] = THEME(menu)->colors[THEME_COLOR_SELECTED];
		else
			theme.colors[THEME_COLOR_NORMAL] = THEME(menu)->colors[THEME_COLOR_NORMAL];

		l->theme = &theme;
		l->x = theme.padding + (c * menu->tex.eltw) + (c * menu->tex.spaceh);
		l->y = theme.padding + (r * menu->tex.elth) + (r * menu->tex.spacev);

		if (++c >= menu->ncols) {
			++r;
			c = 0;
		}

		if (l->text)
			label_draw(l);
	}
}

static void
repaint(struct gridmenu *menu)
{
	struct texture *tex = &menu->tex.texture;

	if (!texture_ok(tex) && texture_new(tex, menu->w, menu->tex.relh) < 0)
		panic();

	PAINTER_BEGIN(tex);

	painter_clear();
	repaint_frame(menu);
	repaint_labels(menu);

	PAINTER_END();
}

static void
zoom(struct gridmenu *menu)
{
	struct gridmenu_texture *tex = &menu->tex;
	struct label *cur = &tex->labels[menu->selected];

	/* Readjust relative position. */
	if ((unsigned int)cur->y > tex->rely + menu->h || cur->y < tex->rely)
		tex->rely = cur->y - THEME(menu)->padding;
}

static void
handle_keydown(struct gridmenu *menu, const struct event_key *key)
{
	assert(key->type == EVENT_KEYDOWN);

	const struct index idx = get_index(menu);
	const unsigned int save = menu->selected;

	switch (key->key) {
	case KEY_UP:
		if (idx.row > 0)
			menu->selected -= menu->ncols;
		break;
	case KEY_RIGHT:
		if (idx.col + 1U < menu->ncols)
			menu->selected += 1;
		break;
	case KEY_DOWN:
		if (idx.row + 1U < GRIDMENU_ENTRY_MAX / menu->ncols)
			menu->selected += menu->ncols;
		break;
	case KEY_LEFT:
		if (idx.col > 0)
			menu->selected -= 1;
		break;
	case KEY_ENTER:
		menu->state = GRIDMENU_STATE_ACTIVATED;
		break;
	default:
		break;
	}

	if (save != menu->selected)
		gridmenu_repaint(menu);
}

static void
handle_clickdown(struct gridmenu *menu, const struct event_click *click)
{
	assert(click->type == EVENT_CLICKDOWN);

	const unsigned int save = menu->selected;

	for (size_t i = 0; i < GRIDMENU_ENTRY_MAX; ++i) {
		const struct label *l = &menu->tex.labels[i];
		const int x = menu->x + l->x;
		const int y = menu->y + l->y - menu->tex.rely;
		const unsigned int w = menu->tex.eltw;
		const unsigned int h = menu->tex.elth;

		if (maths_is_boxed(x, y, w, h, click->x, click->y)) {
			menu->selected = i;
			break;
		}
	}

	/* A click immediately active the widget. */
	if (save != menu->selected) {
		gridmenu_repaint(menu);
		menu->state = GRIDMENU_STATE_ACTIVATED;
	}
}

void
gridmenu_reset(struct gridmenu *menu)
{
	assert(menu);

	menu->state = GRIDMENU_STATE_NONE;
}

void
gridmenu_repaint(struct gridmenu *menu)
{
	assert(menu);
	assert(GRIDMENU_ENTRY_MAX % menu->ncols == 0);

	/* Re-compute positions. */
	positionate(menu);

	/* Zoom to the appropriate y-relative. */
	zoom(menu);

	/* Redraw. */
	repaint(menu);
}

void
gridmenu_handle(struct gridmenu *menu, const union event *ev)
{
	assert(menu);
	assert(ev);

	switch (ev->type) {
	case EVENT_KEYDOWN:
		handle_keydown(menu, &ev->key);
		break;
	case EVENT_CLICKDOWN:
		handle_clickdown(menu, &ev->click);
		break;
	default:
		break;
	}
}

void
gridmenu_draw(const struct gridmenu *menu)
{
	assert(menu);
	assert(menu->nrows > 0 && menu->ncols > 0);
	assert(texture_ok(&menu->tex.texture));

	texture_scale(&menu->tex.texture,
	    0, menu->tex.rely, menu->w, menu->h,
	    menu->x, menu->y, menu->w, menu->h, 0.0);
}

void
gridmenu_finish(struct gridmenu *menu)
{
	assert(menu);

	if (texture_ok(&menu->tex.texture))
		texture_finish(&menu->tex.texture);

	memset(menu, 0, sizeof (*menu));
}
