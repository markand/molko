/*
 * battle-indicator.h -- drawable for rendering a hp/mp count usage
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

#ifndef MLK_RPG_BATTLE_INDICATOR_H
#define MLK_RPG_BATTLE_INDICATOR_H

#include <mlk/core/core.h>
#include <mlk/core/texture.h>

#define BATTLE_INDICATOR_HP_COLOR (0xa5303000)
#define BATTLE_INDICATOR_MP_COLOR (0xa23e8c00)

struct theme;

struct battle_indicator {
	unsigned int color;
	unsigned int amount;
	const struct theme *theme;
	unsigned int cur;
	unsigned int elapsed;
	unsigned int alpha;
	struct texture tex[2];
};

CORE_BEGIN_DECLS

void
battle_indicator_start(struct battle_indicator *);

int
battle_indicator_completed(const struct battle_indicator *);

int
battle_indicator_ok(const struct battle_indicator *);

int
battle_indicator_update(struct battle_indicator *, unsigned int);

void
battle_indicator_draw(const struct battle_indicator *, int, int);

void
battle_indicator_finish(struct battle_indicator *);

CORE_END_DECLS

#endif /* !MLK_RPG_BATTLE_INDICATOR_H */
