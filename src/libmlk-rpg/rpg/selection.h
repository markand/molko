/*
 * selection.h -- kind of selection
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

#ifndef MLK_RPG_SELECTION_H
#define MLK_RPG_SELECTION_H

#include <core/core.h>

struct battle;

enum selection_kind {
	SELECTION_KIND_SELF,
	SELECTION_KIND_ONE,
	SELECTION_KIND_ALL,
	SELECTION_KIND_BOTH
};

enum selection_side {
	/* Which side allowed (can be both). */
	SELECTION_SIDE_TEAM     = (1 << 0),
	SELECTION_SIDE_ENEMY    = (1 << 1)
};

struct selection {
	enum selection_kind allowed_kinds;
	enum selection_side allowed_sides;
	
	/* Character index in battle entity array. */
	unsigned int index_character;

	/* Side index (0 = enemy, 1 = team). */
	unsigned int index_side;
};

CORE_BEGIN_DECLS

void
selection_first(struct selection *, const struct battle *);

void
selection_random(struct selection *, const struct battle *);

CORE_END_DECLS

#endif /* !MLK_RPG_SELECTION_H */
