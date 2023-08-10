/*
 * ui.h -- libui convenient header
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

#ifndef MLK_UI_UI_H
#define MLK_UI_UI_H

#include <mlk/core/core.h>
#include <mlk/core/font.h>

/* TODO: make this a global variable to allow modification of default theme. */
/* https://lospec.com/palette-list/duel */
#define MLK_UI_COLOR_TEXT       (0x222323ff)
#define MLK_UI_COLOR_SELECTED   (0x55b67dff)
#define MLK_UI_COLOR_BG         (0xf5f7faff)
#define MLK_UI_COLOR_BORDER     (0xcdd2daff)
#define MLK_UI_COLOR_DEBUG      (0xe45c5fff)

#define MLK_UI_PADDING          (10)
#define MLK_UI_BORDER           (2)

enum mlk_align;

enum mlk_ui_font {
	MLK_UI_FONT_INTERFACE,
	MLK_UI_FONT_LAST
};

extern struct mlk_font mlk_ui_fonts[MLK_UI_FONT_LAST];

#if defined(__cplusplus)
extern "C" {
#endif

int
mlk_ui_init(void);

/* TODO: probably better somewhere else? */
void
mlk_ui_draw_text(enum mlk_align align,
                 struct mlk_font *font,
                 unsigned long color,
                 const char *text,
                 int x,
                 int y,
                 unsigned int w,
                 unsigned int h);

void
mlk_ui_finish(void);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_UI_UI_H */
