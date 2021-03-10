/*
 * label.c -- GUI label
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

#include <core/action.h>
#include <core/font.h>
#include <core/panic.h>
#include <core/texture.h>

#include "label.h"
#include "theme.h"

static void
draw(struct action *act)
{
	label_draw(act->data);
}

void
label_draw_default(const struct theme *t, const struct label *label)
{
	assert(t);
	assert(label);

	struct font *font;
	struct texture tex;

	font = t->fonts[THEME_FONT_INTERFACE];

	/* Shadow text, only if enabled. */
	if (label->flags & LABEL_FLAGS_SHADOW) {
		if (font_render(font, &tex, label->text, t->colors[THEME_COLOR_SHADOW]) < 0)
			panic();

		texture_draw(&tex, label->x + 1, label->y + 1);
		texture_finish(&tex);
	}

	/* Normal text. */
	if (font_render(font, &tex, label->text, t->colors[THEME_COLOR_NORMAL]) < 0)
		panic();

	texture_draw(&tex, label->x, label->y);
	texture_finish(&tex);
}

int
label_ok(const struct label *label)
{
	return label && label->text && strlen(label->text) > 0;
}

void
label_query(const struct label *label, unsigned int *w, unsigned int *h)
{
	assert(label);
	assert(label->text);

	const struct theme *t = label->theme ? label->theme : theme_default();

	if (font_query(t->fonts[THEME_FONT_INTERFACE], label->text, w, h) < 0)
		panic();
}

void
label_draw(const struct label *label)
{
	assert(label);
	assert(label->text);

	theme_draw_label(label->theme, label);
}

void
label_action(struct label *label, struct action *act)
{
	assert(label);
	assert(act);

	memset(act, 0, sizeof (*act));
	act->data = label;
	act->draw = draw;
}
