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
struct mlk_frame;
struct mlk_label;
struct mlk_sprite;

enum mlk_theme_font {
	MLK_THEME_FONT_DEBUG,
	MLK_THEME_FONT_INTERFACE,
	MLK_THEME_FONT_IMPORTANT,
	MLK_THEME_FONT_NUM
};

enum mlk_theme_color {
	MLK_THEME_COLOR_DEBUG,
	MLK_THEME_COLOR_NORMAL,
	MLK_THEME_COLOR_SELECTED,
	MLK_THEME_COLOR_SHADOW,
	MLK_THEME_COLOR_NUM
};

enum mlk_theme_sprite {
	MLK_THEME_SPRITE_CURSOR,
	MLK_THEME_SPRITE_NUM
};

struct mlk_theme {
	struct mlk_font *fonts[MLK_THEME_FONT_NUM];
	const struct mlk_sprite *sprites[MLK_THEME_SPRITE_NUM];
	unsigned long colors[MLK_THEME_COLOR_NUM];
	unsigned int padding;

	void (*draw_frame)(const struct mlk_theme *, const struct mlk_frame *);
	void (*draw_label)(const struct mlk_theme *, const struct mlk_label *);
	void (*draw_button)(const struct mlk_theme *, const struct mlk_button *);
	void (*draw_checkbox)(const struct mlk_theme *t, const struct mlk_checkbox *);
};

MLK_CORE_BEGIN_DECLS

int
mlk_theme_init(void);

struct mlk_theme *
mlk_theme_default(void);

void
mlk_theme_shallow(struct mlk_theme *, const struct mlk_theme *);

void
mlk_theme_draw_frame(const struct mlk_theme *, const struct mlk_frame *);

void
mlk_theme_draw_label(const struct mlk_theme *, const struct mlk_label *);

void
mlk_theme_draw_button(const struct mlk_theme *, const struct mlk_button *);

void
mlk_theme_draw_checkbox(const struct mlk_theme *, const struct mlk_checkbox *);

void
mlk_theme_finish(void);

MLK_CORE_END_DECLS

#endif /* !MLK_UI_THEME_H */
