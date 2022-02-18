/*
 * label.h -- GUI label
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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

#ifndef MLK_UI_LABEL_H
#define MLK_UI_LABEL_H

#include <core/core.h>

struct action;
struct theme;

enum label_flags {
	LABEL_FLAGS_NONE,
	LABEL_FLAGS_SHADOW      = (1 << 0),
	LABEL_FLAGS_IMPORTANT   = (1 << 1),
	LABEL_FLAGS_SELECTED    = (1 << 2)
};

struct label {
	int x;
	int y;
	const char *text;
	enum label_flags flags;
	const struct theme *theme;
};

CORE_BEGIN_DECLS

void
label_draw_default(const struct theme *, const struct label *);

int
label_ok(const struct label *);

void
label_query(const struct label *, unsigned int *, unsigned int *);

void
label_draw(const struct label *);

void
label_action(struct label *, struct action *);

CORE_END_DECLS

#endif /* !MLK_UI_LABEL_H */
