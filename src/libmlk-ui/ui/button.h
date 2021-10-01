/*
 * button.h -- GUI button
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

#ifndef MOLKO_UI_BUTTON_H
#define MOLKO_UI_BUTTON_H

#include <core/core.h>

union event;

struct action;
struct theme;

enum button_state {
	BUTTON_STATE_NONE,
	BUTTON_STATE_PRESSED,
	BUTTON_STATE_ACTIVATED
};

struct button {
	int x;
	int y;
	unsigned int w;
	unsigned int h;
	const char *text;
	enum button_state state;
	const struct theme *theme;
};

CORE_BEGIN_DECLS

void
button_handle(struct button *, const union event *);

void
button_reset(struct button *);

void
button_draw_default(const struct theme *, const struct button *);

void
button_draw(const struct button *);

void
button_action(struct button *, struct action *);

CORE_END_DECLS

#endif /* !MOLKO_UI_BUTTON_H */
