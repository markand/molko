/*
 * theme.h -- abstract theming
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

#ifndef MLK_UI_THEME_H
#define MLK_UI_THEME_H

#include <mlk/core/core.h>

struct mlk_button;
struct mlk_checkbox;
struct mlk_font;
struct frame;
struct label;
struct mlk_sprite;

enum theme_font {
	THEME_FONT_DEBUG,
	THEME_FONT_INTERFACE,
	THEME_FONT_IMPORTANT,
	THEME_FONT_NUM
};

enum theme_color {
	THEME_COLOR_DEBUG,
	THEME_COLOR_NORMAL,
	THEME_COLOR_SELECTED,
	THEME_COLOR_SHADOW,
	THEME_COLOR_NUM
};

enum theme_sprite {
	THEME_SPRITE_CURSOR,
	THEME_SPRITE_NUM
};

struct theme {
	struct mlk_font *fonts[THEME_FONT_NUM];
	const struct mlk_sprite *sprites[THEME_SPRITE_NUM];
	unsigned long colors[THEME_COLOR_NUM];
	unsigned int padding;

	void (*draw_frame)(const struct theme *, const struct frame *);
	void (*draw_label)(const struct theme *, const struct label *);
	void (*draw_button)(const struct theme *, const struct mlk_button *);
	void (*draw_checkbox)(const struct theme *t, const struct mlk_checkbox *);
};

MLK_CORE_BEGIN_DECLS

int
theme_init(void);

struct theme *
theme_default(void);

void
theme_shallow(struct theme *, const struct theme *);

void
theme_draw_frame(const struct theme *, const struct frame *);

void
theme_draw_label(const struct theme *, const struct label *);

void
theme_draw_button(const struct theme *, const struct mlk_button *);

void
theme_draw_checkbox(const struct theme *, const struct mlk_checkbox *);

void
theme_finish(void);

MLK_CORE_END_DECLS

#endif /* !MLK_UI_THEME_H */
