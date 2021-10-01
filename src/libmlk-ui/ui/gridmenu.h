/*
 * gridmenu.h -- GUI grid menu
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

#ifndef MOLKO_UI_GRIDMENU_H
#define MOLKO_UI_GRIDMENU_H

#include <stddef.h>

#include <core/core.h>
#include <core/texture.h>

#include "label.h"

#define GRIDMENU_ENTRY_MAX (256)

struct theme;

union event;

enum gridmenu_state {
	GRIDMENU_STATE_NONE,
	GRIDMENU_STATE_ACTIVATED
};

struct gridmenu_texture {
	int rely;
	unsigned int relh;
	unsigned int eltw;
	unsigned int elth;
	unsigned int spacev;
	unsigned int spaceh;
	struct texture texture;
	struct label labels[GRIDMENU_ENTRY_MAX];
};

struct gridmenu {
	int x;
	int y;
	unsigned int w;
	unsigned int h;
	enum gridmenu_state state;
	size_t selected;
	const struct theme *theme;
	const char *menu[GRIDMENU_ENTRY_MAX];
	unsigned int nrows;
	unsigned int ncols;
	struct gridmenu_texture tex;
};

CORE_BEGIN_DECLS

void
gridmenu_reset(struct gridmenu *);

void
gridmenu_repaint(struct gridmenu *);

void
gridmenu_handle(struct gridmenu *, const union event *);

void
gridmenu_draw(const struct gridmenu *);

void
gridmenu_finish(struct gridmenu *);

CORE_END_DECLS

#endif /* !MOLKO_UI_GRIDMENU_H */
