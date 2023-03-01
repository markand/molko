/*
 * checkbox.h -- GUI checkbox
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

#ifndef MLK_UI_CHECKBOX_H
#define MLK_UI_CHECKBOX_H

#include <mlk/core/core.h>

union mlk_event;

struct mlk_checkbox;

struct mlk_checkbox_style {
	void *data;
	unsigned long bg_color;
	unsigned long border_color;
	unsigned long check_color;
	void (*init)(struct mlk_checkbox_style *, struct mlk_checkbox *);
	void (*update)(struct mlk_checkbox_style *, struct mlk_checkbox *, unsigned int);
	void (*draw)(struct mlk_checkbox_style *, const struct mlk_checkbox *);
	void (*finish)(struct mlk_checkbox_style *, struct mlk_checkbox *);
};

struct mlk_checkbox {
	int x;
	int y;
	unsigned int w;
	unsigned int h;
	int checked;
	struct mlk_checkbox_style *style;
};

extern struct mlk_checkbox_style mlk_checkbox_style;

MLK_CORE_BEGIN_DECLS

void
mlk_checkbox_init(struct mlk_checkbox *);

int
mlk_checkbox_handle(struct mlk_checkbox *, const union mlk_event *);

void
mlk_checkbox_draw(const struct mlk_checkbox *);

void
mlk_checkbox_finish(struct mlk_checkbox *);

MLK_CORE_END_DECLS

#endif /* !MLK_UI_CHECKBOX_H */
