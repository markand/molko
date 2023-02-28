/*
 * button.h -- GUI button
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

#ifndef MLK_UI_BUTTON_H
#define MLK_UI_BUTTON_H

#include <mlk/core/core.h>

union mlk_event;

struct mlk_theme;

enum mlk_button_state {
	MLK_BUTTON_STATE_NONE,
	MLK_BUTTON_STATE_PRESSED,
	MLK_BUTTON_STATE_ACTIVATED
};

struct mlk_button {
	int x;
	int y;
	unsigned int w;
	unsigned int h;
	const char *text;
	enum mlk_button_state state;
	const struct mlk_theme *theme;
};

MLK_CORE_BEGIN_DECLS

int
mlk_button_handle(struct mlk_button *, const union mlk_event *);

void
mlk_button_reset(struct mlk_button *);

void
mlk_button_draw_default(const struct mlk_theme *, const struct mlk_button *);

void
mlk_button_draw(const struct mlk_button *);

MLK_CORE_END_DECLS

#endif /* !MLK_UI_BUTTON_H */
