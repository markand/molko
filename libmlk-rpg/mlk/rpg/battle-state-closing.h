/*
 * battle-state-closing.h -- battle state (closing)
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

#ifndef MLK_RPG_BATTLE_STATE_CLOSING_H
#define MLK_RPG_BATTLE_STATE_CLOSING_H

#include <mlk/core/texture.h>

struct battle;

struct battle_state_closing {
	struct texture texture;
	unsigned int alpha;
	unsigned int elapsed;
};

void
battle_state_closing_init(struct battle_state_closing *);

int
battle_state_closing_update(struct battle_state_closing *, unsigned int);

void
battle_state_closing_draw(const struct battle_state_closing *, const struct battle *);

void
battle_state_closing_finish(struct battle_state_closing *);

void
battle_state_closing(struct battle *);

#endif /* !MLK_RPG_BATTLE_STATE_CLOSING_H */
